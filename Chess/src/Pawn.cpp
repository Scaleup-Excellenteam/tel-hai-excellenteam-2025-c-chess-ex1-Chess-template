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

std::vector<std::pair<int, int>> Pawn::getLegalMoves(int fromX, int fromY, const Piece* const board[8][8]) const {
    std::vector<std::pair<int, int>> moves;
    int direction = getIsWhite() ? 1 : -1;
    int startRow = getIsWhite() ? 1 : 6;

    int nextX = fromX + direction;

    // צעד רגיל קדימה
    if (nextX >= 0 && nextX < 8 && board[nextX][fromY] == nullptr) {
        moves.emplace_back(nextX, fromY);

        // צעד כפול מהשורה ההתחלתית
        if (fromX == startRow && board[nextX + direction][fromY] == nullptr) {
            moves.emplace_back(nextX + direction, fromY);
        }
    }

    // הכאה באלכסון
    for (int dy = -1; dy <= 1; dy += 2) {
        int toY = fromY + dy;
        if (toY >= 0 && toY < 8 && nextX >= 0 && nextX < 8) {
            const Piece* target = board[nextX][toY];
            if (target && target->getIsWhite() != this->getIsWhite()) {
                moves.emplace_back(nextX, toY);
            }
        }
    }

    return moves;
}

Piece* Pawn::clone() const {
    return new Pawn(*this);
}