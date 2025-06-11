#include "King.h"
#include "Board.h"
#include "King.h"
#include "Board.h"

King::King(bool is_white): Piece((is_white ? "K" : "k"), is_white) {}

bool King::isValidMove(int from_x, int from_y, int to_x, int to_y, const Board &board) const {
    // Check if destination has a friendly piece
    Piece* dest = board.getPieceAt(to_x, to_y);
    if (dest && dest->isWhite() == this->isWhite()) {
        return false;
    }

    // King can move one square in any direction
    int dx = abs(to_x - from_x);
    int dy = abs(to_y - from_y);

    return (dx <= 1 && dy <= 1);
}

