#include "Bishop.h"
#include "Move.h"

// Constructor for the Bishop class, initializing the piece with the specified color and location.
Bishop::Bishop(COLOR color, const Coordinate& location) : Piece(BISHOP, color, location) {

}

// Calculates the raw possible moves for the bishop.
// The bishop can move diagonally in all four diagonal directions on the board.
std::vector<Coordinate> Bishop::getRawMoves(const Pieces &board) {
    std::vector<Coordinate> result;

    // Collects diagonal moves in the front-right direction.
    std::vector<Coordinate> temp = diagFrontRight(board, *this);
    result.insert(result.end(), temp.begin(), temp.end());

    // Collects diagonal moves in the front-left direction.
    temp = diagFrontLeft(board, *this);
    result.insert(result.end(), temp.begin(), temp.end());

    // Collects diagonal moves in the back-right direction.
    temp = diagBackRight(board, *this);
    result.insert(result.end(), temp.begin(), temp.end());

    // Collects diagonal moves in the back-left direction.
    temp = diagBackLeft(board, *this);
    result.insert(result.end(), temp.begin(), temp.end());

    return result;
}

// Creates a clone of the current Bishop piece.
std::shared_ptr<Piece> Bishop::clone() const {
    return std::make_shared<Bishop>(*this);
}
