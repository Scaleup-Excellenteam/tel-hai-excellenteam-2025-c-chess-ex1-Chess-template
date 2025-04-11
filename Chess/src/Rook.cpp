#include "../include/Rook.h"

Rook::Rook(bool isWhite) : isWhite(isWhite) {}

bool Rook::isValidMove(int fromX, int fromY, int toX, int toY, const std::string& board) const {
    return (fromX == toX && fromY != toY) || (fromY == toY && fromX != toX);
}

char Rook::getSymbol() const {
    return isWhite ? 'R' : 'r';
}
