#pragma once

#include "Piece.h"

/**
 * @class Knight
 * @brief Represents the Knight chess piece and its movement logic.
 * The Knight moves in an "L" shape: two squares in one direction and then
 * one square perpendicular to that. It is the only piece that can jump over others.
 */
class Knight : public Piece {
public:
    /**
     * @brief Constructs a Knight piece.
     * @param color The color of the knight (BLACK or WHITE).
     * @param location The initial position of the knight on the board.
     */
    Knight(COLOR color, const Coordinate& location);

    /**
     * @brief Returns all possible raw moves for the Knight, regardless of check conditions.
     * The Knight moves in "L"-shaped patterns and can jump over other pieces.
     * @param pieces A reference to all current pieces on the board.
     * @return std::vector<Coordinate> A list of legal coordinates the Knight can move to.
     */
    std::vector<Coordinate> getRawMoves(const Pieces&) override;

    /**
     * @brief Creates a copy of the current Knight piece.
     * @return std::shared_ptr<Piece> A shared pointer to the cloned Knight.
     */
    [[nodiscard]] std::shared_ptr<Piece> clone() const override;
};
