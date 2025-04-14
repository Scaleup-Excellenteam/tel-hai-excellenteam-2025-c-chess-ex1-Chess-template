#pragma once

#include "Piece.h"
#include <vector>
#include <memory>

/**
 * @file Bishop.h
 * @brief Bishop Class Definition.
 */

/**
 * @class Bishop
 * @brief Represents a bishop chess piece.
 * The Bishop class simulates a bishop in chess, which can move diagonally.
 */
class Bishop : public Piece {
public:
    /**
     * @brief Constructs a Bishop object.
     * Initializes a bishop with a color and position.
     * @param color The color of the bishop.
     * @param coordinate The position of the bishop.
     */
    Bishop(COLOR color, const Coordinate& coordinate);

    /**
     * @brief Returns all possible moves for the bishop.
     * Calculates legal moves on the board.
     * @param pieces All pieces on the board.
     * @return A vector of moves.
     * @override
     */
    std::vector<Coordinate> getRawMoves(const Pieces& pieces) override;

    /**
     * @brief Creates a copy of the bishop.
     * @return A smart pointer to the copy.
     * @override
     */
    [[nodiscard]] std::shared_ptr<Piece> clone() const override;
};
