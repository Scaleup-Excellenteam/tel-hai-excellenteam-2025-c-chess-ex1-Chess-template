#include "Piece.h"
#include <utility>
#include "Pieces.h"

/**
 * @brief Constructs a Piece object.
 * Initializes a Piece with the given ID, color, and location on the chessboard.
 * @param id The type of the piece (e.g., KING, QUEEN, PAWN, etc.).
 * @param color The color of the piece (either WHITE or BLACK).
 * @param location The initial location of the piece on the board.
 */
Piece::Piece(ID id, COLOR color,Coordinate  location): _id(id) , _color(color) , _location(std::move(location)){
    _hasMoved = false;
    _isProtected = false;
}
/**
 * @brief Adds valid moves to the piece's potential move set.
 * If the piece is a KING, it clears the previous moves before adding new ones.
 * @param moves A vector of coordinates representing valid move locations.
 */
void Piece::addMoves(std::vector<Coordinate> moves) {
    if(getId() == KING){
        clearMoves();
    }
    _potenMoves.insert(moves.begin() , moves.end());
}
/**
 * @brief Clears all potential moves for the piece.
 */
void Piece::clearMoves() {
    _potenMoves.clear();
}
/**
 * @brief Checks if a given move is valid for the piece.
 * @param move The target location to move to.
 * @param board The current state of the chessboard.
 * @return True if the move is valid, otherwise false.
 */
bool Piece::isValidMove(Coordinate move , const Pieces& board) {
    std::vector<Coordinate> raw = getRawMoves(board);
    for(auto coord: raw)if(coord == move)return true;
    return false;
}
/**
 * @brief Updates the piece's potential moves based on the current board state.
 * Clears the previous moves and adds the new valid moves while ensuring the piece
 * is not moving into check.
 * @param board The current state of the chessboard.
 */
void Piece::updatePotenMoves(const Pieces & board) {
    clearMoves();
    addMoves(removeOwnCheck(board));
}
/**
 * @brief Filters out moves that would result in the piece being in check.
 * Iterates through all raw moves and removes those that would leave the player in check.
 * @param board The current state of the chessboard.
 * @return A vector of valid moves that do not result in check.
 */
std::vector<Coordinate> Piece::removeOwnCheck(const Pieces &board) {
    std::vector<Coordinate> result = getRawMoves(board);
    for (auto it = result.begin(); it != result.end();){
        Coordinate currentMove = *it;
        Pieces currentBoard(board);

        if(currentBoard.isOccupiedBox(currentMove) && getId() == KING && board.getPiece(currentMove)->isIsProtected()){
            it = result.erase(it);
            continue;
        }
        currentBoard.pieceMove(currentMove,clone());
        std::set<Coordinate> temp = currentBoard.getPiecesRawMoves(colorNot(getColor()));
        if (temp.find(currentBoard.getKingLocation(getColor())) != temp.end()){
            it = result.erase(it);
        }
        else{
            ++it;
        }
    }
    return result;
}
/**
 * @brief Determines if moving the piece to a destination would place the player's king in check.
 * Simulates the move and checks if the opposing player can attack the king after the move.
 * @param destination The target location where the piece would move.
 * @param board The current state of the chessboard.
 * @return True if the move results in a check on the king, otherwise false.
 */
bool Piece::willBeInCheck(const Coordinate& destination, const Pieces &board) const {
    Pieces currentBoard(board);

    currentBoard.pieceMove(destination, clone());

    std::set<Coordinate> opponentMoves = currentBoard.getPiecesRawMoves(colorNot(getColor()));
    Coordinate kingLocation = currentBoard.getKingLocation(getColor());

    return opponentMoves.find(kingLocation) != opponentMoves.end();
}
