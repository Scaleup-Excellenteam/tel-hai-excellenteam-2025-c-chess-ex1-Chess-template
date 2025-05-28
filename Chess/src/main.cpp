#include <thread>
#include <mutex>
#include <vector>
#include <future>
#include <iostream>
#include <chrono>
#include "Chess.h"
#include "ThreadPool.h"
#include "PriorityQueue.h"

using namespace std;

int main() {
    string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    Chess game(board);
    int res;

    int depth, numThreads, mode;
    cout << "Enter depth: ";
    cin >> depth;
    game.setDepth(depth);

    cout << "Enter number of threads: ";
    cin >> numThreads;

    cout << "Choose mode (1 = manual, 2 = automatic): ";
    cin >> mode;

    game.setDepth(depth);

    ThreadPool pool(numThreads);
    mutex pq_mutex;

    int moveCount = 0;
    const int maxAutoMoves = 8;

    while (mode == 1 || mode == 2 || game.getInput() != "exit") {
        bool turn = game.getTurn();
        auto pieces = game.getPiecesOfCurrentTurn();

        PriorityQueue<string> globalPQ;
        vector<future<void>> futures;

        int totalPieces = pieces.size();
        int piecesPerThread = totalPieces / numThreads;
        int remainder = totalPieces % numThreads;
        int start = 0;

        auto startTime = chrono::high_resolution_clock::now();

        for (int i = 0; i < numThreads; ++i) {
            int count = piecesPerThread + (i < remainder ? 1 : 0);
            vector<string> subset(pieces.begin() + start, pieces.begin() + start + count);
            start += count;

            futures.emplace_back(pool.enqueue([&, subset]() {
                for (const auto& pos : subset) {
                    string move = game.getBestMoveForPiece(pos);
                    if (!move.empty()) {
                        lock_guard<mutex> lock(pq_mutex);
                        globalPQ.push(move);
                    }
                }
            }));
        }

        for (auto& f : futures)
            f.get();

        auto bestMove = globalPQ.pull();
        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();


        if (mode == 2) {
            // game.displayBoard();
            cout << "Best move suggested: " << bestMove << " (computed in " << duration << " ms)" << endl;
            cout << "Auto-playing move: " << bestMove << endl;
            game.playMove(bestMove);
            moveCount++;
            if (moveCount >= maxAutoMoves) break;
        } else {
            while(true) {
                game.displayBoard();
                cout << "Best move suggested: " << bestMove << " (computed in " << duration << " ms)" << endl;
                res = game.playMove(game.getInput());
                if(res)
                    break;
            }
        }
    }

    cout << "Game ended." << endl;
    return 0;
}