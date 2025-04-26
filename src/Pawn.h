#pragma once
#include "Piece.h"
#include "Board.h"

class Pawn : public Piece {
public:
    Pawn(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'P' : 'p'; }

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override;
};
