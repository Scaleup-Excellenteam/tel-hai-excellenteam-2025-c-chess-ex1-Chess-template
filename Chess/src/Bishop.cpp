#include "Bishop.hpp"

Bishop::Bishop(bool white) : Piece(white) {}

char Bishop::getChar() const {
    return isWhite ? 'B' : 'b';
}

bool Bishop::isValidMove(Position from, Position to, const std::vector<std::vector<Piece*>>& board) {
    int rowDiff = abs(from.row - to.row);
    int colDiff = abs(from.col - to.col);

    if (rowDiff != colDiff)
        return false;

    int rowStep = (to.row - from.row) / rowDiff;
    int colStep = (to.col - from.col) / colDiff;

    int r = from.row + rowStep, c = from.col + colStep;
    while (r != to.row || c != to.col) {
        if (board[r][c] != nullptr)
            return false;
        r += rowStep;
        c += colStep;
    }

    return true;
}