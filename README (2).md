# Chess C++ Project

## 🧩 Project Overview
A C++ implementation of a command-line chess game with support for:
- Human vs Human mode
- Human vs Computer (AI) mode
- Computer vs Computer (benchmark mode)

## 🚀 How to Build & Run
Chess/
├── cmake-build-debug/        
├── include/                   
│   ├── Bishop.h, Board.h, Piece.h, ...
│   ├── Model.h, ThreadPool.h,...
│   └── CMakeLists.txt         
├── src/                       
│   ├── Bishop.cpp, Board.cpp, ...
│   ├── main.cpp              
│   └── CMakeLists.txt         
└── README.md                  


### Prerequisites
- A C++17 compatible compiler
- CMake (version >= 3.10)

### Build Instructions
cd ~/chess-ex1-halabat
mkdir build
cd build
cmake ..
make
./ChessGame


### Run
C:\WINDOWS\system32\wsl.exe --distribution Ubuntu --exec /bin/bash -c \
"cd /home/hala/chess-ex1-halabat/cmake-build-debug && ./ChessGame"

### 🕹️ Running example
Enter depth: 2
Choose game mode:
0 = Human vs Human
1 = Human vs Computer
2 = Computer vs Computer (Auto-play benchmark)
1



## 🧠 Features Implemented

### Game Modes
- ✅ Human vs Human
- ✅ Human vs Computer
- ✅ Computer vs Computer (benchmark mode)

### Rules Implemented
- ✅ Basic chess movement and rules
- ✅ Check and checkmate detection
- ✅ Stalemate and insufficient material detection

### Bonus
- ✅ AI opponent using Minimax-inspired move suggestion
- ✅ Multithreading support via thread pool
- ✅ Configurable depth and heuristic evaluation

## 🤖 AI Algorithm Description
The computer opponent uses a heuristic scoring system to evaluate potential moves to a given depth:
- Top moves are chosen using priority queue
- Optionally, random selection from top scoring moves (benchmark mode)
- Thread pool used to evaluate piece moves in parallel

## ✅ Testing Strategy
- Manual testing with known board setups
- Debug printouts for move suggestions and scores
- Verifications for check, checkmate, stalemate cases
- Testing edge cases: suicide moves, promotions, invalid moves

## 🧪 Known Challenges
- Synchronizing board and UI output during human-computer interaction
- Handling invalid input robustly without crashing
- Ensuring AI doesn't hang or crash on edge cases

## 🧗‍♀️ What was the biggest challenge I encountered at this stage?
The main challenge was managing the turn between player and computer correctly, while maintaining an accurate display, correct input/output flow, and avoiding double-printing or confusion between players.
Since the logic of the game (the board and rules) was separated from the display (the Chess class), it was necessary to coordinate the internal state of the board with what was displayed to the user, especially when one of the parties is a computer.

🌟 Improvements I would add if I had more time
Full implementation of the scream (Castling) with all its conditions.

Undo/Redo functionality for a human player.

Saving and loading states (Save/Load).

Game timer – time management for each player (like a chess clock).

Choosing a difficulty level for the computer algorithm.

Graphical interface (for example, using SFML or Qt).

Improvements to the algorithm – moving to Minimax with Alpha-Beta pruning.

Draw detection according to 3 repetitions/50-move rule (according to FIDE rules).

## 🙌 Authors
- Developed by Hala Batheesh .