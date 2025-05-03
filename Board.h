
#ifndef HOMEWORK01EX_BOARD_H
#define HOMEWORK01EX_BOARD_H
#pragma once
#include <vector>
#include <string>
#include "Piece.h"

class Board {
    std::vector<std::vector<Piece*>> grid;
    bool whiteTurn = true;
public:
    Board(const std::string& boardString);
    ~Board();
    int validateAndMove(const std::string& move);
    std::vector<std::vector<Piece*>> getGrid() const;
    bool isInCheck(bool white) const;
    bool causesCheck(int srcRow, int srcCol, int destRow, int destCol);
};
#endif //HOMEWORK01EX_BOARD_H
