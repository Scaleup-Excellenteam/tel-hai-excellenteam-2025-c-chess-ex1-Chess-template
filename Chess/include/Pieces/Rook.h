#pragma once

#include "Pieces/Piece.h"

class Rook : public Piece {
private:
    bool hasMoved_;

public:
    Rook(bool isWhite);
    virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override;
    virtual std::unique_ptr<Piece> clone() const override; // Remove inline implementation
    virtual std::vector<CMove> legalMoves(int r, int c, const Board& b) const override;
    
    bool getHasMoved() const { return hasMoved_; }
    void setHasMoved(bool moved) { hasMoved_ = moved; }
};