#include <iostream>
#include "Knight.h"
#include "Board.h"

Knight::Knight(bool is_white) : Piece((is_white ? "N" : "n"), is_white) {}

bool Knight::isValidMove(int from_x, int from_y, int to_x, int to_y, const Board& board) const {
    // Prevent capturing your own piece
    Piece* dest = board.getPieceAt(to_x, to_y);
    if (dest && dest->isWhite() == this->isWhite()) {
        return false;
    }

    int dx = abs(from_x - to_x);
    int dy = abs(from_y - to_y);

    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}
