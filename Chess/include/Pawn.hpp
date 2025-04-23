#pragma once
#include "Piece.hpp"

class Pawn : public Piece {
public:
    Pawn(bool white);
    char getChar() const override;
    bool isValidMove(Position from, Position to, const std::vector<std::vector<Piece*>>& board) override;
};