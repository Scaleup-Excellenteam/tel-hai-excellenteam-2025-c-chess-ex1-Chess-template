//
// Created by HALAA on 12/05/2025.
//
#include "Model.h"
#include "PriorityQueue.h"
#include "MyComparator.h"
#include "ThreadPool.h"


int Model::evaluateMove(int fromX, int fromY, int toX, int toY, const Board& board) {
    int score = 0;

    Piece* fromPiece = board.getPieceAt(fromX, fromY);
    Piece* toPiece = board.getPieceAt(toX, toY);

    // Sanity check – invalid if no piece or move is not legal
    if (!fromPiece) return -999;
    if (!fromPiece->isValidMove(fromX, fromY, toX, toY, board)) return -1000;

    bool isWhite = fromPiece->isWhite();
    int myValue = getPieceValue(fromPiece->getName());

    // Condition 3: if this move captures an enemy piece, add its value
    if (toPiece && toPiece->isWhite() != isWhite) {
        score += getPieceValue(toPiece->getName());
    }

    //  Condition 1: does this move place our piece in danger from weaker enemy pieces?
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* enemy = board.getPieceAt(i, j);
            if (enemy && enemy->isWhite() != isWhite &&
                enemy->isValidMove(i, j, toX, toY, board)) {
                int enemyValue = getPieceValue(enemy->getName());
                if (enemyValue < myValue) {
                    score -= (myValue - enemyValue); // penalty for being threatened by weaker piece
                }
            }
        }
    }

    // Condition 2: after this move, do we threaten a stronger enemy piece?
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* target = board.getPieceAt(i, j);
            if (target && target->isWhite() != isWhite &&
                fromPiece->isValidMove(toX, toY, i, j, board)) {
                int targetValue = getPieceValue(target->getName());
                if (targetValue > myValue) {
                    score += (targetValue - myValue); // bonus for threatening a stronger piece
                }
            }
        }
    }
    /*
    // Logging (debug): explain why this score was assigned
    std::cout << std::left << std::setw(10) << "Move:"
              << char('a' + fromY) << fromX + 1 << " -> "
              << char('a' + toY) << toX + 1
             << " | ";

    if (toPiece && toPiece->isWhite() != isWhite) {
        std::cout << "Ate " << toPiece->getName()
                  << " (+ " << getPieceValue(toPiece->getName()) << ") ";
    }

    // Log threats from weaker enemy pieces
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* enemy = board.getPieceAt(i, j);
            if (enemy && enemy->isWhite() != isWhite &&
                enemy->isValidMove(i, j, toX, toY, board)) {
                int enemyValue = getPieceValue(enemy->getName());
                if (enemyValue < myValue) {
                    std::cout << "| Threatened by weaker " << enemy->getName()
                              << " (- " << (myValue - enemyValue) << ") ";
                }
            }
        }
    }

    // Log threats against stronger pieces
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* target = board.getPieceAt(i, j);
            if (target && target->isWhite() != isWhite &&
                fromPiece->isValidMove(toX, toY, i, j, board)) {
                int targetValue = getPieceValue(target->getName());
                if (targetValue > myValue) {
                    std::cout << "| Threatens stronger " << target->getName()
                              << " (+ " << (targetValue - myValue) << ") ";
                }
            }
        }
    }

    std::cout << "| Final Score = " << score << "\n";
    */
    return score;
}



int Model::getPieceValue(const std::string& name) {
    if (name == "P" || name == "p") return 1;
    if (name == "N" || name == "n") return 3;
    if (name == "B" || name == "b") return 3;
    if (name == "R" || name == "r") return 5;
    if (name == "Q" || name == "q") return 9;
    if (name == "K" || name == "k") return 100;
    return 0;
}

