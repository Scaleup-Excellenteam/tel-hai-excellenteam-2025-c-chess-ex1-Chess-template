//
// Created by GUYYY on 14/04/2025.
//

#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(char symbol);

    bool isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                     Piece* board[8][8]) const override;

    Knight* clone() const override {
        return new Knight(_symbol);
    }
};



#endif //KNIGHT_H
