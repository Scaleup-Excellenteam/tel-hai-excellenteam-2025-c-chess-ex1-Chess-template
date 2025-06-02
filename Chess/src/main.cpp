// main.cpp
#include <iostream>
#include <thread>
#include <chrono>

#include "Chess.h"
#include "Board.h"
#include "ThreadPool.h"

// Forward‐declare runBenchmarks (defined in bench.cpp):
extern void runBenchmarks();

int main() {
    // 1) Build a single shared 64‐char string
    std::string startBoard =
            "RNBQKBNR"
            "########"
            "########"
            "########"
            "########"
            "########"
            "########"
            "rnbqkbnr";

    // 2) Construct GUI and logic each with their own copy (Option A)
    Chess* gui   = new Chess(startBoard);
    Board  logic = Board(startBoard);

    // 3) Ask how many threads to use
    std::cout << "Enter number of threads to use (0 = none, options: 1, 2, 4, 8): ";
    size_t numThreads;
    std::cin >> numThreads;

    ThreadPool* poolPtr = nullptr;
    if (numThreads > 0) {
        poolPtr = new ThreadPool(numThreads);
    }

    // 4) Ask whether to run benchmarks or play
    std::cout << "Select option: (1) Run Benchmarks, (2) Play (Manual/Automatic)\n";
    int mode;
    std::cin >> mode;
    if (mode == 1) {
        // Now that runBenchmarks is declared above, this compiles:
        runBenchmarks();
        delete gui;
        delete poolPtr;
        return 0;
    }

    // 5) Main game loop: manual vs. automatic
    while (true) {
        // Always show the engine’s internal board first
        std::cout << logic << std::endl;

        std::cout << "Select mode: (1) Manual, (2) Automatic, (3) Exit\n";
        int submode;
        std::cin >> submode;
        if (submode == 3) break;

        if (submode == 2) {
            // Automatic: play 8 half‐moves
            for (int i = 0; i < 8; ++i) {
                auto recs = logic.recommendMoves(2, 1, poolPtr);
                if (recs.empty()) break;

                int code;
                logic.movePiece(recs[0].first, code);

                // Copy logic’s updated 64‐char string into gui
                std::string newBoard = logic.getRawString();
                gui->setBoardString(newBoard);
                gui->setCodeResponse(code);
                gui->setPieces();

                std::cout << logic << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        } else {
            // Manual: prompt and read one move
            std::cout << (logic.isWhiteTurn() ? "White (enter move): " : "Black (enter move): ");
            std::string mv;
            std::cin >> mv;
            if (mv == "exit" || mv == "quit") break;

            int codeResponse;
            logic.movePiece(mv, codeResponse);

            // Copy the updated board string into gui
            std::string newBoard = logic.getRawString();
            gui->setBoardString(newBoard);
            gui->setCodeResponse(codeResponse);
            gui->setPieces();

            std::cout << logic << std::endl;
        }
    }

    delete gui;
    delete poolPtr;
    return 0;
}
