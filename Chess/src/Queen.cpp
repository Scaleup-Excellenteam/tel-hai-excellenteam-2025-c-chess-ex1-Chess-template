#include "../include/Queen.h"
#include <string>

Queen::Queen(char s) : symbol(s) {}

Queen::Queen(char s, bool isWhite) : Piece(isWhite), symbol(s) {}

char Queen::getSymbol() const {
    return symbol;
}

bool Queen::isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const {
    // תנועת אלכסון
    if (abs(fromX - toX) == abs(fromY - toY)) {
        int dx = (toX > fromX) ? 1 : -1;
        int dy = (toY > fromY) ? 1 : -1;
        int x = fromX + dx;
        int y = fromY + dy;

        while (x != toX && y != toY) {
            if (board[x][y] != nullptr)
                return false;
            x += dx;
            y += dy;
        }

        return true;
    }

    // תנועה ישרה
    if (fromX == toX || fromY == toY) {
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

    // תנועה לא חוקית
    return false;
}

std::vector<std::pair<int, int>> Queen::getLegalMoves(int fromX, int fromY, const Piece* const board[8][8]) const {
    std::vector<std::pair<int, int>> moves;

    const int directions[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},   // ישרים
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}  // אלכסונים
    };

    for (auto [dx, dy] : directions) {
        int x = fromX + dx;
        int y = fromY + dy;

        while (x >= 0 && x < 8 && y >= 0 && y < 8) {
            const Piece* target = board[x][y];
            if (target == nullptr) {
                moves.emplace_back(x, y);
            } else {
                if (target->getIsWhite() != this->getIsWhite())
                    moves.emplace_back(x, y);
                break;  // אי אפשר להמשיך מעבר לחייל
            }

            x += dx;
            y += dy;
        }
    }

    return moves;
}

Piece* Queen::clone() const {
    return new Queen(*this);
}
