#pragma once
#include "Piece.h"
#include "Board.h"

class Queen : public Piece {
public:
    Queen(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'Q' : 'q'; }

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override;
};
