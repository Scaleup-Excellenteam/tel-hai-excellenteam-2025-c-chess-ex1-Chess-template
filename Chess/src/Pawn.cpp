#include "Pawn.h"
#include "Board.h"

Pawn::Pawn(bool is_white): Piece((is_white ? "P" : "p"), is_white){}

bool Pawn::isValidMove(int from_x , int from_y ,int to_x , int to_y ,const Board &board)const {
    Piece* targetPiece = board.getPieceAt(to_x, to_y);
    int direction = is_white ? 1 : -1;
    int startRow = is_white ? 1 : 6;

    // Move forward by one square
    if (to_x == from_x + direction && to_y == from_y && targetPiece == nullptr) {
        return true;
    }

    // Move forward by two squares from the starting position
    if (to_x == from_x + 2 * direction && to_y == from_y && from_x == startRow && targetPiece == nullptr) {
        return true;
    }

    // Capture diagonally
    if (to_x == from_x + direction && (to_y == from_y + 1 || to_y == from_y - 1) && targetPiece != nullptr && targetPiece->isWhite() != is_white) {
        return true;
    }

    return false;
}
