#include "Pawn.h"

bool Pawn::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    int direction = isWhite ? -1 : 1;
    int startRow = isWhite ? 6 : 1;

    if (destCol == srcCol && !board.getPiece(destRow, destCol)) {
        if (destRow == srcRow + direction) return true;
        if (srcRow == startRow && destRow == srcRow + 2 * direction && !board.getPiece(srcRow + direction, destCol)) return true;
    }
    if (abs(destCol - srcCol) == 1 && destRow == srcRow + direction) {
        if (auto target = board.getPiece(destRow, destCol)) {
            return target->isWhitePiece() != isWhite;
        }
    }

    return false;
}
