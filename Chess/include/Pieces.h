#pragma once
#include <map>
#include "Common.h"
#include "Piece.h"
#include <memory>

// Class representing the collection of all pieces on the board.
class Pieces {
public:
    /**
     * @brief Constructs a Pieces object with the given board configuration.
     * @param configuration A string representing the initial board setup.
     */
    explicit Pieces(const std::string& configuration);

    /**
     * @brief Copy constructor for the Pieces class.
     * @param other Another Pieces object to copy from.
     */
    Pieces(const Pieces&);

    /**
     * @brief Assignment operator for the Pieces class.
     * @param other Another Pieces object to assign from.
     * @return A reference to this Pieces object.
     */
    Pieces& operator=(const Pieces&) = default;

    /**
     * @brief Destructor for the Pieces class.
     */
    ~Pieces() = default;

    /**
     * @brief Adds a piece to the board.
     * @param piece A shared pointer to the piece to be added.
     */
    void addPiece(const std::shared_ptr<Piece>& piece);

    /**
     * @brief Removes a piece from the board at a given coordinate.
     * @param location The coordinate of the piece to be removed.
     */
    void removePiece(const Coordinate& location);

    /**
     * @brief Retrieves the location of the king for the specified color.
     * @param color The color of the king (e.g., white or black).
     * @return The coordinate of the king's location.
     */
    [[nodiscard]] Coordinate getKingLocation(COLOR color) const;

    /**
     * @brief Retrieves the piece at the specified coordinate.
     * @param location The coordinate of the piece to retrieve.
     * @return A shared pointer to the piece at the given coordinate.
     */
    [[nodiscard]] const std::shared_ptr<Piece>& getPiece(const Coordinate& location) const;

    /**
     * @brief Retrieves the set of potential moves for all pieces of the specified color.
     * @param color The color of the pieces (e.g., white or black).
     * @return A set of coordinates representing the potential moves for the pieces.
     */
    std::set<Coordinate> getPiecesPotenMoves(COLOR color);

    /**
     * @brief Retrieves the raw set of moves for all pieces of the specified color.
     * @param color The color of the pieces (e.g., white or black).
     * @return A set of coordinates representing the raw possible moves for the pieces.
     */
    std::set<Coordinate> getPiecesRawMoves(COLOR color);

    /**
     * @brief Checks whether the specified color is in check.
     * @param color The color to check for check (e.g., white or black).
     * @return True if the specified color is in check, false otherwise.
     */
    bool isCheck(COLOR color);

    /**
     * @brief Updates the potential moves for all pieces of the specified color.
     * @param color The color of the pieces to update moves for.
     */
    void updatePotenMoves(COLOR color);

    /**
     * @brief Moves a piece to a new location.
     * @param location The destination coordinate of the move.
     * @param piece A shared pointer to the piece to be moved.
     */
    void pieceMove(const Coordinate& location, const std::shared_ptr<Piece>& piece);

    /**
     * @brief Checks whether a given coordinate is occupied by a piece.
     * @param location The coordinate to check.
     * @return True if the box is occupied, false otherwise.
     */
    [[nodiscard]] bool isOccupiedBox(const Coordinate& location) const;

private:
    // Map storing the pieces on the board, indexed by their coordinates.
    std::map<Coordinate, std::shared_ptr<Piece>> _board;
};
