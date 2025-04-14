#pragma once
#include "Piece.h"

/**
 * @class Pawn
 * @brief Represents the Pawn chess piece and its movement logic.
 * Pawns move forward one square, with the option to move two squares forward from their initial position.
 * They capture diagonally and have special movement rules like promotion and en passant (not handled here).
 */
class Pawn : public Piece {
public:
    /**
     * @brief Constructs a Pawn piece.
     * @param color The color of the pawn (BLACK or WHITE).
     * @param location The initial position of the pawn on the board.
     */
    Pawn(COLOR color, const Coordinate& location);

    /**
     * @brief Returns all possible raw moves for the Pawn, without validating check or special rules.
     * Includes single and double forward moves (if applicable), as well as diagonal captures.
     * Does not include en passant or promotion logic unless explicitly implemented inside.
     * @param pieces A reference to all current pieces on the board.
     * @return std::vector<Coordinate> A list of coordinates the Pawn can move to.
     */
    std::vector<Coordinate> getRawMoves(const Pieces&) override;

    /**
     * @brief Creates a copy of the current Pawn piece.
     * @return std::shared_ptr<Piece> A shared pointer to the cloned Pawn.
     */
    [[nodiscard]] std::shared_ptr<Piece> clone() const override;
};
