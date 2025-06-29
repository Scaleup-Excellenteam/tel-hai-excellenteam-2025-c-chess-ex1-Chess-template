
#include "Rook.h"

Rook::Rook(bool isWhite) : white(isWhite) {}

bool Rook::isWhite() const { return white; }

char Rook::symbol() const { return white ? 'R' : 'r'; }

bool Rook::isLegalMove(int srcRow, int srcCol, int destRow, int destCol,
                       const std::vector<std::vector<Piece*>>& board) const {
    if (srcRow != destRow && srcCol != destCol)
        return false;

    int stepRow = (destRow - srcRow == 0) ? 0 : (destRow - srcRow > 0 ? 1 : -1);
    int stepCol = (destCol - srcCol == 0) ? 0 : (destCol - srcCol > 0 ? 1 : -1);

    int row = srcRow + stepRow;
    int col = srcCol + stepCol;
    while (row != destRow || col != destCol) {
        if (board[row][col] != nullptr)
            return false;
        row += stepRow;
        col += stepCol;
    }

    if (board[destRow][destCol] != nullptr && board[destRow][destCol]->isWhite() == white)
        return false;

    return true;
}