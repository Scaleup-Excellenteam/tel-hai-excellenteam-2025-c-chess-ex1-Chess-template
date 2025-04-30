#include "Queen.h"
#include "Board.h"
#include "Rook.h"
#include "Bishop.h"

Queen::Queen(bool is_white): Piece((is_white ? "Q" : "q"), is_white){}

bool Queen::isValidMove(int from_x , int from_y ,int to_x , int to_y ,const Board &board)const {
//    if (from_x == to_x || from_y == to_y) {
//        // Rook-like movement
//        int xIncrement = (to_x != from_x) ? (to_x - from_x) / abs(to_x - from_x) : 0;
//        int yIncrement = (to_y != from_y) ? (to_y - from_y) / abs(to_y - from_y) : 0;
//
//        for (int i = from_x + xIncrement, j = from_y + yIncrement; i != to_x || j != to_y; i += xIncrement, j += yIncrement) {
//            if (board.getPieceAt(i, j) != nullptr) {
//                return false;
//            }
//        }
//
//        return true;
//    }
//
//    if (abs(from_x - to_x) == abs(from_y - to_y)) {
//        // Bishop-like movement
//        int xIncrement = (to_x - from_x) / abs(to_x - from_x);
//        int yIncrement = (to_y - from_y) / abs(to_y - from_y);
//
//        for (int i = 1; i < abs(to_x - from_x); ++i) {
//            if (board.getPieceAt(from_x + xIncrement * i, from_y + yIncrement * i) != nullptr) {
//                return false;
//            }
//        }
//
//        return true;
//    }
//
//    return false;
    Rook tempRook(is_white); // Create a temporary rook with the same color
    Bishop tempBishop(is_white); // Create a temporary bishop with the same color

    // If the move is valid for either a rook or a bishop, it's valid for a queen
    if (tempRook.isValidMove(from_x, from_y, to_x, to_y, board) || tempBishop.isValidMove(from_x, from_y, to_x, to_y, board)) {
        return true;
    }

    return false;
}
