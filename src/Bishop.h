#pragma once
#include "Piece.h"
#include "Board.h"

class Bishop : public Piece {
public:
    Bishop(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'B' : 'b'; }

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override;
};
