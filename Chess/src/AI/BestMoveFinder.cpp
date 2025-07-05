#include "AI/BestMoveFinder.h"
#include "Utils/ThreadPool.h"
#include "Utils/ThreadSafePriorityQueue.h"
#include "Utils/Colors.h" // Needed for Colors::Pieces constants
#include <algorithm>
#include <climits>
#include <future>
#include <vector>
#include <cctype>

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

// --- Helper: findBestMoves - finds the best moves for a given board state using alpha-beta pruning.
std::vector<MoveScorePair>
findBestMoves(const Board& board, bool isWhite, int limit, int searchDepth) // Added searchDepth parameter
{
    std::vector<MoveScorePair> candidates;

    std::vector<CMove> legalMoves = board.generateLegalMoves(isWhite);
    candidates.reserve(legalMoves.size());

    for (const CMove& move : legalMoves) {
        Board tempBoard = board;
        tempBoard.applyMove(move);
        int score = minimax_ab(tempBoard, searchDepth - 1, INT_MIN, INT_MAX, false, isWhite);
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
              int threadCount,
              int searchDepth) 
{
    if (threadCount <= 1) {
        return findBestMoves(board, isWhite, limit, searchDepth); // Pass searchDepth to single-threaded version
    }

    ThreadPool pool(threadCount);
    ThreadSafePriorityQueue<MoveScorePair> results_queue;
    std::vector<std::future<void>> futures;

    std::vector<CMove> legalMoves = board.generateLegalMoves(isWhite);

    for (const auto& move : legalMoves) {
        futures.emplace_back(
            pool.enqueue([&board, move, isWhite, searchDepth, &results_queue] {
                Board tempBoard = board;
                tempBoard.applyMove(move);
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
int BestMoveFinder::pieceValue(const std::string& symbol)
{
    if (symbol == Colors::Pieces::WHITE_PAWN || symbol == Colors::Pieces::BLACK_PAWN) return 1;
    if (symbol == Colors::Pieces::WHITE_KNIGHT || symbol == Colors::Pieces::BLACK_KNIGHT) return 3;
    if (symbol == Colors::Pieces::WHITE_BISHOP || symbol == Colors::Pieces::BLACK_BISHOP) return 3;
    if (symbol == Colors::Pieces::WHITE_ROOK || symbol == Colors::Pieces::BLACK_ROOK) return 5;
    if (symbol == Colors::Pieces::WHITE_QUEEN || symbol == Colors::Pieces::BLACK_QUEEN) return 9;
    return 0; // King or empty square
}

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
    auto bestMoves = AI::findBestMoves(board, isWhite, 1, 3); // Default depth 3 if not specified
    if (bestMoves.empty()) {
        return { CMove{}, isWhite ? INT_MIN : INT_MAX };
    }
    return bestMoves[0];
}

} // namespace AI