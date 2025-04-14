#include "Move.h"
#include "Knight.h"
/**
 * Checks if the piece at the given location has the same color as the provided color.
 * @param board Reference to the board containing pieces.
 * @param location The coordinate of the piece to check.
 * @param color The color to compare.
 * @return true if the piece at the location has the same color as the given color, otherwise false.
 */
bool isSameColor(const Pieces &board,const Coordinate& location, COLOR color) {
    return board.getPiece(location)->getColor() == color;
}
/**
 * Gets the list of free boxes in front of the given piece.
 * It stops searching when a piece of the same color is encountered, or a box is blocked.
 * @param board Reference to the board containing pieces.
 * @param piece The piece for which the free boxes in front are being checked.
 * @return A vector of coordinates representing the free boxes in front of the piece.
 */
std::vector<Coordinate> frontFreeBoxes(const Pieces &board, const Piece &piece) {
    bool searchEnd = false;
    int direction = (piece.getColor() == BLACK) ? -1 : 1;
    int moveLength = direction;
    int row = piece.getLocation().first;
    char coll = piece.getLocation().second;
    std::vector<Coordinate> result;
    while(!searchEnd){
        Coordinate checkFree(row+moveLength , coll);
        if(!inBoard(checkFree))break;
        if(!board.isOccupiedBox(checkFree)){
            result.push_back(checkFree);
        }
        else{
            if(!isSameColor(board,checkFree,piece.getColor())){
                result.push_back(checkFree);
            }
            else{
                board.getPiece(checkFree)->setIsProtected(true);
            }
            searchEnd = true;
        }
        moveLength += direction;
        if(piece.getId() == KING){
            searchEnd = true;
        }
    }
    return result;
}
/**
 * Gets the list of free boxes behind the given piece.
 * It stops searching when a piece of the same color is encountered, or a box is blocked.
 * @param board Reference to the board containing pieces.
 * @param piece The piece for which the free boxes behind are being checked.
 * @return A vector of coordinates representing the free boxes behind the piece.
 */
std::vector<Coordinate> backFreeBoxes(const Pieces &board, const Piece &piece) {
    bool searchEnd = false;
    int direction = (piece.getColor() == BLACK) ? -1 : 1;
    int moveLength = direction;
    int row = piece.getLocation().first;
    char coll = piece.getLocation().second;
    std::vector<Coordinate> result;
    while(!searchEnd){
        Coordinate checkFree(row+moveLength , coll);
        if(!inBoard(checkFree))break;
        if(!board.isOccupiedBox(checkFree)){
            result.push_back(checkFree);
        }
        else{
            if(!isSameColor(board,checkFree,piece.getColor())){
                result.push_back(checkFree);
            }
            else{
                board.getPiece(checkFree)->setIsProtected(true);
            }
            searchEnd = true;
        }
        moveLength += direction;
        if(piece.getId() == KING){
            searchEnd = true;
        }
    }
    return result;
}
/**
 * Gets the list of free boxes to the right of the given piece.
 * It stops searching when a piece of the same color is encountered, or a box is blocked.
 * @param board Reference to the board containing pieces.
 * @param piece The piece for which the free boxes to the right are being checked.
 * @return A vector of coordinates representing the free boxes to the right of the piece.
 */
std::vector<Coordinate> rightFreeBoxes(const Pieces &board, const Piece &piece) {
    bool searchEnd = false;
    int direction = 1;
    int row = piece.getLocation().first;
    char coll = piece.getLocation().second;
    std::vector<Coordinate> result;
    while(!searchEnd){
        Coordinate checkFree(row ,coll+direction);
        if(!inBoard(checkFree))break;
        if(!board.isOccupiedBox(checkFree)){
            result.push_back(checkFree);
        }
        else{
            if(!isSameColor(board,checkFree,piece.getColor())){
                result.push_back(checkFree);
            }
            else{
                board.getPiece(checkFree)->setIsProtected(true);
            }
            searchEnd = true;
        }
        direction++;
        if(piece.getId() == KING){
            searchEnd = true;
        }
    }
    return result;
}
/**
 * Gets the list of free boxes to the left of the given piece.
 * It stops searching when a piece of the same color is encountered, or a box is blocked.
 * @param board Reference to the board containing pieces.
 * @param piece The piece for which the free boxes to the left are being checked.
 * @return A vector of coordinates representing the free boxes to the left of the piece.
 */
