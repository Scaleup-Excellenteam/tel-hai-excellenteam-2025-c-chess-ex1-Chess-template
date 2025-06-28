#include "AI/BestMoveFinder.h"
#include "Utils/ThreadPool.h"
#include "Utils/ThreadSafePriorityQueue.h"
#include "Utils/Colors.h" // Needed for Colors::Pieces constants
#include <algorithm>
#include <climits>
#include <future>
#include <vector>
#include <cctype> // For std::tolower if still needed, but not directly for Unicode strings

namespace AI
{

// Evaluates the board based on material. Positive score for white, negative for black.
int evaluateBoard(const Board& board) {
    int totalScore = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece* p = board.getPiece(r, c);
            if (p) {
                // p->getSymbol() now returns std::string, pass to updated pieceValue
                int pieceVal = BestMoveFinder::pieceValue(p->getSymbol());
                if (p->getIsWhite()) {
                    totalScore += pieceVal;
                } else {
                    totalScore -= pieceVal;
                }
            }
        }
    }
    return totalScore;
}


// Minimax algorithm with Alpha-Beta Pruning.
int minimax_ab(const Board& board, int depth, int alpha, int beta, bool isMaximizingPlayer, bool isWhiteAI) {
    // Base case: if we've reached the desired depth, return the static board evaluation.
    if (depth == 0) {
        return evaluateBoard(board);
    }

    const bool currentTurnIsWhite = (isWhiteAI == isMaximizingPlayer); // isMaximizingPlayer is true if AI's turn
    std::vector<CMove> legalMoves = board.generateLegalMoves(currentTurnIsWhite);

    // Base case 2: If no legal moves, the game is over (checkmate or stalemate).
    if (legalMoves.empty()) {
        if (board.inCheck(currentTurnIsWhite)) {
            // Checkmate: If it's the maximizing player's turn and they are in checkmate, it's a very bad score.
            // If it's the minimizing player's turn and they are in checkmate, it's a very good score for maximizing.
            return isMaximizingPlayer ? INT_MIN : INT_MAX; // Checkmate
        }
        return 0; // Stalemate
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        for (const auto& move : legalMoves) {
            Board tempBoard = board;
            tempBoard.applyMove(move);
            // Recursive call for the opponent's turn (minimizing player)
            int eval = minimax_ab(tempBoard, depth - 1, alpha, beta, false, isWhiteAI);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Beta cutoff
            }
        }
        return maxEval;
    } else { // Minimizing player
        int minEval = INT_MAX;
        for (const auto& move : legalMoves) {
            Board tempBoard = board;
            tempBoard.applyMove(move);
            // Recursive call for the AI's turn (maximizing player)
            int eval = minimax_ab(tempBoard, depth - 1, alpha, beta, true, isWhiteAI);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }
        return minEval;
    }
}


// Single-threaded findBestMoves updated to use alpha-beta.
std::vector<MoveScorePair>
findBestMoves(const Board& board, bool isWhite, int limit)
{
    std::vector<MoveScorePair> candidates;
    const int searchDepth = 3; // Default or adjustable by parameter if needed

    std::vector<CMove> legalMoves = board.generateLegalMoves(isWhite);
    candidates.reserve(legalMoves.size());

    for (const CMove& move : legalMoves) {
        Board tempBoard = board;
        tempBoard.applyMove(move);
        // Initial call to minimax_ab: if it's white's turn, and AI is white, then AI is maximizing.
        // Or generally, the AI is always the maximizing player in its root call.
        int score = minimax_ab(tempBoard, searchDepth - 1, INT_MIN, INT_MAX, false, isWhite); // isWhite here refers to AI's color
        candidates.push_back({ move, score });
    }

    std::stable_sort(candidates.begin(), candidates.end(),
                     [](const MoveScorePair& a, const MoveScorePair& b) {
                         return a.score > b.score;
                     });

    if (limit > 0 && static_cast<int>(candidates.size()) > limit) {
        candidates.resize(limit);
    }

    return candidates;
}


