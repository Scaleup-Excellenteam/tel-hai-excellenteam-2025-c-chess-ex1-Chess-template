#pragma once

#include "Piece.h"
#include "Board.h"
class King :public Piece{
public:
    King(bool is_white);
    bool isValidMove(int from_x , int from_y ,int to_x , int to_y , const Board &board)const override;


};
