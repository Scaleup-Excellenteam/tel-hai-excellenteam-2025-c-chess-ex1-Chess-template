#pragma once
#include "Piece.hpp"

class Rook : public Piece {
public:
    Rook(bool white);
    char getChar() const override;
    bool isValidMove(Position from, Position to, const std::vector<std::vector<Piece*>>& board) override;
};