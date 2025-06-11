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

std::vector<std::pair<int, int>> Rook::getLegalMoves(int x, int y, const Piece* const board[8][8]) const {
    std::vector<std::pair<int, int>> moves;

    // תנועות לאורך הציר X (ימינה ושמאלה)
    for (int dx = x + 1; dx < 8; ++dx) {
        if (board[dx][y] == nullptr) {
            moves.emplace_back(dx, y);
        } else {
            // אם פיסה של היריב - ניתן לתפוס ולפסיק, אם שלנו - פשוט מפסיקים
            if (board[dx][y]->getIsWhite() != this->getIsWhite()) {
                moves.emplace_back(dx, y);
            }
            break;
        }
    }
    for (int dx = x - 1; dx >= 0; --dx) {
        if (board[dx][y] == nullptr) {
            moves.emplace_back(dx, y);
        } else {
            if (board[dx][y]->getIsWhite() != this->getIsWhite()) {
                moves.emplace_back(dx, y);
            }
            break;
        }
    }

    // תנועות לאורך הציר Y (מעלה ומטה)
    for (int dy = y + 1; dy < 8; ++dy) {
        if (board[x][dy] == nullptr) {
            moves.emplace_back(x, dy);
        } else {
            if (board[x][dy]->getIsWhite() != this->getIsWhite()) {
                moves.emplace_back(x, dy);
            }
            break;
        }
    }
    for (int dy = y - 1; dy >= 0; --dy) {
        if (board[x][dy] == nullptr) {
            moves.emplace_back(x, dy);
        } else {
            if (board[x][dy]->getIsWhite() != this->getIsWhite()) {
                moves.emplace_back(x, dy);
            }
            break;
        }
    }

    return moves;
}

Piece* Rook::clone() const {
    return new Rook(*this);
}
