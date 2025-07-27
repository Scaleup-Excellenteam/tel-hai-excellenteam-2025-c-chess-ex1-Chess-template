#pragma once
#include "Pieces/Piece.h"


class Pawn : public Piece {
public:
    Pawn(bool isWhite); // constructor
    virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override; 
    virtual std::unique_ptr<Piece> clone() const override { return std::make_unique<Pawn>(*this); } 
    virtual std::vector<CMove> legalMoves(int r, int c, const Board& b) const override; 
    bool isValidCapture(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const; 
    bool isValidEnPassant(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const;

};