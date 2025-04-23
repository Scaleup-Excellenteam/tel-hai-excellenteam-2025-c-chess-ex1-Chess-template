#include "Rook.hpp"

Rook::Rook(bool white) : Piece(white) {}

char Rook::getChar() const {
    return isWhite ? 'R' : 'r';
}

bool Rook::isValidMove(Position from, Position to, const std::vector<std::vector<Piece*>>& board) {
    if (from.row != to.row && from.col != to.col)
        return false;

    int rowStep = (to.row - from.row) != 0 ? (to.row - from.row) / abs(to.row - from.row) : 0;
    int colStep = (to.col - from.col) != 0 ? (to.col - from.col) / abs(to.col - from.col) : 0;

    int r = from.row + rowStep, c = from.col + colStep;
    while (r != to.row || c != to.col) {
        if (board[r][c] != nullptr)
            return false;
        r += rowStep;
        c += colStep;
    }
    return true;
}