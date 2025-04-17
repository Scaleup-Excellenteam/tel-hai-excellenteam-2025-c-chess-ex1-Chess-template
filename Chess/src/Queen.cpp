#include "Queen.h"
#include "Board.h"
#include <cmath>

// Queen moves like a rook or a bishop
bool Queen::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    int rowDiff = std::abs(destRow - srcRow);
    int colDiff = std::abs(destCol - srcCol);

    // Rook-style move (straight line)
    if (srcRow == destRow || srcCol == destCol) {
        return board.isPathClear(srcRow, srcCol, destRow, destCol);
    }

    // Bishop-style move (diagonal)
    if (rowDiff == colDiff) {
        int rowStep = (destRow > srcRow) ? 1 : -1;
        int colStep = (destCol > srcCol) ? 1 : -1;

        int r = srcRow + rowStep;
        int c = srcCol + colStep;

        while (r != destRow && c != destCol) {
            if (board.getPiece(r, c)) {
                return false;
            }
            r += rowStep;
            c += colStep;
        }

        return true;
    }

    return false;
}
