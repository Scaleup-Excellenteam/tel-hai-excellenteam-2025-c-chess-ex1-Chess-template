#pragma once
/**
 * @file BestMoveFinder.h
 * @brief A simple material-only evaluator that can pick either
 * a single best move or a short list of the top-N moves.
 *
 * ✔  Stateless, re-entrant, unit-test friendly
 * ✔  Easy to swap in a deeper search later
 */

#include <vector>
#include <climits>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>

#include "Board.h"
#include "../Utils/CMove.h"
#include "../Utils/MoveScorePair.h"
#include "../Utils/Colors.h"

namespace AI
{
    using ::MoveScorePair;

    class BestMoveFinder
    {
    public:
        BestMoveFinder() = default;

        MoveScorePair findBestMove(const Board& board,
                                   bool isWhite) const;

        int evaluateMove(const Board& board,
                         const CMove& move) const;

        static int pieceValue(const std::string& symbol);
    private:

        std::mutex pqMutex;
        std::atomic<bool> stopFlag{false};

    };

    std::vector<MoveScorePair> findBestMoves(const Board& board, bool isWhite, int limit);

    std::vector<MoveScorePair> findBestMoves(const Board& board, bool isWhite, int limit, int threadCount);

} // namespace AI