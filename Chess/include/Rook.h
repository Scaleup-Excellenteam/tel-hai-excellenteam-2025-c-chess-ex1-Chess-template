//
// Created by GUYYY on 14/04/2025.
//

#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece {
public:
    Rook(char symbol);

    bool isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                     Piece* board[8][8]) const override;
};



#endif //ROOK_H
