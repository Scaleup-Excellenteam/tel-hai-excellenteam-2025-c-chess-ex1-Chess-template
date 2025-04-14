#include "Pieces.h"
#include "Constants.h"
#include "Rook.h"
#include "Knight.h"
#include "King.h"
#include "Queen.h"
#include "Pawn.h"
#include "Bishop.h"
/**
 * @brief Constructs a Pieces object from a string representing the chessboard.
 * Initializes the chessboard by interpreting a string of piece representations.
 * The string must have a length of BOARD_CELL_NUMBER.
 * @param str A string representing the initial state of the chessboard.
 */
Pieces::Pieces(const std::string &str) {
    if (str.size() != BOARD_CELL_NUMBER) {
        std::cerr << "Invalid board string length: expected " << BOARD_CELL_NUMBER
                  << ", got " << str.size() << std::endl;
        throw std::runtime_error("Invalid board string length: " + str);
    }

    for(int i = 0; i < BOARD_CELL_NUMBER;i++){
        int row = (i / ROW_SIZE) + 1;
        int col = i % ROW_SIZE;
        Coordinate location = {row , FIRST_COLL + col};
        switch (str[i]) {
            case 'r': _board[location] = std::make_shared<Rook>(BLACK ,location); break;
            case 'R': _board[location] = std::make_shared<Rook>(WHITE ,location); break;
            case 'n': _board[location] = std::make_shared<Knight>(BLACK ,location); break;
            case 'N': _board[location] = std::make_shared<Knight>(WHITE ,location); break;
            case 'b': _board[location] = std::make_shared<Bishop>(BLACK ,location); break;
            case 'B': _board[location] = std::make_shared<Bishop>(WHITE ,location); break;
            case 'k': _board[location] = std::make_shared<King>(BLACK ,location); break;
            case 'K': _board[location] = std::make_shared<King>(WHITE ,location); break;
            case 'q': _board[location] = std::make_shared<Queen>(BLACK ,location); break;
            case 'Q': _board[location] = std::make_shared<Queen>(WHITE ,location); break;
            case 'p': _board[location] = std::make_shared<Pawn>(BLACK ,location); break;
            case 'P': _board[location] = std::make_shared<Pawn>(WHITE ,location); break;
            case '#': break;
            default:
                std::cerr << "Invalid character '" << str[i] << "' at index " << i << std::endl;
                throw std::runtime_error("Invalid character in board string: " + str);
        }
    }
}
/**
 * @brief Copy constructor for creating a deep copy of another Pieces object.
 *
 * @param board The Pieces object to copy.
 */Pieces::Pieces(const Pieces &board) {
    for(const auto& element : board._board){
        _board[element.first] = element.second->clone();
    }
}
/**
 * @brief Checks if a specific box on the board is occupied by a piece.
 * @param location The coordinate of the box to check.
 * @return True if the box is occupied, otherwise false.
 */bool Pieces::isOccupiedBox(const Coordinate& location) const {
    return _board.find(location) != _board.end();
}
/**
 * @brief Adds a piece to the board at its current location.
 * @param piece The piece to add to the board.
 */
void Pieces::addPiece(const std::shared_ptr<Piece>& piece) {
    _board[piece->getLocation()] = piece;
}
/**
 * @brief Removes a piece from the board at the specified location.
 * @param location The coordinate of the piece to remove.
 */
void Pieces::removePiece(const Coordinate& location) {
    if(isOccupiedBox(location)){
        _board.erase(location);
    }
}
/**
 * @brief Gets the location of the king of a specific color.
 * @param color The color of the king to find (either WHITE or BLACK).
 * @return The coordinate of the king's location.
 */
Coordinate Pieces::getKingLocation(COLOR color) const {
    for(auto &piece : _board){
        if(piece.second->getId() == KING && piece.second->getColor() == color){
            return piece.second->getLocation();
        }
    }
    return {ROW_PLACE_HOLDER , COLL_PLACE_HOLDER};
}
/**
 * @brief Retrieves a piece at a specific location on the board.
 * @param location The coordinate of the piece to retrieve.
 * @return A reference to the piece at the given location.
 */
const std::shared_ptr<Piece>& Pieces::getPiece(const Coordinate& location)const{
    static const std::shared_ptr<Piece> nullPiece = nullptr;
    if(_board.find(location) == _board.end()){
        return nullPiece;
    }
    return _board.at(location);
}
/**
 * @brief Gets all potential moves for all pieces of a specific color.
 * @param color The color of the pieces to get moves for.
 * @return A set of coordinates representing the potential moves of all pieces of the specified color.
 */
std::set<Coordinate> Pieces::getPiecesPotenMoves(COLOR color) {
    std::set<Coordinate> result;
    for(auto &element : _board){
        if(element.second->getColor() == color){
            auto temp = element.second->getPotenMoves();
            result.insert(temp.begin() , temp.end());
        }
    }
    return result;
}
/**
 * @brief Gets all raw moves for all pieces of a specific color.
 * @param color The color of the pieces to get raw moves for.
 * @return A set of coordinates representing the raw moves of all pieces of the specified color.
 */
std::set<Coordinate> Pieces::getPiecesRawMoves(COLOR color) {
    std::set<Coordinate> result;
    for(auto &element : _board){
        if(element.second->getColor() == color){
            auto temp = element.second->getRawMoves(*this);
            result.insert(temp.begin() , temp.end());
        }
    }
    return result;
}
/**
 * @brief Checks if the king of a specific color is in check.
 * @param color The color of the king to check.
 * @return True if the king is in check, otherwise false.
 */
bool Pieces::isCheck(COLOR color) {
    return getPiecesPotenMoves(colorNot(color)).contains(getKingLocation(color));

}
/**
 * @brief Moves a piece to a new location on the board.
 * Updates the piece's location and removes the piece from its old location.
 * @param destination The new location to move the piece to.
 * @param piece The piece to move.
 */
void Pieces::pieceMove(const Coordinate& destination,const std::shared_ptr<Piece>& piece){
    Coordinate currentLocation = piece->getLocation();
    piece->setLocation(destination);
    piece->setHasMoved(true);
    addPiece(piece);
    //here will be pawn logic.
    removePiece(currentLocation);
}
/**
 * @brief Updates the potential moves for all pieces of a specific color.
 * Iterates over all pieces, updating their potential moves based on the current board state.
 * @param color The color of the pieces to update.
 */
void Pieces::updatePotenMoves(COLOR color) {
    for(auto &element : _board){
        element.second->setIsProtected(false);
    }
    for(const auto& element : _board){
        if(element.second->getColor() != color){
            element.second->updatePotenMoves(*this);
        }
    }
    for(auto &element : _board){
        if(element.second->getColor() == color){
            element.second->updatePotenMoves(*this);
        }
    }
}








