//
// Created by HALAA on 12/05/2025.
//
#include "Model.h"
#include "PriorityQueue.h"
#include "MyComparator.h"


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

std::vector<Move> Model::suggestMoves(const Board& board, bool isWhiteTurn) {
    PriorityQueue<Move, MyComparator<Move>> moveQueue;

    // Loop over all board squares to find pieces of the current player
    for (int fromX = 0; fromX < 8; ++fromX) {
        for (int fromY = 0; fromY < 8; ++fromY) {
            Piece* piece = board.getPieceAt(fromX, fromY);
            if (!piece || piece->isWhite() != isWhiteTurn) continue;

            // Try all possible destination squares for this piece
            for (int toX = 0; toX < 8; ++toX) {
                for (int toY = 0; toY < 8; ++toY) {
                    if (fromX == toX && fromY == toY) continue; // skip move to same square

                    if (piece->isValidMove(fromX, fromY, toX, toY, board)) {
                        int score = evaluateMove(fromX, fromY, toX, toY, board);
                        if (score > -500) {
                            moveQueue.insert({fromX, fromY, toX, toY, score});
                        }
                    }
                }
            }
        }
    }

    // Extract all moves from priority queue (sorted by best score)
    std::vector<Move> bestMoves;
    while (!moveQueue.isEmpty()) {
        bestMoves.push_back(moveQueue.popMax());
    }

    return bestMoves;
}

std::vector<Move> Model::suggestMovesDepth2(const Board& board, bool isWhiteTurn) {
    PriorityQueue<Move, MyComparator<Move>> moveQueue;

    for (int fromX = 0; fromX < 8; ++fromX) {
        for (int fromY = 0; fromY < 8; ++fromY) {
            Piece* piece = board.getPieceAt(fromX, fromY);
            if (!piece || piece->isWhite() != isWhiteTurn) continue;

            for (int toX = 0; toX < 8; ++toX) {
                for (int toY = 0; toY < 8; ++toY) {
                    if (fromX == toX && fromY == toY) continue;
                    if (!piece->isValidMove(fromX, fromY, toX, toY, board)) continue;

                    //  Initial score for my step
                    int myScore = evaluateMove(fromX, fromY, toX, toY, board);

                    //  Create a mock board
                    Board* simulated = board.createSimulatedCopy();
                    try {
                        simulated->movePiece(fromX, fromY, toX, toY);
                    } catch (...) {
                        delete simulated;
                        continue;
                    }


                    //  Calculate what the best the opponent can do after my move
                    int worstEnemyScore = 0;
                    auto enemyMoves = suggestMoves(*simulated, !isWhiteTurn);
                    if (!enemyMoves.empty()) {
                        worstEnemyScore = enemyMoves[0].score; // The worst for me
                    }

                    delete simulated; // Releasing the dummy copy

                    int finalScore = myScore - worstEnemyScore;
                    moveQueue.insert({fromX, fromY, toX, toY, finalScore});
                }
            }
        }
    }

    std::vector<Move> bestMoves;
    while (!moveQueue.isEmpty()) {
        bestMoves.push_back(moveQueue.popMax());
    }

    return bestMoves;
}





