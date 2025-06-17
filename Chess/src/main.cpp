// Chess
#include "Chess.h"
#include "Board.h"
#include "Model.h"
#include "Move.h"
#include <iostream>
#include <algorithm>
#include "InvalidMoveBaseException.h"
#include "InvalidSuicideException.h"
#include "InvalidPromotionException.h"
#include <chrono>
#include <random>


using namespace std;

// Auto-play benchmark function (8 moves total)
void autoPlayBenchmark(int numThreads, bool useRandom, int depth, int const thresholdScore) {
    cout << "\nStarting auto-play benchmark with " << numThreads << " threads...\n";
    string initialBoard = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    Chess a(initialBoard);
    Board& board = Board::getInstance();

    auto start = chrono::high_resolution_clock::now();

    bool isWhiteTurn = true;
    int turn = 0;
    for (; turn < 8; ++turn) {
        cout << "\nTurn " << (turn + 1) << ": Player " << (isWhiteTurn ? "White" : "Black") << "'s move\n";

        auto moves = Model::suggestMovesDepth(board, isWhiteTurn, numThreads, depth, thresholdScore);

        if (!moves.empty() && moves[0].score >= thresholdScore) {
            std::cout << "Threshold reached with score " << moves[0].score << " – exiting benchmark early\n";
            break;
        }
        cout << "Got " << moves.size() << " moves.\n";

        if (moves.empty()) {
            cout << "No legal moves for " << (isWhiteTurn ? "White" : "Black") << "!\n";
            break;
        }

        // Choose move: either best or random
        Move chosen;
        if (useRandom) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, moves.size() - 1);
            chosen = moves[dis(gen)];
        } else {
            chosen = moves[0]; // Best move
        }

        try {
            board.movePiece(chosen.fromX, chosen.fromY, chosen.toX, chosen.toY);
        } catch (const InvalidMoveBaseException& e) {
            cout << "Invalid move skipped: " << e.what() << "\n";
            continue; // ננסה מהלך אחר
        }

        a.syncWithBoard(board);
        a.show();
        isWhiteTurn = !isWhiteTurn;
        if (Model().isCheckmate(board, isWhiteTurn)) {
            std::cout << "CHECKMATE! " << (isWhiteTurn ? "White" : "Black") << " loses.\n";
            break;
        }

        if (Model().isStalemate(board, isWhiteTurn)) {
            std::cout << "DRAW! Stalemate – no legal moves and not in check.\n";
            break;
        }

        if (Model().isInsufficientMaterial(board)) {
            std::cout << "DRAW! Insufficient material to checkmate.\n";
            break;
        }

    }

    auto end = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(end - start).count();

    cout << "Finished auto-play benchmark in " << elapsed << " seconds\n";
    cout << "============================\n";
}

int main() {
    int numThreads = 4;
    int depth = 2;
    bool useRandom = false;
    const int thresholdScore = 500; // BONUS

    cout << "Enter depth: ";
    cin >> depth;

    cout << "Choose game mode:\n";
    cout << "0 = Human vs Human\n";
    cout << "1 = Human vs Computer\n";
    cout << "2 = Computer vs Computer (Auto-play benchmark)\n";
    int mode;
    cin >> mode;

    if (mode == 2) {
        cout << "Enter number of threads (0, 2, 4, or 8): ";
        cin >> numThreads;
        cout << "Auto-play mode: choose move selection (0 = best move, 1 = random): ";
        int selection;
        cin >> selection;
        useRandom = (selection == 1);
        autoPlayBenchmark(numThreads, useRandom, depth, thresholdScore);
        return 0;
    }

    bool vsComputer = (mode == 1);
    string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    Chess a(board);
    Board& selfMadeBoard = Board::getInstance();
    int codeResponse = 0;
    bool isWhiteTurn = true;
    string res = "";

    Model model;

    a.syncWithBoard(selfMadeBoard);
    a.show();

    while (true) {
        auto bestMoves = Model::suggestMovesDepth(selfMadeBoard, isWhiteTurn, numThreads, depth, thresholdScore);

        cout << "Suggested moves for " << (isWhiteTurn ? "White" : "Black") << ":\n";
        for (int i = 0; i < min(3, (int)bestMoves.size()); ++i)
            cout << "  " << moveToNotation(bestMoves[i]) << " (Score: " << bestMoves[i].score << ")\n";

        a.setTurnForDisplay(isWhiteTurn);  // Set display turn explicitly

        if (!vsComputer) {
            res = a.getInput();

            if (res == "exit") break;

            int from_x = res[0] - 'a';
            int from_y = res[1] - '0' - 1;
            int to_x = res[2] - 'a';
            int to_y = res[3] - '0' - 1;

            try {
                codeResponse = selfMadeBoard.movePiece(from_x, from_y, to_x, to_y);
            } catch (const InvalidMoveBaseException& e) {
                cerr << "Invalid move: " << e.what() << endl;
                codeResponse = 12;
                continue;
            } catch (const InvalidSuicideException& e) {
                cerr << "Suicide move: " << e.what() << endl;
                codeResponse = 31;
                continue;
            } catch (const InvalidPromotionException& e) {
                cerr << "Invalid promotion: " << e.what() << endl;
                codeResponse = 32;
                continue;
            }
        } else {
            if (isWhiteTurn) {
                res = a.getInput();

                if (res == "exit") break;

                int from_x = res[0] - 'a';
                int from_y = res[1] - '0' - 1;
                int to_x = res[2] - 'a';
                int to_y = res[3] - '0' - 1;

                try {
                    codeResponse = selfMadeBoard.movePiece(from_x, from_y, to_x, to_y);
                } catch (const InvalidMoveBaseException& e) {
                    cerr << "Invalid move: " << e.what() << endl;
                    codeResponse = 12;
                    continue;
                } catch (const InvalidSuicideException& e) {
                    cerr << "Suicide move: " << e.what() << endl;
                    codeResponse = 31;
                    continue;
                } catch (const InvalidPromotionException& e) {
                    cerr << "Invalid promotion: " << e.what() << endl;
                    codeResponse = 32;
                    continue;
                }
            } else {
                cout << "\nPlayer 2 (Black - Computer) >> ";
                //a.setIsHumanTurn(false);
                if (!bestMoves.empty()) {
                    Move chosen = bestMoves[0];
                    cout << moveToNotation(chosen) << endl;
                    selfMadeBoard.movePiece(chosen.fromX, chosen.fromY, chosen.toX, chosen.toY);
                } else {
                    cout << "Computer has no legal moves.\n";
                    break;
                }
            }
        }

        a.setCodeResponse(codeResponse);
        a.syncWithBoard(selfMadeBoard);
        a.show();
        isWhiteTurn = selfMadeBoard.getTurn();

        if (model.isCheckmate(selfMadeBoard, isWhiteTurn)) {
            cout << "CHECKMATE! " << (isWhiteTurn ? "White" : "Black") << " loses.\n";
            break;
        }
        if (model.isStalemate(selfMadeBoard, isWhiteTurn)) {
            cout << "DRAW! Stalemate – no legal moves and not in check.\n";
            break;
        }
        if (model.isInsufficientMaterial(selfMadeBoard)) {
            cout << "DRAW! Insufficient material to checkmate.\n";
            break;
        }
    }

    cout << "\nExiting game.\n";
    return 0;
}
