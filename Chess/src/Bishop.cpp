#include "../include/Bishop.h"
#include <string>

Bishop::Bishop(char s) : symbol(s) {}

Bishop::Bishop(char s, bool isWhite) : Piece(isWhite), symbol(s) {}

char Bishop::getSymbol() const {
    return symbol;
}

bool Bishop::isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const {
    if (abs(fromX - toX) != abs(fromY - toY))
        return false;

    int dx = (toX > fromX) ? 1 : -1;
    int dy = (toY > fromY) ? 1 : -1;

    int x = fromX + dx;
    int y = fromY + dy;

    while (x != toX) {
        if (board[x][y] != nullptr)
            return false;
        x += dx;
        y += dy;
    }

    return true;
}

