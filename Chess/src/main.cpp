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
    int winner = -1;
    int count = 0;
    int numThreads = 8;

    int depth, mode;
    cout << "Enter depth: ";
    cin >> depth;
    game.setDepth(depth);

    cout << "Choose mode (1 = 1 player (vs computer), 2 = 2 players): ";
    cin >> mode;

    game.setDepth(depth);

    ThreadPool pool(numThreads);
    mutex pq_mutex;


    while (mode == 1 || mode == 2 || game.getInput() != "exit") {
        bool turn = game.getTurn();
        auto pieces = game.getPiecesOfCurrentTurn();

        PriorityQueue<string> globalPQ;
        vector<future<void>> futures;
        string move;

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
                    move = game.getBestMoveForPiece(pos);
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

        game.displayBoard();

        if (mode == 1) {
            while(true) {
                if (!(count%2)) { //human player
                    cout << "Best move suggested: " << move << endl;
                    res = game.playMove(game.getInput());
                }
                else { //computer
                    cout << "Auto-playing move: " << move << endl;
                    res = game.playMove(move);
                }
                if(res)
                    break;
            }
        }
        else {
            while(true) {
                cout << "Best move suggested: " << bestMove << endl;
                res = game.playMove(game.getInput());
                if(res)
                    break;
            }
        }

        if (res == 2) {
            break;
        }
        count ++;
    }
    if (count%2) {
        cout << "Game ended. Black wins" << endl;
    }
    else {
        cout << "Game ended. White wins" << endl;
    }
    return 0;
}