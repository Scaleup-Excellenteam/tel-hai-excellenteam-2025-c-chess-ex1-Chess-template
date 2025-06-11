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


std::vector<std::pair<int, int>> Bishop::getLegalMoves(int fromX, int fromY, const Piece* const board[8][8]) const {
    std::vector<std::pair<int, int>> moves;

    const int directions[4][2] = {
        {1, 1},   // down-right
        {1, -1},  // down-left
        {-1, 1},  // up-right
        {-1, -1}  // up-left
    };

    for (const auto& dir : directions) {
        int x = fromX + dir[0];
        int y = fromY + dir[1];

        while (x >= 0 && x < 8 && y >= 0 && y < 8) {
            const Piece* target = board[x][y];

            if (target == nullptr) {
                moves.emplace_back(x, y);
            } else {
                if (target->getIsWhite() != this->getIsWhite()) {
                    moves.emplace_back(x, y); // capture
                }
                break; // can't jump over
            }

            x += dir[0];
            y += dir[1];
        }
    }

    return moves;
}

Piece* Bishop::clone() const {
    return new Bishop(*this);
}