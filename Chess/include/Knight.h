#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
    Knight(bool isWhite) : Piece(isWhite) {}

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override;
    char getSymbol() const override { return isWhite ? 'N' : 'n'; }
};
