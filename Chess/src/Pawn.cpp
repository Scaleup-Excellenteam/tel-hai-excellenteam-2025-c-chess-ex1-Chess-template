#include "../include/Pawn.h"
#include <cmath>

Pawn::Pawn(char s) : symbol(s) {}

Pawn::Pawn(char s, bool isWhite) : Piece(isWhite), symbol(s) {}

char Pawn::getSymbol() const {
    return symbol;
}

bool Pawn::isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const {
    int direction = !getIsWhite() ? -1 : 1;
    int startRow = !getIsWhite() ? 6 : 1;

   
    if (toY == fromY && toX == fromX + direction && board[toX][toY] == nullptr) {
        return true;
    }

   
    if (fromX == startRow && toY == fromY &&
        toX == fromX + 2 * direction &&
        board[fromX + direction][fromY] == nullptr &&
        board[toX][toY] == nullptr) {
        return true;
    }

   
    if (std::abs(toY - fromY) == 1 && toX == fromX + direction) {
        const Piece* target = board[toX][toY];
        if (target != nullptr && target->getIsWhite() != this->getIsWhite()) {
            return true;
        }
    }

    
    return false;
}
