#pragma once

#include "Piece.h"
#include "Board.h"
class Bishop :public Piece{
public:
    Bishop(bool is_white);
    bool isValidMove(int from_x , int from_y ,int to_x , int to_y , const Board &board)const override;
};
