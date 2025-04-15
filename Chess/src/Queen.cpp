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