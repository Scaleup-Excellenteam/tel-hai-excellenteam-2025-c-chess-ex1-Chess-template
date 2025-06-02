// bench.cpp
#include "Board.h"
#include "ThreadPool.h"
#include <chrono>
#include <iostream>

// Measure elapsed time of any callable in milliseconds.
template<typename Func>
long long timeMillis(Func f) {
    auto t0 = std::chrono::high_resolution_clock::now();
    f();
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
}

void runBenchmarks() {
    std::cout << "=== Starting benchmarks ===\n";

    // Standard initial position (64-char string):
    std::string start ="RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";

    // 0 threads (single‐threaded original code):
    {
        Board logic0(start);
        long long elapsed0 = timeMillis([&] {
            for (int i = 0; i < 8; ++i) {
                auto recs = logic0.recommendMoves(2, 1, /*pool=*/nullptr);
                if (recs.empty()) break;
                int code;
                logic0.movePiece(recs[0].first, code);
            }
        });
        std::cout << "Threads=0 (none) time=" << elapsed0 << "ms\n";
    }

    // 1, 2, 4, 8 threads:
    for (auto tcount : { 1u, 2u, 4u, 8u }) {
        long long elapsedN = timeMillis([&] {
            Board logicN(start);            // fresh board each trial
            ThreadPool poolN(tcount);       // thread pool of size tcount
            for (int i = 0; i < 8; ++i) {
                auto recs = logicN.recommendMoves(2, 1, &poolN);
                if (recs.empty()) break;
                int code;
                logicN.movePiece(recs[0].first, code);
            }
        });
        std::cout << "Threads=" << tcount << " time=" << elapsedN << "ms\n";
    }

    std::cout << "=== Benchmarks done ===\n";
}
