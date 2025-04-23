#pragma once
#include <vector>
#include <string>
#include "Position.hpp"

class Piece {
protected:
    bool isWhite;
public:
    Piece(bool white);
    virtual ~Piece();
    virtual char getChar() const = 0;
    virtual bool isValidMove(Position from, Position to, const std::vector<std::vector<Piece*>>& board) = 0;
    bool getColor() const;
};