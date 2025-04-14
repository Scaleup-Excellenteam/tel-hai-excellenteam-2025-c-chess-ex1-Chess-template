#pragma once

#include <iostream>

/**
 * @brief Represents the color of a chess piece.
 */
enum COLOR {
    BLACK, ///< Black color
    WHITE  ///< White color
};

/**
 * @brief Represents the type (ID) of a chess piece.
 */
enum ID {
    KING,   ///< King piece
    QUEEN,  ///< Queen piece
    KNIGHT, ///< Knight piece
    ROOK,   ///< Rook piece
    PAWN,   ///< Pawn piece
    BISHOP  ///< Bishop piece
};

/**
 * @brief Represents a coordinate on the chessboard.
 * The coordinate is represented as a pair:
 * - First: row (int)
 * - Second: column (char)
 * Example: {1, 'a'} represents A1 in standard chess notation.
 */
typedef std::pair<int, char> Coordinate;

/**
 * @brief Equality operator for chessboard coordinates.
 * @param c1 First coordinate.
 * @param c2 Second coordinate.
 * @return true if both coordinates are equal.
 * @return false otherwise.
 */
bool operator==(const Coordinate& c1, const Coordinate& c2);

/**
 * @brief Checks whether a coordinate lies within the bounds of a standard 8x8 chessboard.
 * @param coord The coordinate to check.
 * @return true if the coordinate is valid (1 <= row <= 8 and 'a' <= column <= 'h').
 * @return false otherwise.
 */
bool inBoard(const Coordinate& coord);

/**
 * @brief Returns the opposite color.
 * @param color The current color.
 * @return COLOR The opposite color (BLACK <-> WHITE).
 */
COLOR colorNot(COLOR color);

/**
 * @brief Flips a coordinate vertically as if rotating the board 180 degrees.
 * Useful for perspective flipping between players.
 * @param coord The coordinate to flip.
 * @return Coordinate The flipped coordinate.
 */
Coordinate flipChessCoord(const Coordinate& coord);
