#pragma once
#include "Piece.h"



class Knight : public Piece {
public:
    Knight(bool isWhite);
    virtual ~Knight() = default;

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const  string& boardStr) const override;
    char getSymbol() const override;
};

