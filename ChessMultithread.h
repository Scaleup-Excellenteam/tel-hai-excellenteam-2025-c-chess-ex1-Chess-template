
#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include "Move.h"
#include "MyComparator.h"
#include "PriorityQueue.h"
#include "Board.h"
#include "Piece.h"

class ChessMultithread {
public:
    ChessMultithread(Board& board, bool isWhiteTurn);
    std::vector<Move> getTopMoves(int threadCount = 4, int maxMoves = 3);

private:
    Board& _board;
    bool _isWhiteTurn;
    std::mutex _pqMutex;
    std::atomic<bool> _stopFlag;

    int evaluateMove(int fromRow, int fromCol, int toRow, int toCol);
};
