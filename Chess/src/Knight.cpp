#include "Knight.hpp"

Knight::Knight(bool white) : Piece(white) {}

char Knight::getChar() const {
    return isWhite ? 'N' : 'n';
}

bool Knight::isValidMove(Position from, Position to, const std::vector<std::vector<Piece*>>& board) {
    int rowDiff = abs(from.row - to.row);
    int colDiff = abs(from.col - to.col);
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}