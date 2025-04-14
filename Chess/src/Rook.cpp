#include "Rook.h"
#include "Move.h"
/**
 * @brief Constructor for the Rook piece.
 * Initializes the Rook with a specific color and location on the board.
 * @param color The color of the Rook (either WHITE or BLACK).
 * @param location The location on the board where the Rook is placed.
 */
Rook::Rook(COLOR color,const Coordinate& location) : Piece(ROOK, color, location) {}
/**
 * @brief Gets all raw moves for the Rook piece on the board.
 * The Rook can move horizontally and vertically. This function gathers all
 * possible raw moves for the Rook (without considering any blockages).
 * @param board The current state of the board with all the pieces.
 * @return A vector of coordinates representing the raw moves for the Rook.
 */
std::vector<Coordinate> Rook::getRawMoves(const Pieces &board) {
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

    return result;
}
/**
 * @brief Creates a clone of the Rook piece.
 * This function creates a new instance of the Rook piece with the same attributes.
 * @return A shared pointer to a new Rook object.
 */
std::shared_ptr<Piece> Rook::clone() const {
    return std::make_shared<Rook>(*this);
}
