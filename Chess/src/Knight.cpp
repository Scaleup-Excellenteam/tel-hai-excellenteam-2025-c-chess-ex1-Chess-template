#include "Knight.h"
#include "Move.h"

/**
 * @brief Constructor for the Knight class.
 * Initializes a Knight piece with a specified color and location on the chessboard.
 * The Knight is a unique piece that moves in an "L" shape: two squares in one direction
 * and then one square perpendicular to that direction (or vice versa).
 * @param color The color of the Knight (either WHITE or BLACK).
 * @param location The position of the Knight on the board, represented by a Coordinate (row, column).
 */
Knight::Knight(COLOR color, const Coordinate& location) : Piece(KNIGHT, color, location) {}

/**
 * @brief Calculates and returns the possible moves for the Knight piece on the current board.
 * The Knight moves in an "L" shape: two squares in one direction (horizontal or vertical)
 * and then one square perpendicular to that direction (or vice versa). This method checks all
 * valid Knight moves in all four possible directions (front, back, right, left) and collects them
 * in a result vector.
 * @param board The current state of the board, represented by a collection of pieces.
 * @return A vector of Coordinates representing the valid moves for the Knight. If there are no valid moves,
 *         an empty vector will be returned.
 */
std::vector<Coordinate> Knight::getRawMoves(const Pieces &board) {
    std::vector<Coordinate> result;

    // Check for Knight moves in the front direction.
    std::vector<Coordinate> tmp = frontKnight(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    // Check for Knight moves in the back direction.
    tmp = backKnight(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    // Check for Knight moves in the right direction.
    tmp = rightKnight(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    // Check for Knight moves in the left direction.
    tmp = leftKnight(board, *this);
    result.insert(result.end(), tmp.begin(), tmp.end());

    return result;  // Returns the list of all valid moves for the Knight.
}

/**
 * @brief Creates a clone of the current Knight piece.
 * This method returns a new instance of the Knight piece, which is a deep copy of the current Knight,
 * with the same color and location. The clone method is used to create a new object that behaves
 * exactly like the current one but is a separate instance.
 * @return A shared pointer to a newly created Knight piece, identical to the current Knight.
 */
std::shared_ptr<Piece> Knight::clone() const {
    return std::make_shared<Knight>(*this);  // Creates a new shared pointer to a cloned Knight.
}
