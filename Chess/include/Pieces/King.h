#pragma once
#include "Pieces/Piece.h"

class King : public Piece {
public:
    King(bool isWhite);
    virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override;
    // virtual std::unique_ptr<Piece> clone() const override { return std::make_unique<King>(*this); }
    virtual std::unique_ptr<Piece> clone() const override; // Just the declaration now

    bool isInCheck(int row, int col, const Board &board) const;
    bool isInCheckmate(int row, int col, const Board &board) const;
    bool isInStalemate(int row, int col, const Board &board) const;

    bool getHasMoved() const { return hasMoved_; }
    void setHasMoved(bool moved) { hasMoved_ = moved; }

private:
    bool canCastle(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const;
    bool wouldBeInCheck(int row, int col, const Board& board) const;

    bool hasMoved_;
};