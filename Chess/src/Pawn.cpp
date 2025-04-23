#include "Pawn.hpp"
#include "Position.hpp"

Pawn::Pawn(bool white) : Piece(white) {}

char Pawn::getChar() const {
    return isWhite ? 'P' : 'p';  // White = uppercase, Black = lowercase
}

bool Pawn::isValidMove(Position from, Position to, const std::vector<std::vector<Piece*>>& board) {
    int dir = isWhite ? 1 : -1;
    int startRow = isWhite ? 1 : 6;

    // Move forward one square
    if (from.col == to.col && to.row == from.row + dir && board[to.row][to.col] == nullptr)
        return true;

    // Move forward two squares from starting row
    if (from.col == to.col &&
        from.row == startRow &&
        to.row == from.row + 2 * dir &&
        board[from.row + dir][to.col] == nullptr &&
        board[to.row][to.col] == nullptr)
        return true;

    // Capture diagonally
    if (abs(from.col - to.col) == 1 &&
        to.row == from.row + dir &&
        board[to.row][to.col] &&
        board[to.row][to.col]->getColor() != isWhite)
        return true;

    return false;
}