#include "Queen.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"

Queen::Queen(bool white) : Piece(white) {}

char Queen::getChar() const {
    return isWhite ? 'Q' : 'q';
}

bool Queen::isValidMove(Position from, Position to, const std::vector<std::vector<Piece*>>& board) {
    Rook rookLike(isWhite);
    Bishop bishopLike(isWhite);
    return rookLike.isValidMove(from, to, board) || bishopLike.isValidMove(from, to, board);
}