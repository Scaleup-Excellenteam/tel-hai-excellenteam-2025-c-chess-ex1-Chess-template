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

std::vector<std::pair<int, int>> King::getLegalMoves(int fromX, int fromY, const Piece* const board[8][8]) const {
    std::vector<std::pair<int, int>> moves;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue; // לא להישאר במקום

            int x = fromX + dx;
            int y = fromY + dy;

            if (x >= 0 && x < 8 && y >= 0 && y < 8) {
                const Piece* target = board[x][y];
                if (target == nullptr || target->getIsWhite() != this->getIsWhite()) {
                    moves.emplace_back(x, y);
                }
            }
        }
    }

    return moves;
}


Piece* King::clone() const {
    return new King(*this);
}