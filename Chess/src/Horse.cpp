#include "../include/Horse.h"
#include <cmath>

Horse::Horse(char s) : symbol(s) {}

Horse::Horse(char s, bool isWhite) : Piece(isWhite), symbol(s) {}

char Horse::getSymbol() const {
    return symbol;
}

bool Horse::isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const {
   
    int dx = abs(toX - fromX); 
    int dy = abs(toY - fromY); 

    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

std::vector<std::pair<int, int>> Horse::getLegalMoves(int fromX, int fromY, const Piece* const board[8][8]) const {
    std::vector<std::pair<int, int>> moves;

    const int knightMoves[8][2] = {
        {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
    };

    for (const auto& move : knightMoves) {
        int x = fromX + move[0];
        int y = fromY + move[1];

        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            const Piece* target = board[x][y];
            if (target == nullptr || target->getIsWhite() != this->getIsWhite()) {
                moves.emplace_back(x, y);
            }
        }
    }

    return moves;
}

Piece* Horse::clone() const {
    return new Horse(*this);
}