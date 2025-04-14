#include "Pawn.h"
#include "Move.h"
#include "Pieces.h"

/**
 * @brief Constructs a Pawn object.
 * Initializes a Pawn with the specified color and location on the chessboard.
 * @param color The color of the Pawn (either WHITE or BLACK).
 * @param location The initial location of the Pawn on the board.
 */
Pawn::Pawn(COLOR color,const Coordinate& location): Piece(PAWN , color , location) {}
/**
 * @brief Returns all valid moves for the Pawn.
 * Calculates the Pawn's potential moves based on its current position and whether it has moved before. The function
 * checks for possible forward moves and diagonal captures.
 * @param board The current chessboard state.
 * @return A vector of coordinates representing the valid moves for the Pawn.
 */
std::vector<Coordinate> Pawn::getRawMoves(const Pieces &board) {
    std::vector<Coordinate> result;

    // Check if the Pawn hasn't moved yet
    if (!isHasMoved()) {
        // Check for the first and second square in front of the Pawn
        if (!frontFreeBoxes(board, *this).empty() && !board.isOccupiedBox(frontFreeBoxes(board, *this).at(0))) {
            result.push_back(frontFreeBoxes(board, *this).at(0));
        }
        if (frontFreeBoxes(board, *this).size() > 1 && !board.isOccupiedBox(frontFreeBoxes(board, *this).at(1))) {
            result.push_back(frontFreeBoxes(board, *this).at(1));
        }
    } else {
        // If Pawn has moved, check only the first square in front
        if (!frontFreeBoxes(board, *this).empty() && !board.isOccupiedBox(frontFreeBoxes(board, *this).at(0))) {
            result.push_back(frontFreeBoxes(board, *this).at(0));
        }
    }

    // Check diagonal captures (right and left)
    if (!diagFrontRight(board, *this).empty()) {
        Coordinate rightDiag = diagFrontRight(board, *this).at(0);
        if (board.isOccupiedBox(rightDiag) && board.getPiece(rightDiag)->getColor() != getColor()) {
            result.push_back(rightDiag);
        }
    }
    if (!diagFrontLeft(board, *this).empty()) {
        Coordinate leftDiag = diagFrontLeft(board, *this).at(0);
        if (board.isOccupiedBox(leftDiag) && board.getPiece(leftDiag)->getColor() != getColor()) {
            result.push_back(leftDiag);
        }
    }

    return result;
}
/**
 * @brief Creates a clone of the Pawn.
 * Returns a new shared pointer to a copy of the current Pawn.
 * @return A shared pointer to the cloned Pawn.
 */
std::shared_ptr<Piece> Pawn::clone() const {
    return std::make_shared<Pawn>(*this);
}

