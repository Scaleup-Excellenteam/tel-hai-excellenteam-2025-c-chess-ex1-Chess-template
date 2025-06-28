#pragma once

#include "Pieces/Piece.h"

class King : public Piece {
public:
    King(bool isWhite); // constructor
    virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override; // check if the movement is valid
    virtual std::unique_ptr<Piece> clone() const override { return std::make_unique<King>(*this); } // for cloning the piece
    virtual std::vector<CMove> legalMoves(int r, int c, const Board& b) const override; // <--- ADDED THIS DECLARATION
    bool isInCheck(int row, int col, const Board &board) const;
    bool isInCheckmate(int row, int col, const Board &board) const;
    bool isInStalemate(int row, int col, const Board &board) const;
};