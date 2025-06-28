#pragma once

#include "Pieces/Piece.h"

class Knight : public Piece {
public:
    Knight(bool isWhite); // constructor
    virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override; // check if the movement is valid
    virtual std::unique_ptr<Piece> clone() const override { return std::make_unique<Knight>(*this); } // for cloning the piece
    virtual std::vector<CMove> legalMoves(int r, int c, const Board& b) const override; // <--- ADDED THIS DECLARATION
};