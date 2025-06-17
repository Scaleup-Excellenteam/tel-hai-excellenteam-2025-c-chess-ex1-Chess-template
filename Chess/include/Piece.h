#pragma once

#include "string"
#include "Board.h"
using namespace std;
class Board;
class Piece {
protected:
    string name;
    bool is_white;
public:
    Piece(const string& name , bool is_white);
    virtual ~Piece();
    bool isWhite() const;

    string getName() const;
    virtual bool isValidMove(int from_x , int from_y ,int to_x , int to_y , const Board& board)const = 0;

};
