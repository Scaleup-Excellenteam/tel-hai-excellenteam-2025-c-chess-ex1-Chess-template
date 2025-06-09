//
// Created by GUYYY on 14/04/2025.
//

#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(char symbol);

    bool isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                     Piece* board[8][8]) const override;

    Queen* clone() const override {
        return new Queen(_symbol);
    }
};



#endif //QUEEN_H
