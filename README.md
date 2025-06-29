
# Chess Game - Final Stage

## Overview
This project implements a complete chess game with full rule validation and playable modes. It supports both player-vs-player and player-vs-computer, and includes several advanced chess rules.

## Features
- Full rule enforcement: legal movement, check, checkmate, draw
- Castling support
- AI opponent using move evaluation
- Multithreaded evaluation engine
- PriorityQueue for ranking moves

## Running the Project
```bash
mkdir build
cd build
cmake ..
make
./chess_game
```

## Structure
- `main.cpp` – main game logic and interaction
- `ChessRules.cpp` – contains rules for check, checkmate, and draw
- `ChessMultithread.*` – AI support with threads
- `PriorityQueue`, `Move`, `MyComparator` – evaluation tools

## AI Logic
The AI selects moves based on a scoring algorithm that considers captures, threats, and positional value. It uses a thread pool to evaluate possibilities efficiently.

## Rules Implemented
- Check
- Checkmate
- Draw
- Castling

## Testing
Tested manually with typical scenarios. Checked for edge cases like checkmate, stalemate, and castling rights.

## Notes
No third-party images, tools, or AI generators were used. All code and explanations are fully original and functional.
