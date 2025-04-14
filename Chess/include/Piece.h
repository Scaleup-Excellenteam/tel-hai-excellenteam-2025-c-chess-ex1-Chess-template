#pragma once
#include "Common.h"
#include <set>
#include <vector>
#include <memory>

// Forward declaration of the Pieces class, which represents the board.
class Pieces;

// Abstract base class representing a generic chess piece.
class Piece {
public:
    /**
     * @brief Constructs a Piece object with a unique ID, color, and location.
     * @param id Unique identifier for the piece.
     * @param color The color of the piece (e.g., white or black).
     * @param location The initial location of the piece on the board.
     */
    Piece(ID id, COLOR color, Coordinate location);

    /**
     * @brief Retrieves the unique ID of the piece.
     * @return The piece's unique ID.
     */
    [[nodiscard]] ID getId() const { return _id; }

    /**
     * @brief Retrieves the color of the piece.
     * @return The color of the piece.
     */
    [[nodiscard]] COLOR getColor() const { return _color; }

    /**
     * @brief Retrieves the current location of the piece on the board.
     * @return A reference to the piece's location.
     */
    [[nodiscard]] const Coordinate &getLocation() const { return _location; }

    /**
     * @brief Sets the location of the piece on the board.
     * @param location The new location of the piece.
     */
    void setLocation(const Coordinate &location) { _location = location; }

    /**
     * @brief Retrieves the set of potential moves for the piece.
     * @return A reference to the set of potential moves.
     */
    [[nodiscard]] const std::set<Coordinate> &getPotenMoves() const { return _potenMoves; }

    /**
     * @brief A pure virtual function to be implemented by derived classes to compute the raw possible moves for the piece.
     * @param board The current state of the board.
     * @return A vector of coordinates representing the raw possible moves.
     */
    virtual std::vector<Coordinate> getRawMoves(const Pieces&) = 0;

    /**
     * @brief Checks whether the piece is protected by another piece.
     * @return True if the piece is protected, false otherwise.
     */
    [[nodiscard]] bool isIsProtected() const { return _isProtected; }

    /**
     * @brief Sets the protection status of the piece.
     * @param isProtected True if the piece is protected, false otherwise.
     */
    void setIsProtected(bool isProtected) { _isProtected = isProtected; }

    /**
     * @brief Checks whether the piece has moved.
     * @return True if the piece has moved, false otherwise.
     */
    [[nodiscard]] bool isHasMoved() const { return _hasMoved; }

    /**
     * @brief Sets the moved status of the piece.
     * @param hasMoved True if the piece has moved, false otherwise.
     */
    void setHasMoved(bool hasMoved) { _hasMoved = hasMoved; }

    /**
     * @brief Removes moves that would leave the piece in check.
     * @param board The current state of the board.
     * @return A vector of coordinates representing valid moves that do not result in check.
     */
    std::vector<Coordinate> removeOwnCheck(const Pieces&);

    /**
     * @brief Checks whether a potential move would place the piece in check.
     * @param destination The destination coordinate for the move.
     * @param board The current state of the board.
     * @return True if the move results in check, false otherwise.
     */
    [[nodiscard]] bool willBeInCheck(const Coordinate& destination, const Pieces &board) const;

    /**
     * @brief Adds a list of possible moves to the piece's set of potential moves.
     * @param moves A vector of coordinates representing new potential moves.
     */
    void addMoves(std::vector<Coordinate>);

    /**
     * @brief Clears the piece's set of potential moves.
     */
    void clearMoves();

    /**
     * @brief Checks whether a move to a given coordinate is valid.
     * @param destination The target coordinate.
     * @param board The current state of the board.
     * @return True if the move is valid, false otherwise.
     */
    [[nodiscard]] bool isValidMove(Coordinate, const Pieces&);

    /**
     * @brief Updates the set of potential moves based on the current board state.
     * @param board The current state of the board.
     */
    void updatePotenMoves(const Pieces&);

    /**
     * @brief Clones the current piece, creating a copy with the same properties.
     * @return A shared pointer to a new Piece object, identical to the current one.
     */
    [[nodiscard]] virtual std::shared_ptr<Piece> clone() const = 0;

private:
    ID _id;                    ///< Unique identifier for the piece.
    COLOR _color;              ///< The color of the piece (e.g., white or black).
    Coordinate _location;      ///< The current location of the piece.
    std::set<Coordinate> _potenMoves;  ///< The set of potential moves for the piece.
    bool _hasMoved;            ///< Indicates whether the piece has moved.
    bool _isProtected;         ///< Indicates whether the piece is protected by another piece.
};
