
#include "ChessMultithread.h"

ChessMultithread::ChessMultithread(Board& board, bool isWhiteTurn)
    : _board(board), _isWhiteTurn(isWhiteTurn), _stopFlag(false) {}

int ChessMultithread::evaluateMove(int fromRow, int fromCol, int toRow, int toCol) {
    Piece* target = _board.getPiece(toRow, toCol);
    int score = 0;
    if (target) score += target->getValue();
    if (score >= 100) {
        _stopFlag.store(true);
    }
    return score;
}

std::vector<Move> ChessMultithread::getTopMoves(int threadCount, int maxMoves) {
    PriorityQueue<Move, MyComparator> pq;
    std::vector<std::thread> threads;
    std::vector<std::pair<int, int>> whitePieces;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = _board.getPiece(r, c);
            if (p && p->isWhite() == _isWhiteTurn)
                whitePieces.emplace_back(r, c);
        }
    }

    int total = whitePieces.size();
    int perThread = (total + threadCount - 1) / threadCount;

    for (int t = 0; t < threadCount; ++t) {
        threads.emplace_back([&, t]() {
            int start = t * perThread;
            int end = std::min(start + perThread, total);

            for (int i = start; i < end && !_stopFlag.load(); ++i) {
                int r = whitePieces[i].first;
                int c = whitePieces[i].second;

                for (int dr = 0; dr < 8; ++dr) {
                    for (int dc = 0; dc < 8; ++dc) {
                        if ((r != dr || c != dc)) {
                            Piece* piece = _board.getPiece(r, c);
                            if (piece && piece->isLegalMove(r, c, dr, dc, _board.toString())
                                 && piece->isClearPath(r, c, dr, dc, _board.toString())) {
                                int score = evaluateMove(r, c, dr, dc);
                                if (_stopFlag.load()) break;
                                std::lock_guard<std::mutex> lock(_pqMutex);
                                pq.push({r, c, dr, dc, score});
                            }
                        }
                    }
                }
            }
        });
    }

    for (auto& th : threads) {
        if (th.joinable())
            th.join();
    }

    std::vector<Move> result;
    while (!pq.empty() && result.size() < maxMoves) {
        result.push_back(pq.poll());
    }
    return result;
}