std::vector<Coordinate> leftFreeBoxes(const Pieces &board, const Piece &piece) {
    bool searchEnd = false;
    int direction = -1;
    int row = piece.getLocation().first;
    char coll = piece.getLocation().second;
    std::vector<Coordinate> result;
    while(!searchEnd){
        Coordinate checkFree(row , coll+direction);
        if(!inBoard(checkFree))break;
        if(!board.isOccupiedBox(checkFree)){
            result.push_back(checkFree);
        }
        else{
            if(!isSameColor(board,checkFree,piece.getColor())){
                result.push_back(checkFree);
            }
            else{
                board.getPiece(checkFree)->setIsProtected(true);
            }
            searchEnd = true;
        }
        direction--;
        if(piece.getId() == KING){
            searchEnd = true;
        }
    }
    return result;
}
/**
 * Gets the list of free boxes diagonally from the front-right of the given piece.
 * It stops searching when a piece of the same color is encountered, or a box is blocked.
 * @param board Reference to the board containing pieces.
 * @param piece The piece for which the free boxes diagonally to the front-right are being checked.
 * @return A vector of coordinates representing the free boxes diagonally to the front-right of the piece.
 */
std::vector<Coordinate> diagFrontRight(const Pieces &board, const Piece &piece) {
    bool searchEnd = false;
    int direction = (piece.getColor() == BLACK) ? -1 :  1;
    int moveLength = direction;
    int row = piece.getLocation().first;
    char coll = piece.getLocation().second;
    std::vector<Coordinate> result;
    while(!searchEnd){
        Coordinate checkFree(row + moveLength , coll+moveLength);
        if(!inBoard(checkFree))break;
        if(!board.isOccupiedBox(checkFree)){
            result.push_back(checkFree);
        }
        else{
            if(!isSameColor(board,checkFree,piece.getColor())){
                result.push_back(checkFree);
            }
            else{
                board.getPiece(checkFree)->setIsProtected(true);
            }
            searchEnd = true;
        }
        moveLength += direction;
        if(piece.getId() == KING){
            searchEnd = true;
        }
    }
    return result;
}
/**
 * Gets the list of free boxes diagonally from the back-right of the given piece.
 * It stops searching when a piece of the same color is encountered, or a box is blocked.
 * @param board Reference to the board containing pieces.
 * @param piece The piece for which the free boxes diagonally to the back-right are being checked.
 * @return A vector of coordinates representing the free boxes diagonally to the back-right of the piece.
 */
std::vector<Coordinate> diagBackRight(const Pieces &board, const Piece &piece) {
    bool searchEnd = false;
    int rowDirection = (piece.getColor() == WHITE) ? -1 :  1;
    int collDirection = -1*rowDirection;
    int collLength = collDirection;
    int rowLength = rowDirection;
    int row = piece.getLocation().first;
    char coll = piece.getLocation().second;
    std::vector<Coordinate> result;
    while(!searchEnd){
        Coordinate checkFree(row + rowLength , coll+collLength);
        if(!inBoard(checkFree))break;
        if(!board.isOccupiedBox(checkFree)){
            result.push_back(checkFree);
        }
        else{
            if(!isSameColor(board,checkFree,piece.getColor())){
                result.push_back(checkFree);
            }
            else{
                board.getPiece(checkFree)->setIsProtected(true);
            }
            searchEnd = true;
        }
        collLength += collDirection;
        rowLength += rowDirection;
        if(piece.getId() == KING){
            searchEnd = true;
        }
    }
    return result;
}
/**
 * Gets the list of free boxes diagonally from the front-left of the given piece.
 * It stops searching when a piece of the same color is encountered, or a box is blocked.
 * @param board Reference to the board containing pieces.
 * @param piece The piece for which the free boxes diagonally to the front-left are being checked.
 * @return A vector of coordinates representing the free boxes diagonally to the front-left of the piece.
 */
