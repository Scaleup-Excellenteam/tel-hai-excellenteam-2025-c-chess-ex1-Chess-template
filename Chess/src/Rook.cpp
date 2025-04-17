#include "Rook.h"
#include "Board.h"

// Check if the rook is moving in a straight line and the path is clear
bool Rook::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    if (srcRow != destRow && srcCol != destCol) {
        return false; // Rook can only move in straight lines
    }

    if (!board.isPathClear(srcRow, srcCol, destRow, destCol)) {
        return false; // Path is blocked
    }

    return true;
}
