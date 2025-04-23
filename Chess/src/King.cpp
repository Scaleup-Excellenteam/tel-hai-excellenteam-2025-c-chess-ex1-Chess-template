#include "King.hpp"

King::King(bool white) : Piece(white) {}

char King::getChar() const {
    return isWhite ? 'K' : 'k';
}

bool King::isValidMove(Position from, Position to, const std::vector<std::vector<Piece*>>& board) {
    int rowDiff = abs(from.row - to.row);
    int colDiff = abs(from.col - to.col);
    return rowDiff <= 1 && colDiff <= 1;
}