std::vector<Coordinate> diagFrontLeft(const Pieces &board, const Piece &piece) {
    bool searchEnd = false;
    int rowDirection = (piece.getColor() == BLACK) ? -1 :  1;
    int collDirection = -1*rowDirection;
    int collLength = collDirection;
    int rowLength = rowDirection;
    int row = piece.getLocation().first;
    char coll = piece.getLocation().second;
    std::vector<Coordinate> result;
    while (!searchEnd){
        Coordinate checkFree(row+rowLength , coll + collLength);
        if(!inBoard(checkFree))break;
        if(!board.isOccupiedBox(checkFree)){
            result.push_back(checkFree);
        }
        else{
            //is in this box is filled with opposite piece then add the option to eat it , else make this box protected.
            if(!isSameColor(board,checkFree,piece.getColor())){
                result.push_back(checkFree);
            }
            else {
                board.getPiece(checkFree)->setIsProtected(true);
            }
            searchEnd = true;
        }
        collLength += collDirection;
        rowLength += rowDirection;
        if(piece.getId() == KING){
            searchEnd = true;
        }
    }
    return result;
}
/**
 * Gets the list of free boxes diagonally from the back-left of the given piece.
 * It stops searching when a piece of the same color is encountered, or a box is blocked.
 * @param board Reference to the board containing pieces.
 * @param piece The piece for which the free boxes diagonally to the back-left are being checked.
 * @return A vector of coordinates representing the free boxes diagonally to the back-left of the piece.
 */
std::vector<Coordinate> diagBackLeft(const Pieces &board, const Piece &piece) {
    bool searchEnd = false;
    int direction = (piece.getColor() == WHITE) ? -1 :  1;
    int moveLength = direction;
    int row = piece.getLocation().first;
    char coll = piece.getLocation().second;
    std::vector<Coordinate> result;
    while (!searchEnd){
        Coordinate checkFree( row+moveLength , coll + moveLength);
        if(!inBoard(checkFree))break;
        if(!board.isOccupiedBox(checkFree)){
            result.push_back(checkFree);
        }
        else{
            //is in this box is filled with opposite piece then add the option to eat it , else make this box protected.
            if(!isSameColor(board,checkFree,piece.getColor())){
                result.push_back(checkFree);
            }
            else {
                board.getPiece(checkFree)->setIsProtected(true);
            }
            searchEnd = true;
        }
        moveLength += direction;
        if(piece.getId() == KING){
            searchEnd = true;
        }
    }
    return result;
}
/**
 * Checks possible moves for a Knight piece in front of it.
 * The direction depends on the knight's color.
 * @param board Reference to the board containing pieces.
 * @param knight The knight for which possible moves are checked.
 * @return A vector of coordinates representing the possible moves for the knight.
 */
std::vector<Coordinate> frontKnight(const Pieces &board, const Knight &knight) {
    std::vector<Coordinate> result;
    int row = knight.getLocation().first;
    char coll = knight.getLocation().second;
    int direction = (knight.getColor() == WHITE) ? 2 : -2;
    Coordinate checkRight(row + direction , coll + 1);
    Coordinate checkLeft( row + direction , coll - 1);
    std::vector<Coordinate> help;
    help.push_back(checkRight);
    help.push_back(checkLeft);
    for(Coordinate coord : help){
        if(inBoard(coord)){
            if(!board.isOccupiedBox(coord)){
                result.push_back(coord);
            }
            else {
                //is in this coord is filled with opposite piece then add the option to eat it , else make this coord protected.
                if (!isSameColor(board, coord, knight.getColor())){
                    result.push_back(coord);
                }
                else {
                    board.getPiece(coord)->setIsProtected(true);
                }
            }
        }
    }
    return result;
}
/**
 * @brief Calculates possible backward moves for the knight.
 * Determines valid backward "L" shaped moves for the knight based on its position and color.
 * @param board The current chessboard state.
 * @param knight The knight piece.
 * @return A vector of possible coordinates for the knight to move to.
 */
