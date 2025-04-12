#include "Rook.h"
#include "Board.h"

Rook::Rook(bool is_white): Piece((is_white ? "R" : "r"), is_white){}


bool Rook::isValidMove(int from_x , int from_y ,int to_x , int to_y ,const Board &board)const {
    if (from_x != to_x && from_y != to_y) {
        return false;
    }

    int xIncrement = (to_x != from_x) ? (to_x - from_x) / abs(to_x - from_x) : 0;
    int yIncrement = (to_y != from_y) ? (to_y - from_y) / abs(to_y - from_y) : 0;

    for (int i = from_x + xIncrement, j = from_y + yIncrement; i != to_x || j != to_y; i += xIncrement, j += yIncrement) {
        if (board.getPieceAt(i, j) != nullptr) {
            return false;
        }
    }

    return true;
}
