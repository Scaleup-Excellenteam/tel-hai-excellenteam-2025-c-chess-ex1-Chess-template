#include "Bishop.h"
#include "Board.h"

Bishop::Bishop(bool is_white): Piece((is_white ? "B" : "b"), is_white){}

bool Bishop::isValidMove(int from_x , int from_y ,int to_x , int to_y ,const Board &board)const {
<<<<<<< HEAD
=======
    Piece* dest = board.getPieceAt(to_x, to_y);
    if (dest && dest->isWhite() == this->isWhite()) {
        return false;
    }
>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
    if (abs(from_x - to_x) != abs(from_y - to_y)) {
        return false;
    }

    int xIncrement = (to_x - from_x) / abs(to_x - from_x);
    int yIncrement = (to_y - from_y) / abs(to_y - from_y);

    for (int i = 1; i < abs(from_x - to_x); ++i) {
        if (board.getPieceAt(from_x + xIncrement * i, from_y + yIncrement * i) != nullptr) {
            return false;
        }
    }
    return true;
}
