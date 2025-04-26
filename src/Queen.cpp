#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"

bool Queen::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    Rook rook(isWhite);
    Bishop bishop(isWhite);
    return rook.isValidMove(srcRow, srcCol, destRow, destCol, board) ||
           bishop.isValidMove(srcRow, srcCol, destRow, destCol, board);
}