std::vector<Move> Model::suggestMovesDepth(const Board& board, bool isWhiteTurn, int numThreads, int depth, int const thresholdScore) {
    PriorityQueue<Move, MyComparator<Move>> moveQueue;
    std::mutex queue_mutex;
    std::atomic<bool> stopFlag = false;

    auto evaluateWithDepth = [&](int fromX, int fromY, int toX, int toY) -> int {
        int myScore = evaluateMove(fromX, fromY, toX, toY, board);
        if (depth <= 1)
            return myScore;

        Board* simulated = board.createSimulatedCopy();
        try {
            simulated->movePiece(fromX, fromY, toX, toY);
        } catch (...) {
            delete simulated;
            return INT_MIN;
        }

        int worstEnemyScore = 0;
        auto enemyMoves = suggestMovesDepth(*simulated, !isWhiteTurn, 0, depth - 1, thresholdScore);
        if (!enemyMoves.empty()) {
            worstEnemyScore = enemyMoves[0].score;
        }

        delete simulated;
        return myScore - worstEnemyScore;
    };

    if (numThreads == 0) {
        for (int fromX = 0; fromX < 8; ++fromX) {
            for (int fromY = 0; fromY < 8; ++fromY) {
                Piece* piece = board.getPieceAt(fromX, fromY);
                if (!piece || piece->isWhite() != isWhiteTurn) continue;

                for (int toX = 0; toX < 8; ++toX) {
                    for (int toY = 0; toY < 8; ++toY) {
                        if (fromX == toX && fromY == toY) continue;
                        if (!piece->isValidMove(fromX, fromY, toX, toY, board)) continue;

                        int finalScore = evaluateWithDepth(fromX, fromY, toX, toY);
                        if (finalScore != INT_MIN) {
                            if (finalScore >= thresholdScore) {
                                stopFlag = true;
                            }
                            moveQueue.insert({fromX, fromY, toX, toY, finalScore});
                        }
                        if (stopFlag) break;
                    }
                    if (stopFlag) break;
                }
                if (stopFlag) break;
            }
            if (stopFlag) break;
        }
    } else {
        ThreadPool pool(numThreads);
        std::atomic<int> completedTasks(0);
        std::condition_variable cv;
        std::mutex done_mutex;
        int totalPieces = 0;

        for (int fromX = 0; fromX < 8; ++fromX) {
            for (int fromY = 0; fromY < 8; ++fromY) {
                if (stopFlag) break;

                Piece *piece = board.getPieceAt(fromX, fromY);
                if (!piece || piece->isWhite() != isWhiteTurn) continue;

                ++totalPieces;
                Piece *myPiece = piece;

                pool.enqueue(
                        [fromX, fromY, myPiece, &board, &moveQueue, &queue_mutex, &completedTasks, &done_mutex, &cv, isWhiteTurn, depth, &stopFlag, thresholdScore]() {
                            for (int toX = 0; toX < 8; ++toX) {
                                for (int toY = 0; toY < 8; ++toY) {
                                    if (stopFlag) {
                                        std::cout << "[Thread for " << fromX << "," << fromY << "] Stopped early\n";
                                        {
                                            std::lock_guard<std::mutex> lock(done_mutex);
                                            ++completedTasks;
                                            cv.notify_one();
                                        }
                                        return;
                                    }

                                    if (fromX == toX && fromY == toY) continue;
                                    if (!myPiece->isValidMove(fromX, fromY, toX, toY, board)) continue;

                                    int finalScore = evaluateMove(fromX, fromY, toX, toY, board);

                                    if (depth > 1) {
                                        Board* simulated = board.createSimulatedCopy();
                                        try {
                                            simulated->movePiece(fromX, fromY, toX, toY);
                                        } catch (...) {
                                            delete simulated;
                                            continue;
                                        }

                                        int worstEnemyScore = 0;
                                        auto enemyMoves = suggestMovesDepth(*simulated, !isWhiteTurn, 0, depth - 1, thresholdScore);
                                        if (!enemyMoves.empty()) {
                                            worstEnemyScore = enemyMoves[0].score;
                                        }

                                        delete simulated;
                                        finalScore -= worstEnemyScore;
                                    }

                                    if (finalScore >= thresholdScore) {
                                        {
                                            std::lock_guard<std::mutex> lock(queue_mutex);
                                            moveQueue.insert({fromX, fromY, toX, toY, finalScore});
                                        }
                                        std::cout << "[Thread for " << fromX << "," << fromY
                                                  << "] High scoring move: " << finalScore
                                                  << " >= " << thresholdScore << " → setting stopFlag\n";
                                        stopFlag = true;
                                        return;
                                    }

                                    {
                                        std::lock_guard<std::mutex> lock(queue_mutex);
                                        moveQueue.insert({fromX, fromY, toX, toY, finalScore});
                                    }
                                }
                            }

                            {
                                std::lock_guard<std::mutex> lock(done_mutex);
                                ++completedTasks;
                                cv.notify_one();
                            }
                        });
            }
            if (stopFlag) break;
        }

        std::unique_lock<std::mutex> lock(done_mutex);
        cv.wait(lock, [&]() { return completedTasks == totalPieces || stopFlag; });
    }

    std::vector<Move> bestMoves;
    while (!moveQueue.isEmpty()) {
        bestMoves.push_back(moveQueue.popMax());
    }
    return bestMoves;
}









