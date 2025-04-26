#include "Bishop.h"

bool Bishop::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    if (abs(destRow - srcRow) != abs(destCol - srcCol)) return false;

    int rowStep = (destRow > srcRow) ? 1 : -1;
    int colStep = (destCol > srcCol) ? 1 : -1;
    int r = srcRow + rowStep, c = srcCol + colStep;

    while (r != destRow && c != destCol) {
        if (board.getPiece(r, c)) return false;
        r += rowStep;
        c += colStep;
    }

    return true;
}
