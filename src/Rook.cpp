#include "Rook.h"

bool Rook::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    if (srcRow != destRow && srcCol != destCol) return false;

    int stepRow = (destRow - srcRow) ? (destRow - srcRow > 0 ? 1 : -1) : 0;
    int stepCol = (destCol - srcCol) ? (destCol - srcCol > 0 ? 1 : -1) : 0;
    int r = srcRow + stepRow, c = srcCol + stepCol;

    while (r != destRow || c != destCol) {
        if (board.getPiece(r, c)) return false;
        r += stepRow;
        c += stepCol;
    }
    return true;
}
