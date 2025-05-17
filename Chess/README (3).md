# ♟ Chess Move Recommender with Depth-2 Evaluation

This project implements a smart move suggestion system for a command-line chess simulation, supporting strategic thinking up to depth = 2 (i.e., considering the opponent’s best possible response).

---

## Algorithm Overview

###  `Model::suggestMoves` – Depth 1
This function evaluates all valid moves for the current player and assigns a score to each move based on:

- **Capturing an opponent's piece** → adds its value to the score
- **Threatening a stronger piece** → adds bonus points
- **Landing on a square threatened by a weaker piece** → subtracts penalty points

### `Model::suggestMovesDepth2` – Depth 2
This extended function simulates each possible move, then evaluates the **opponent's best counter-move**, and adjusts the final score accordingly:

> `finalScore = myScore - enemyBestScore`

This allows the AI to avoid moves that look good at first glance but leave it vulnerable.

---

## Board Copying for Simulation

To simulate moves without altering the real game state, the project implements:
.
```cpp
Board* Board::createSimulatedCopy() const;
```

This function creates a deep copy of the current board, allowing safe testing of future states.

> Note: The destructor of `Board` must be declared `public` and `virtual` to allow deletion of simulated copies.
Even though I know it's not a good decision




