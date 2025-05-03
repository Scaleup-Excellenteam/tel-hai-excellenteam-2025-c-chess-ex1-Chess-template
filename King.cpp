//
// Created by MyPC on 12/04/2025.
//
#include "King.h"
#include <cmath>

King::King(bool isWhite) : white(isWhite) {}

bool King::isWhite() const { return white; }

char King::symbol() const { return white ? 'K' : 'k'; }

bool King::isLegalMove(int srcRow, int srcCol, int destRow, int destCol,
                       const std::vector<std::vector<Piece*>>& board) const {
    int dRow = std::abs(destRow - srcRow);
    int dCol = std::abs(destCol - srcCol);
    if (dRow <= 1 && dCol <= 1) {
        if (board[destRow][destCol] == nullptr || board[destRow][destCol]->isWhite() != white)
            return true;
    }
    return false;
}