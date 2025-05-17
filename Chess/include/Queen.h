#pragma once

#include "Piece.h"
#include "Board.h"
class Queen :public Piece{
public:
    Queen(bool is_white);

    bool isValidMove(int from_x , int from_y ,int to_x , int to_y , const Board &board)const override;
<<<<<<< HEAD
=======

>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
};
