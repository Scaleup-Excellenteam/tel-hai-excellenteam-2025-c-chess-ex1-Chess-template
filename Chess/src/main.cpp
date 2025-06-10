#include "Chess.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>   // Required for file I/O
#include <ctime>     // Required for timestamp
#include <iomanip>   // Required for formatting time
#include "GameManager.h"
#include "AI/BestMoveFinder.h"

// --- Benchmarking Function ---
// Simulates a short game and appends the performance results to README.md.
void runBenchmarking(int searchDepth) {
    // --- File Output Setup ---
    // The executable is typically run from the 'build' directory, so we navigate
    // up one level to find README.md.
    std::ofstream readme_file("../README.md", std::ios::app);
    if (!readme_file.is_open()) {
        std::cerr << "Warning: Could not open ../README.md. Trying current directory." << std::endl;
        // Fallback for cases where the executable is run from the project root.
        readme_file.open("README.md", std::ios::app);
    }

    // --- Timestamp for the report ---
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf{};
#ifdef _WIN32
    localtime_s(&buf, &in_time_t);
#else
    localtime_r(&in_time_t, &buf); // POSIX-compliant
#endif

    // --- Prepare the output header ---
    std::stringstream header_stream;
    header_stream << "\n\n## Benchmark Results (" << std::put_time(&buf, "%Y-%m-%d %H:%M:%S") << ")\n\n";
    header_stream << "AI Search Depth: **" << searchDepth << "**\n\n";
    header_stream << "| Threads | Total Time (ms) | Average Time per Move (ms) |\n";
    header_stream << "|:-------:|:---------------:|:--------------------------:|\n";

    // Write header to console and, if possible, to the file
    std::cout << "\n--- Starting Benchmark ---\n";
    std::cout << header_stream.str();
    if (readme_file.is_open()) {
        readme_file << header_stream.str();
    }

    // A consistent, legal set of moves for benchmarking
    std::vector<std::string> gameMoves = {
        "g2g4", "b7b5", "h2f3", "b8c6", "h5d7", "a7a6", "d7c6", "b7xc6"
    };
    std::vector<int> threadCounts = {1, 2, 4, 8};

    for (int threads : threadCounts) {
        GameManager gm;
        long long total_duration_ms = 0;
        
        for (const auto& moveStr : gameMoves) {
            gm.makeMove(moveStr);
            auto start_time = std::chrono::high_resolution_clock::now();
            AI::findBestMoves(gm.currentBoard(), gm.whiteToMove(), 3, threads);
            auto end_time = std::chrono::high_resolution_clock::now();
            total_duration_ms += std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        }

        long long average_duration = total_duration_ms / gameMoves.size();
        
        // Prepare the results row
        std::stringstream result_stream;
        result_stream << "| " << std::setw(7) << threads << " | "
                      << std::setw(15) << total_duration_ms << " | "
                      << std::setw(26) << average_duration << " |\n";

        // Write results to console and file
        std::cout << result_stream.str();
        if (readme_file.is_open()) {
            readme_file << result_stream.str();
        }
    }

    std::cout << "\nBenchmark finished. Results appended to README.md.\n";
    if (readme_file.is_open()) {
        readme_file << "\n---\n";
        readme_file.close();
    }
}

// --- Main Function ---
int main()
{
    int searchDepth;
    int gameMode;

    std::cout << "Enter AI search depth (e.g., 3 or 4): ";
    std::cin >> searchDepth;
    if (std::cin.fail()) {
        std::cout << "Invalid input. Exiting.\n";
        return 1;
    }

    std::cout << "Select game mode:\n";
    std::cout << "1. Interactive Game (Player vs. Player)\n";
    std::cout << "2. Automatic Benchmark\n";
    std::cout << "Enter mode (1 or 2): ";
    std::cin >> gameMode;
    if (std::cin.fail() || (gameMode != 1 && gameMode != 2)) {
        std::cout << "Invalid mode. Exiting.\n";
        return 1;
    }

    if (gameMode == 2) {
        runBenchmarking(searchDepth);
    } else {
        // --- Interactive Game Loop ---
        string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
        Chess a(board);
        int codeResponse = 0;
        string res = a.getInput();

        while (res != "exit") {
            codeResponse = a.validateMoveViaManager(res);
            a.setCodeResponse(codeResponse);
            res = a.getInput();
        }
        std::cout << std::endl << "Exiting " << std::endl;
    }

    return 0;
}