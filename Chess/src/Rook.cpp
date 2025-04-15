#include "../include/Rook.h"
#include <string>

Rook::Rook(char s) : symbol(s) {}

Rook::Rook(char s, bool isWhite) : Piece(isWhite), symbol(s) {}

char Rook::getSymbol() const {
    return symbol;
}

bool Rook::isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const {
    if (fromX != toX && fromY != toY)
        return false;

    int dx = (toX > fromX) - (toX < fromX);
    int dy = (toY > fromY) - (toY < fromY);

    int x = fromX + dx;
    int y = fromY + dy;

    while (x != toX || y != toY) {
        if (board[x][y] != nullptr)
            return false;
        x += dx;
        y += dy;
    }



    return true;
}

