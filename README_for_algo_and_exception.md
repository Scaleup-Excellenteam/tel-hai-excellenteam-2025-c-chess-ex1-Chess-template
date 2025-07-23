# Chess Game Engine – Part 2 & 3

## Overview

This project implements a console-based chess game engine in C++ that supports movement validation, exception handling, and move recommendations using a scoring-based minimax algorithm. It is designed to integrate with a provided `Chess` class for GUI interaction, and emphasizes clean separation of logic and robust error handling.

---

## Features

* Full movement validation for all standard chess pieces (Pawn, Knight, Bishop, Rook, Queen, King)
* Custom exception system for illegal moves
* Smart move recommendation system using a template-based priority queue and minimax
* Turn-based interaction through a console GUI

---

## Exception Handling

To enhance code structure and clarity, custom exceptions are used for move validation:

### Implemented Exceptions

| Type                         | Trigger Condition                                      |
| ---------------------------- | ------------------------------------------------------ |
| `NoPieceAtSource`            | No piece at the source square                          |
| `OpponentPieceAtSource`      | Trying to move a piece that belongs to the opponent    |
| `FriendlyPieceAtDestination` | Target square has a piece of the same color            |
| `InvalidPieceMovement`       | Movement is not valid for the piece type               |
| `CheckViolation`             | Move would result in check (placeholder for extension) |

These exceptions are defined in `Exception_chess.h` and thrown from `Board::movePiece()`. They are caught and translated into response codes (11–42), which are then interpreted by the GUI (`Chess` class) and presented to the user.

This structure cleanly separates move validation from UI error messaging and ensures modular, maintainable code.

---

## Move Recommendation Algorithm

This chess engine features a move recommendation system that ranks the best possible moves for the current player using a **naive minimax strategy** and a custom `PriorityQueue`.

### Key Components

#### Scoring Function (`scorePosition`)

* Assigns piece values (Pawn=1, Knight/Bishop=3, Rook=5, Queen=9, King=100)
* Adds/subtracts scores based on which player’s turn it is
* Simple heuristic that rewards piece advantage

#### Minimax Search (`Board::minimax`)

* Evaluates the board recursively up to a defined depth (default: 2)
* For each move, it simulates the opponent’s best possible response
* Returns the score difference to estimate move strength
#### Move Ranking (`PriorityQueue`)
* Stores top 5 moves by score using a template-based queue
* Sorted via a custom comparator to keep highest-valued moves at the front
* Implemented using `std::list` (no STL `priority_queue`)
### Output
* The best move are displayed automatically after each legal move using an overloaded `operator<<` on the `Board` class.

---

##  Multithreading (Part 3)

### Algorithm Overview

To improve performance, we parallelized the move recommendation process using `std::thread`. Each thread explores legal moves for a subset of the board and ranks them using a scoring-based minimax algorithm. A custom `PriorityQueue` is used to store and sort the best moves safely across threads.

### Work Distribution

* The board has 64 squares (8×8).
* Threads are distributed evenly using index chunks.
* Each thread checks its assigned squares:

    * If a piece belongs to the current player, it generates all possible moves from that piece.
    * For each valid move, it creates a new board state and evaluates its score using `minimax`.

### Synchronization

* A shared `PriorityQueue` is used to store scored moves.
* To prevent race conditions, all queue operations (`push`, `poll`, etc.) are wrapped with `std::mutex`.
* Each thread uses its own `Board` copy to avoid state conflicts.
* We also implemented a bonus feature:
  A `std::atomic<bool> stopFlag` is used to **stop all threads early** when a move with a score ≥ 90 is found.

### Performance Measurement

We benchmarked the engine by running an automatic 8-move game in `autoPlayBenchmark()` for:

* 0 threads (single-threaded fallback)
* 2 threads
* 4 threads
* 8 threads

Timing was measured using `std::chrono::high_resolution_clock`. The results confirmed that using threads significantly improves performance in deeper minimax depths.
