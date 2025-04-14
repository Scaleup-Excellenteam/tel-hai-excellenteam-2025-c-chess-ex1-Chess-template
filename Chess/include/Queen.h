#pragma once
#include "Piece.h"

// Class representing a Queen piece in the chess game, derived from Piece.
class Queen : public Piece {
public:
    /**
     * @brief Constructs a Queen piece with the specified color and location.
     * @param color The color of the queen (e.g., white or black).
     * @param location The initial location of the queen on the board.
     */
    Queen(COLOR color, const Coordinate& location);

    /**
     * @brief Calculates the raw possible moves for the queen.
     * The queen can move in straight lines horizontally, vertically, and diagonally.
     * @param board The current state of the board.
     * @return A vector of coordinates representing the raw possible moves for the queen.
     */
    std::vector<Coordinate> getRawMoves(const Pieces &board) override;

    /**
     * @brief Creates a clone of the current queen.
     * @return A shared pointer to a new Queen object, identical to the current one.
     */
    [[nodiscard]] std::shared_ptr<Piece> clone() const override;
};
