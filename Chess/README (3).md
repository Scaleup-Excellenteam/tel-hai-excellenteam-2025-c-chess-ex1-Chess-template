Overview

This project implements a multithreaded benchmark for evaluating chess moves. The main goal is to simulate chess moves with increasing depth using multiple threads and measure performance. The user can configure depth, thread count, mode of play (manual or automatic), and move selection strategy.

Core Algorithm – suggestMovesDepth

The Model::suggestMovesDepth function is the heart of the evaluation system. It recursively computes the best moves up to a given depth by simulating future moves and counter-moves.

Key Steps:

Move Generation: For each player's piece, valid moves are generated.

Move Evaluation: Each move is scored based on immediate gain and the opponent's best counter-response, recursively.

Depth Handling: If depth > 1, simulate the board, apply the move, and recursively call suggestMovesDepth to calculate the opponent's response.

Early Termination: If a move exceeds a threshold score, all other threads are notified via stopFlag to halt computation.

Threading Model

Each piece is assigned to a separate thread pool task.

All possible moves for that piece are evaluated in its thread.

Threads access a shared PriorityQueue guarded by a mutex for inserting results.

Synchronization

std::mutex: Used for thread-safe access to the shared move queue.

std::atomic<bool> stopFlag: Used to signal threads to stop evaluating once a high-value move is found.

std::condition_variable: Ensures the main thread waits for all evaluations (or early termination).

Benchmarking Performance

Performance is measured using std::chrono::high_resolution_clock. The time from the beginning of the benchmark to the end (or early exit) is recorded.

Sample Input

Enter depth:4
 4
Enter mode: (0) Manual play, (1) Auto-play benchmark:1
 1
Enter number of threads (0, 2, 4, or 8):2
 2
Auto-play mode: choose move selection (0 = best move, 1 = random):1

Early Exit Mechanism

If a move with a score greater than or equal to thresholdScore is found, stopFlag is set. Other threads check this flag and exit early, saving compute time.

Files of Interest

Model::suggestMovesDepth – Multithreaded move suggestion algorithm.

autoPlayBenchmark – Function to run and display moves with timing.

Chess, Board – Maintain board state and game mechanics.




