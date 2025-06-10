#include "AI/BestMoveFinder.h"
#include "Utils/ThreadPool.h"
#include "Utils/ThreadSafePriorityQueue.h"
#include <algorithm>
#include <climits>
#include <future>
#include <vector>

namespace AI
{

// Evaluates the board based on material. Positive score for white, negative for black.
int evaluateBoard(const Board& board) {
    int totalScore = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece* p = board.getPiece(r, c);
            if (p) {
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

    const bool currentTurnIsWhite = (isWhiteAI == isMaximizingPlayer);
    std::vector<CMove> legalMoves = board.generateLegalMoves(currentTurnIsWhite);

    // Base case 2: If no legal moves, the game is over (checkmate or stalemate).
    if (legalMoves.empty()) {
        if (board.inCheck(currentTurnIsWhite)) {
            return isMaximizingPlayer ? INT_MIN : INT_MAX; // Checkmate
        }
        return 0; // Stalemate
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        for (const auto& move : legalMoves) {
            Board tempBoard = board;
            tempBoard.applyMove(move);
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
    const int searchDepth = 3; // Higher depth is smarter but slower.

    std::vector<CMove> legalMoves = board.generateLegalMoves(isWhite);
    candidates.reserve(legalMoves.size());

    for (const CMove& move : legalMoves) {
        Board tempBoard = board;
        tempBoard.applyMove(move);
        // Initial call to minimax_ab with alpha=-inf and beta=+inf
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
              int threadCount)
{
    if (threadCount <= 1) {
        return findBestMoves(board, isWhite, limit);
    }

    ThreadPool pool(threadCount);
    ThreadSafePriorityQueue<MoveScorePair> results_queue;
    std::vector<std::future<void>> futures;

    std::vector<CMove> legalMoves = board.generateLegalMoves(isWhite);
    const int searchDepth = 3;

    for (const auto& move : legalMoves) {
        futures.emplace_back(
            pool.enqueue([&board, move, isWhite, searchDepth, &results_queue] {
                Board tempBoard = board;
                tempBoard.applyMove(move);
                // Each thread runs an alpha-beta search for a top-level move.
                int score = minimax_ab(tempBoard, searchDepth - 1, INT_MIN, INT_MAX, false, isWhite);
                results_queue.push({ move, score });
            })
        );
    }

    for (auto& f : futures) {
        f.get();
    }

    std::vector<MoveScorePair> candidates;
    MoveScorePair temp_move;
    while(results_queue.try_pop(temp_move)) {
        candidates.push_back(temp_move);
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


// --- Helper methods remain unchanged ---

int BestMoveFinder::pieceValue(char symbol)
{
    switch (std::tolower(static_cast<unsigned char>(symbol)))
    {
        case 'p': return 1;
        case 'n':
        case 'b': return 3;
        case 'r': return 5;
        case 'q': return 9;
        default : return 0;
    }
}

// This is no longer the primary evaluation method but can be kept for other purposes.
int BestMoveFinder::evaluateMove(const Board& board, const CMove& move) const
{
    const Piece* src = board.getPiece(move.srcRow,  move.srcCol);
    const Piece* dst = board.getPiece(move.destRow, move.destCol);
    if (!src) return 0;
    if (dst && dst->getIsWhite() != src->getIsWhite()) {
        return pieceValue(dst->getSymbol());
    }
    return 0;
}

MoveScorePair BestMoveFinder::findBestMove(const Board& board, bool isWhite) const
{
    auto bestMoves = findBestMoves(board, isWhite, 1);
    if (bestMoves.empty()) {
        return { CMove{}, isWhite ? INT_MIN : INT_MAX };
    }
    return bestMoves[0];
}

} // namespace AI