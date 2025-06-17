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

        std::unique_ptr<Board> simulated = board.createSimulatedCopy();
        try {
            simulated->movePiece(fromX, fromY, toX, toY);
        } catch (...) {
            return INT_MIN;  // בלי delete!
        }


        int worstEnemyScore = 0;
        auto enemyMoves = suggestMovesDepth(*simulated, !isWhiteTurn, 0, depth - 1, thresholdScore);
        if (!enemyMoves.empty()) {
            worstEnemyScore = enemyMoves[0].score;
        }

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
                                        unique_ptr<Board> simulated = board.createSimulatedCopy();
                                        try {
                                            simulated->movePiece(fromX, fromY, toX, toY);
                                        } catch (...) {
                                            continue;
                                        }

                                        int worstEnemyScore = 0;
                                        auto enemyMoves = suggestMovesDepth(*simulated, !isWhiteTurn, 0, depth - 1, thresholdScore);
                                        if (!enemyMoves.empty()) {
                                            worstEnemyScore = enemyMoves[0].score;
                                        }

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

bool Model::isCheckmate(const Board& board, bool isWhiteTurn) {
    // אם אין שח – זה לא שחמט
    if (!board.isKingInCheck(isWhiteTurn)) {
        return false;
    }

    // עבור כל כלי של השחקן הנוכחי
    for (int fromX = 0; fromX < 8; ++fromX) {
        for (int fromY = 0; fromY < 8; ++fromY) {
            Piece* piece = board.getPieceAt(fromX, fromY);
            if (!piece || piece->isWhite() != isWhiteTurn)
                continue;

            // נבדוק כל תא אפשרי שאליו אפשר לזוז
            for (int toX = 0; toX < 8; ++toX) {
                for (int toY = 0; toY < 8; ++toY) {
                    if (fromX == toX && fromY == toY)
                        continue;

                    if (!piece->isValidMove(fromX, fromY, toX, toY, board))
                        continue;

                    // ננסה לבצע את המהלך על העתק
                    std::unique_ptr<Board> simulated = board.createSimulatedCopy();
                    try {
                        simulated->movePiece(fromX, fromY, toX, toY);
                        if (!simulated->isKingInCheck(isWhiteTurn)) {
                            return false;  // יש תנועה חוקית שמונעת שח
                        }
                    } catch (...) {
                        continue;  // תנועה לא חוקית בפועל – נמשיך לבדוק
                    }
                }
            }
        }
    }

    return true;  // אין אף מהלך שמוציא מהשח ⇒ שחמט
}

bool Model::isStalemate(const Board& board, bool isWhiteTurn) const {
    // אם השחקן בשח – זה לא תיקו אלא אולי שחמט
    if (board.isKingInCheck(isWhiteTurn)) {
        return false;
    }

    // עבור כל הכלים של השחקן
    for (int fromX = 0; fromX < 8; ++fromX) {
        for (int fromY = 0; fromY < 8; ++fromY) {
            Piece* piece = board.getPieceAt(fromX, fromY);
            if (!piece || piece->isWhite() != isWhiteTurn)
                continue;

            for (int toX = 0; toX < 8; ++toX) {
                for (int toY = 0; toY < 8; ++toY) {
                    if (fromX == toX && fromY == toY)
                        continue;

                    if (!piece->isValidMove(fromX, fromY, toX, toY, board))
                        continue;

                    std::unique_ptr<Board> simulated = board.createSimulatedCopy();
                    try {
                        simulated->movePiece(fromX, fromY, toX, toY);
                        // אם המהלך לא גרם לשח – זה מהלך חוקי ⇒ לא תיקו
                        return false;
                    } catch (...) {
                        continue;
                    }
                }
            }
        }
    }

    return true;  // אין אף מהלך חוקי ⇒ תיקו
}

bool Model::isInsufficientMaterial(const Board& board) const {
    int whiteBishops = 0, blackBishops = 0;
    int whiteKnights = 0, blackKnights = 0;
    int whiteOther = 0, blackOther = 0;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = board.getPieceAt(i, j);
            if (!piece) continue;

            std::string name = piece->getName();
            bool isWhite = piece->isWhite();

            if (name == "K" || name == "k") continue;  // המלך לא נחשב ככלי נוסף
            if (name == "N" || name == "n") {
                if (isWhite) whiteKnights++; else blackKnights++;
            } else if (name == "B" || name == "b") {
                if (isWhite) whiteBishops++; else blackBishops++;
            } else {
                if (isWhite) whiteOther++; else blackOther++;
            }
        }
    }

    // מלך בלבד
    if (whiteBishops + whiteKnights + whiteOther == 0 &&
        blackBishops + blackKnights + blackOther == 0) {
        return true;
    }

    // מלך + רץ או פרש מול מלך
    if ((whiteOther == 0 && blackOther == 0) &&
        ((whiteBishops + whiteKnights <= 1 && blackBishops + blackKnights == 0) ||
         (blackBishops + blackKnights <= 1 && whiteBishops + whiteKnights == 0))) {
        return true;
    }

    return false;  // יש מספיק כלים לבצע מט
}











