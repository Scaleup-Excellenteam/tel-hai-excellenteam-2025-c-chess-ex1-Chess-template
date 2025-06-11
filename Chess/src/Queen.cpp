#include "Queen.h"
#include "Board.h"
#include "Rook.h"
#include "Bishop.h"

Queen::Queen(bool is_white) : Piece((is_white ? "Q" : "q"), is_white) {}

bool Queen::isValidMove(int from_x, int from_y, int to_x, int to_y, const Board& board) const {
    // Prevent capturing own piece
    Piece* dest = board.getPieceAt(to_x, to_y);
    if (dest && dest->isWhite() == this->isWhite()) {
        return false;
    }

    // Delegate movement logic to Rook and Bishop
    Rook tempRook(is_white);
    Bishop tempBishop(is_white);

    if (tempRook.isValidMove(from_x, from_y, to_x, to_y, board) ||
        tempBishop.isValidMove(from_x, from_y, to_x, to_y, board)) {
        return true;
    }

    return false;
}
