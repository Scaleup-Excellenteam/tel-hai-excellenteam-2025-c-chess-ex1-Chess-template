# THE Chess Repository

## Compilation Instructions
To compile your project manually, use the following commands:

```sh
mkdir -p build
cd build
cmake ..
make
./Chess 
```

### Project Tree

Project tree should match the following structure:

```bash
.
├── CONTRIBUTING.md
├── .git
│   ├── ...
├── .github
│   └── workflows
│       └── c-cpp.yml
├── .gitignore
├── img
│   ├── excellenteam.png
│   ├── scaleup.png
│   └── cpp_logo.png
├── README.md
├── Chess
|    ├── CMakeLists.txt
|    ├── src
|    │   ├── main.cpp
|    │   ├── Chess.cpp
|    │   ├── ChessAI.cpp
|    │   ├── PriorityQueue.cpp
|    │   ├── ThreadPool.cpp
|    │   ├── Piece.cpp
|    │   ├── Bishop.cpp
|    │   ├── King.cpp
|    │   ├── Knight.cpp
|    │   ├── Pawn.cpp
|    │   ├── Queen.cpp
|    │   ├── Rook.cpp
|    ├── include
|    │   ├── Chess.h
|    │   ├── ChessAI.h
|    │   ├── PriorityQueue.h
|    │   ├── ThreadPool.h
|    │   ├── Piece.h
|    │   ├── Bishop.h
|    │   ├── King.h
|    │   ├── Knight.h
|    │   ├── Pawn.h
|    │   ├── Queen.h
|    │   ├── Rook.h
```

## Features
 ### Castling Support
    Castling has been fully implemented according to official chess rules.
    The logic is handled both in the **King** class and within the **Chess** class's move   validation flow.

#### Castling Conditions:
    - Neither the king nor the involved rook has moved.
    - All squares between the king and the rook are empty.
    - The king is not in check.
    - The king does not pass through or land on a square that is under attack.

 ### Checkmate Detection
isCheck(bool whiteTurn, Piece* board[8][8]) :

    - Scans the board for the current player's king.
    - Checks if any opposing piece can legally move to the king's position.

isCheckmate(bool whiteTurn, Piece* board[8][8]) :
    - First verifies the king is currently in check.
    - Then simulates every possible legal move by the player's pieces.
    - If no legal move removes the check, the game is declared checkmate.


## Player vs Computer Mode
#### The game supports two modes:
- **1** — Player vs Computer
- **2** — Two Player (local)
When the user selects mode 1, the game alternates between the human player and the computer. The computer's move is calculated using a thread pool to parallelize the evaluation of possible moves for each of its pieces.

### The Computer Mode:
#### Best Move Selection Algorithm
This project includes a simple move evaluation algorithm that suggests the best move in a given turn. The idea is to evaluate all legal moves for the current player, score them, and return the top 5.

#### How It Works:
**1. Legal Moves**
\
For each piece of the current player, we generate all legal moves.

**2. Scoring**
\
Each move is scored based on:

- Capturing an opponent's piece (higher score for more valuable pieces).

- Threatening a stronger enemy piece.
- Being threatened by a weaker piece.
- Delivering check.

**3. Lookahead**
\
If a depth > 0 is provided, the algorithm simulates the opponent's best response (and optionally our next move), using a naive minimax approach:
- For depth 1: we subtract the opponent's best move score.
- For depth 2: we add back our best reply, and so on.

**4. Top Moves**
\
All moves are stored in a custom PriorityQueue, and the top 3 are printed before the player moves.

#### Complexity
- At depth 0: roughly O(N×M), where N is number of pieces, and M is average moves per piece.
- At depth D: time complexity grows exponentially like O((N×M)^D), similar to basic minimax.


#### Implementation Highlights:
- For each of the computer's pieces, the best legal move is evaluated and inserted into a global priority queue.
- The move with the highest priority (best score) is selected and executed by the AI.
- The game displays the suggested move for the human player before each turn.


## Testing
To validate the correctness of the implementation, the following manual testing strategies were applied:

- **Computer Player Logic:**

        I tested the AI by playing multiple full games in Player vs Computer mode.
        The computer responded as expected according to its move evaluation algorithm,
        consistently selecting legal and reasonable moves based on the position.

- **New Features (Check, Checkmate, Castling):**

        All added features — including check, checkmate, and castling — were tested by 
        simulating specific board states where these situations should occur.
        I verified the behavior by observing printed messages and confirming that the
        program followed chess rules accurately.

