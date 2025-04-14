#pragma once

#include "Piece.h"
#include "Move.h"

/**
 * @class King
 * @brief Represents the King chess piece and its movement logic.
 * The King can move one square in any direction (horizontal, vertical, or diagonal).
 * This class inherits from the abstract Piece class and implements the required methods
 * for move generation and cloning.
 */
class King : public Piece {
public:
    /**
     * @brief Constructs a King piece.
     * @param color The color of the king (BLACK or WHITE).
     * @param location The initial position of the king on the board.
     */
    King(COLOR color, const Coordinate& location);

    /**
     * @brief Returns all possible raw moves for the King, regardless of check or board state.
     * The King can move one square in any direction, unless blocked.
     * Does not consider check conditions or castling.
     * @param pieces A reference to all current pieces on the board.
     * @return std::vector<Coordinate> A list of target coordinates the King can move to.
     */
    std::vector<Coordinate> getRawMoves(const Pieces&) override;

    /**
     * @brief Creates a copy of the current King piece.
     * @return std::shared_ptr<Piece> A shared pointer to the cloned King.
     */
    [[nodiscard]] std::shared_ptr<Piece> clone() const override;
};
