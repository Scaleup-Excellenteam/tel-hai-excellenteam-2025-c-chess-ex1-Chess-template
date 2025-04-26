#pragma once
#include "Piece.h"
#include "Board.h"

class Knight : public Piece {
public:
    Knight(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'N' : 'n'; }

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override;
};