std::vector<Coordinate> backKnight(const Pieces &board, const Knight &knight) {
    std::vector<Coordinate> result;
    int row = knight.getLocation().first;
    char coll = knight.getLocation().second;
    int direction = (knight.getColor() == BLACK) ? 2 : -2;
    Coordinate checkRight(row + direction , coll + 1);
    Coordinate checkLeft(row + direction , coll - 1);
    std::vector<Coordinate> help;
    help.push_back(checkRight);
    help.push_back(checkLeft);
    for(Coordinate coord : help){
        if(inBoard(coord)){
            if(!board.isOccupiedBox(coord)){
                result.push_back(coord);
            }
            else {
                //is in this coord is filled with opposite piece then add the option to eat it , else make this coord protected.
                if (!isSameColor(board, coord, knight.getColor())){
                    result.push_back(coord);
                }
                else {
                    board.getPiece(coord)->setIsProtected(true);
                }
            }
        }
    }
    return result;
}
/**
 * @brief Calculates possible rightward moves for the knight.
 * Determines valid rightward "L" shaped moves for the knight based on its position and color.
 * @param board The current chessboard state.
 * @param knight The knight piece.
 * @return A vector of possible coordinates for the knight to move to.
 */
std::vector<Coordinate> rightKnight(const Pieces &board, const Knight &knight) {
    std::vector<Coordinate> result;
    int row = knight.getLocation().first;
    char coll = knight.getLocation().second;
    int direction = (knight.getColor() == WHITE) ? 2 : -2;
    Coordinate checkRight(row + 1 , coll + direction);
    Coordinate checkLeft(row -1 , coll + direction);
    std::vector<Coordinate> help;
    help.push_back(checkRight);
    help.push_back(checkLeft);
    for(Coordinate coord : help){
        if(inBoard(coord)){
            if(!board.isOccupiedBox(coord)){
                result.push_back(coord);
            }
            else {
                //is in this coord is filled with opposite piece then add the option to eat it , else make this coord protected.
                if (!isSameColor(board, coord, knight.getColor())){
                    result.push_back(coord);
                }
                else{
                    board.getPiece(coord)->setIsProtected(true);
                }
            }
        }
    }
    return result;
}
/**
 * @brief Calculates possible leftward moves for the knight.
 * Determines valid leftward "L" shaped moves for the knight based on its position and color.
 * @param board The current chessboard state.
 * @param knight The knight piece.
 * @return A vector of possible coordinates for the knight to move to.
 */
std::vector<Coordinate> leftKnight(const Pieces &board, const Knight &knight) {
    std::vector<Coordinate> result;
    int row = knight.getLocation().first;
    char coll = knight.getLocation().second;
    int direction = (knight.getColor() == BLACK) ? 2 : -2;
    Coordinate checkRight(row + 1 , coll + direction);
    Coordinate checkLeft(row -1 , coll + direction);
    std::vector<Coordinate> help;
    help.push_back(checkRight);
    help.push_back(checkLeft);
    for(Coordinate coord : help){
        if(inBoard(coord)){
            if(!board.isOccupiedBox(coord)){
                result.push_back(coord);
            }
            else {
                //is in this coord is filled with opposite piece then add the option to eat it , else make this coord protected.
                if (!isSameColor(board, coord, knight.getColor())){
                    result.push_back(coord);
                }
                else{
                    board.getPiece(coord)->setIsProtected(true);
                }
            }
        }
    }
    return result;
}
/**
 * @brief Converts a string representation of two coordinates to a pair of Coordinate objects.
 * Converts a string like "a1b2" into two Coordinate objects representing the positions on the board.
 * @param str The string representing two coordinates (e.g., "a1b2").
 * @return A pair of coordinates.
 */
std::pair<Coordinate, Coordinate> stringToCoordinate(const std::string& str) {
    return {{str[1] - '0', toupper(str[0])} , {str[3] - '0',toupper(str[2])}};
}
