#pragma once

#include "Piece.h"

// Class representing a Rook piece in the chess game, derived from Piece.
class Rook : public Piece {
public:
    /**
     * @brief Constructs a Rook piece with the specified color and location.
     * @param color The color of the rook (e.g., white or black).
     * @param location The initial location of the rook on the board.
     */
    Rook(COLOR color, const Coordinate& location);

    /**
     * @brief Calculates the raw possible moves for the rook.
     * The rook can move in straight lines horizontally and vertically.
     * @param board The current state of the board.
     * @return A vector of coordinates representing the raw possible moves for the rook.
     */
    std::vector<Coordinate> getRawMoves(const Pieces &board) override;

    /**
     * @brief Creates a clone of the current rook.
     * @return A shared pointer to a new Rook object, identical to the current one.
     */
    [[nodiscard]] std::shared_ptr<Piece> clone() const override;
};
