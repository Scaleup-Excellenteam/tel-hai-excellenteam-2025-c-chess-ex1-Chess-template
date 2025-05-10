#include "../include/King.h"
#include <cmath>

King::King(char s) : symbol(s) {}

King::King(char s, bool isWhite) : Piece(isWhite), symbol(s) {}

char King::getSymbol() const {
    return symbol;
}

bool King::isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const {
    if (abs(fromX - toX) > 1 || abs(fromY - toY) > 1)
        return false;

    const Piece* target = board[toX][toY];
    if (target && target->getIsWhite() == this->getIsWhite())
        return false;

    return true;
}