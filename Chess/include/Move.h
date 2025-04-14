#pragma once

#include "Pieces.h"
#include "Knight.h"

/**
 * @brief Checks if the piece at a given coordinate has the specified color.
 * @param pieces The list of all pieces on the board.
 * @param coord The coordinate to check.
 * @param color The color to compare.
 * @return true if there is a piece of the given color at the coordinate, false otherwise.
 */
bool isSameColor(const Pieces&, const Coordinate&, COLOR);

/**
 * @brief Returns all free squares in the forward direction from the given piece.
 * @param pieces The current state of the board.
 * @param piece The piece for which to calculate moves.
 * @return A list of coordinates in the forward direction until an obstacle.
 */
std::vector<Coordinate> frontFreeBoxes(const Pieces&, const Piece&);

/**
 * @brief Returns all free squares in the backward direction from the given piece.
 */
std::vector<Coordinate> backFreeBoxes(const Pieces&, const Piece&);

/**
 * @brief Returns all free squares to the right of the given piece.
 */
std::vector<Coordinate> rightFreeBoxes(const Pieces&, const Piece&);

/**
 * @brief Returns all free squares to the left of the given piece.
 */
std::vector<Coordinate> leftFreeBoxes(const Pieces&, const Piece&);

/**
 * @brief Returns all free diagonal squares in the front-right direction.
 */
std::vector<Coordinate> diagFrontRight(const Pieces&, const Piece&);

/**
 * @brief Returns all free diagonal squares in the back-right direction.
 */
std::vector<Coordinate> diagBackRight(const Pieces&, const Piece&);

/**
 * @brief Returns all free diagonal squares in the front-left direction.
 */
std::vector<Coordinate> diagFrontLeft(const Pieces&, const Piece&);

/**
 * @brief Returns all free diagonal squares in the back-left direction.
 */
std::vector<Coordinate> diagBackLeft(const Pieces&, const Piece&);

/**
 * @brief Returns the coordinates the Knight can jump to in the front direction.
 * Typically includes two possible L-shaped moves.
 */
std::vector<Coordinate> frontKnight(const Pieces&, const Knight&);

/**
 * @brief Returns the coordinates the Knight can jump to in the back direction.
 */
std::vector<Coordinate> backKnight(const Pieces&, const Knight&);

/**
 * @brief Returns the coordinates the Knight can jump to in the right direction.
 */
std::vector<Coordinate> rightKnight(const Pieces&, const Knight&);

/**
 * @brief Returns the coordinates the Knight can jump to in the left direction.
 */
std::vector<Coordinate> leftKnight(const Pieces&, const Knight&);

/**
 * @brief Converts a user input string (e.g., "e2e4") to a pair of Coordinates.
 * @param input The input string in standard chess notation.
 * @return A pair containing source and target coordinates.
 */
std::pair<Coordinate, Coordinate> stringToCoordinate(const std::string&);
