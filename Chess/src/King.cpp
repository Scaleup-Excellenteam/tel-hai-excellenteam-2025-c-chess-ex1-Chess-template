#include "King.h"

/**
 * @brief Constructor for the King class.
 *
 * Initializes a King piece with a specified color and location on the chessboard.
 * The King is a piece of type KING, and it has a specific movement ability:
 * it can move one square in any direction (diagonally, horizontally, or vertically).
 * @param color The color of the King (either WHITE or BLACK).
 * @param location The position of the King on the board, represented by a Coordinate (row, column).
 */
King::King(COLOR color, const Coordinate& location) : Piece(KING, color, location) {}

/**
 * @brief Calculates and returns the possible moves for the King piece on the current board.
 *
 * The King can move one square in any direction (diagonally, horizontally, or vertically).
 * This method checks all the valid directions for the King, which include:
 * - Diagonal moves (front-right, front-left, back-right, back-left).
 * - Horizontal and vertical moves (front, back, left, right).
 * The method returns a list of coordinates representing the valid moves.
 * @param board The current state of the board, represented by a collection of pieces.
 * @return A vector of Coordinates representing the valid moves for the King. If there are no valid moves,
 *         an empty vector will be returned.
 */
std::vector<Coordinate> King::getRawMoves(const Pieces &board) {
    std::vector<Coordinate> result;

    // Check for diagonal moves (front-right, front-left, back-right, back-left).
    std::vector<Coordinate> temp = diagFrontRight(board, *this);
    if (!temp.empty()) {
        result.push_back(temp.at(0));  // Adds the first valid diagonal move (front-right).
    }

    temp = diagFrontLeft(board, *this);
    if (!temp.empty()) {
        result.push_back(temp.at(0));  // Adds the first valid diagonal move (front-left).
    }

    temp = diagBackRight(board, *this);
    if (!temp.empty()) {
        result.push_back(temp.at(0));  // Adds the first valid diagonal move (back-right).
    }

    temp = diagBackLeft(board, *this);
    if (!temp.empty()) {
        result.push_back(temp.at(0));  // Adds the first valid diagonal move (back-left).
    }

    // Check for rook-like moves (front, back, right, left).
    temp = frontFreeBoxes(board, *this);
    if (!temp.empty()) {
        result.push_back(temp.at(0));  // Adds the first valid horizontal move (front).
    }

    temp = backFreeBoxes(board, *this);
    if (!temp.empty()) {
        result.push_back(temp.at(0));  // Adds the first valid horizontal move (back).
    }

    temp = rightFreeBoxes(board, *this);
    if (!temp.empty()) {
        result.push_back(temp.at(0));  // Adds the first valid vertical move (right).
    }

    temp = leftFreeBoxes(board, *this);
    if (!temp.empty()) {
        result.push_back(temp.at(0));  // Adds the first valid vertical move (left).
    }

    return result;  // Returns the list of all valid moves for the King.
}

/**
 * @brief Creates a clone of the current King piece.
 * This method returns a new instance of the King piece, which is a deep copy of the current King,
 * with the same color and location. The clone method is used to create a new object that behaves
 * exactly like the current one but is a separate instance.
 * @return A shared pointer to a newly created King piece, identical to the current King.
 */
std::shared_ptr<Piece> King::clone() const {
    return std::make_shared<King>(*this);  // Creates a new shared pointer to a cloned King.
}
