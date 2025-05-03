
#include "Pawn.h"

Pawn::Pawn(bool isWhite) : white(isWhite) {}

bool Pawn::isWhite() const { return white; }

char Pawn::symbol() const { return white ? 'P' : 'p'; }

bool Pawn::isLegalMove(int srcRow, int srcCol, int destRow, int destCol,
                       const std::vector<std::vector<Piece*>>& board) const {
    int direction = white ? 1 : -1;
    int startRow = white ? 1 : 6;

    if (srcCol == destCol) {
        if (destRow - srcRow == direction && board[destRow][destCol] == nullptr)
            return true;
        if (srcRow == startRow && destRow - srcRow == 2 * direction &&
            board[srcRow + direction][srcCol] == nullptr && board[destRow][destCol] == nullptr)
            return true;
    } else if (std::abs(destCol - srcCol) == 1 && destRow - srcRow == direction) {
        if (board[destRow][destCol] != nullptr && board[destRow][destCol]->isWhite() != white)
            return true;
    }
    return false;
}