// Multithreaded findBestMoves updated to use alpha-beta.
std::vector<MoveScorePair>
findBestMoves(const Board& board,
              bool isWhite,
              int limit,
              int threadCount)
{
    if (threadCount <= 1) {
        return findBestMoves(board, isWhite, limit);
    }

    ThreadPool pool(threadCount);
    // Using std::priority_queue directly with a mutex would be simpler than ThreadSafePriorityQueue for this use case
    // as we collect all results and then sort. ThreadSafePriorityQueue is good if you need to pop highest priority
    // items *during* computation. For collecting all results, a std::vector of futures or a simple mutex-protected
    // std::vector + sort at the end is standard. But let's stick to ThreadSafePriorityQueue since it's already there.
    ThreadSafePriorityQueue<MoveScorePair> results_queue;
    std::vector<std::future<void>> futures;

    std::vector<CMove> legalMoves = board.generateLegalMoves(isWhite);
    const int searchDepth = 3; // Default or adjustable by parameter if needed

    for (const auto& move : legalMoves) {
        futures.emplace_back(
            pool.enqueue([&board, move, isWhite, searchDepth, &results_queue] {
                Board tempBoard = board;
                tempBoard.applyMove(move);
                // Each thread runs an alpha-beta search for a top-level move.
                // isWhite in the minimax_ab call here should be the AI's color.
                int score = minimax_ab(tempBoard, searchDepth - 1, INT_MIN, INT_MAX, false, isWhite);
                results_queue.push({ move, score });
            })
        );
    }

    for (auto& f : futures) {
        f.get(); // Wait for all tasks to complete
    }

    std::vector<MoveScorePair> candidates;
    MoveScorePair temp_move;
    while(results_queue.try_pop(temp_move)) {
        candidates.push_back(temp_move);
    }

    std::stable_sort(candidates.begin(), candidates.end(),
                     [](const MoveScorePair& a, const MoveScorePair& b) {
                         return a.score > b.score; // Sort descending by score (best first)
                     });

    if (limit > 0 && static_cast<int>(candidates.size()) > limit) {
        candidates.resize(limit);
    }

    return candidates;
}


// --- Helper: convert piece symbol (string) to a crude material value. ---
int BestMoveFinder::pieceValue(const std::string& symbol) // <--- CHANGED PARAMETER TYPE
{
    // Compare with the full Unicode string symbols
    if (symbol == Colors::Pieces::WHITE_PAWN || symbol == Colors::Pieces::BLACK_PAWN) return 1;
    if (symbol == Colors::Pieces::WHITE_KNIGHT || symbol == Colors::Pieces::BLACK_KNIGHT) return 3;
    if (symbol == Colors::Pieces::WHITE_BISHOP || symbol == Colors::Pieces::BLACK_BISHOP) return 3;
    if (symbol == Colors::Pieces::WHITE_ROOK || symbol == Colors::Pieces::BLACK_ROOK) return 5;
    if (symbol == Colors::Pieces::WHITE_QUEEN || symbol == Colors::Pieces::BLACK_QUEEN) return 9;
    return 0; // King or empty square
}

// This is no longer the primary evaluation method but can be kept for other purposes.
// It will now also use the std::string symbol.
int BestMoveFinder::evaluateMove(const Board& board, const CMove& move) const
{
    const Piece* src = board.getPiece(move.srcRow,  move.srcCol);
    const Piece* dst = board.getPiece(move.destRow, move.destCol);
    if (!src) return 0;
    if (dst && dst->getIsWhite() != src->getIsWhite()) {
        return pieceValue(dst->getSymbol()); // Use updated pieceValue
    }
    return 0;
}

MoveScorePair BestMoveFinder::findBestMove(const Board& board, bool isWhite) const
{
    // Default limit for findBestMove that's not multithreaded
    auto bestMoves = findBestMoves(board, isWhite, 1);
    if (bestMoves.empty()) {
        return { CMove{}, isWhite ? INT_MIN : INT_MAX }; // Return very low/high score if no moves
    }
    return bestMoves[0];
}

} // namespace AI