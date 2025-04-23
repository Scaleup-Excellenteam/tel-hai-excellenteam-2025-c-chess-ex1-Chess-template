#pragma once
#include <vector>
#include <string>
#include "Piece.hpp"
#include "Position.hpp"

class Board {
    std::vector<std::vector<Piece*>> board;
    bool whiteTurn;
    void setupPieces();
    bool isCheck(bool white) const;
    Position findKing(bool white) const;
public:
    Board();
    ~Board();
    int move(const std::string& fromStr, const std::string& toStr);
    void printBoard() const;
};