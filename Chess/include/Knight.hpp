#pragma once
#include "Piece.hpp"

class Knight : public Piece {
public:
    Knight(bool white);
    char getChar() const override;
    bool isValidMove(Position from, Position to, const std::vector<std::vector<Piece*>>& board) override;
};