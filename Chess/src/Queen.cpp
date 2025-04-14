#include "Queen.h"
#include "Move.h"

/**
 * @brief Constructor for the Queen piece.
 * Initializes the Queen with a specific color and location on the board.
 * @param color The color of the Queen (either WHITE or BLACK).
 * @param location The location on the board where the Queen is placed.
 */
Queen::Queen(COLOR color,const Coordinate& location) : Piece(QUEEN, color, location) {}
/**
 * @brief Gets all raw moves for the Queen piece on the board.
 * The Queen can move horizontally, vertically, and diagonally. This function
 * gathers all possible raw moves for the Queen (without considering any blockages).
 * @param board The current state of the board with all the pieces.
 * @return A vector of coordinates representing the raw moves for the Queen.
 */
std::vector<Coordinate> Queen::getRawMoves(const Pieces &board) {
    std::vector<Coordinate> result;
    // Get all free spaces in the possible directions
    std::vector<Coordinate> tmp = frontFreeBoxes(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    tmp = backFreeBoxes(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    tmp = rightFreeBoxes(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    tmp = leftFreeBoxes(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    tmp = diagFrontRight(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    tmp = diagFrontLeft(board, *this);
    result.insert(result.end(), tmp.begin(),tmp.end());

    tmp = diagBackRight(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    tmp = diagBackLeft(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    return result;
}
/**
 * @brief Creates a clone of the Queen piece.
 * This function creates a new instance of the Queen piece with the same attributes.
 * @return A shared pointer to a new Queen object.
 */
std::shared_ptr<Piece> Queen::clone() const {
    return std::make_shared<Queen>(*this);
}
