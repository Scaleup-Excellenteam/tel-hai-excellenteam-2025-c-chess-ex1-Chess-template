#include "Bishop.h"
#include "Board.h"
#include <cmath>

// Bishop moves diagonally as long as the path is clear
bool Bishop::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    int rowDiff = std::abs(destRow - srcRow);
    int colDiff = std::abs(destCol - srcCol);

    if (rowDiff != colDiff) {
        return false; // Not a diagonal move
    }

    int rowStep = (destRow > srcRow) ? 1 : -1;
    int colStep = (destCol > srcCol) ? 1 : -1;

    int r = srcRow + rowStep;
    int c = srcCol + colStep;

    while (r != destRow && c != destCol) {
        if (board.getPiece(r, c)) {
            return false; // Path is blocked
        }
        r += rowStep;
        c += colStep;
    }

    return true;
}
