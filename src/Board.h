#pragma once
#include <vector>
#include <memory>
#include "Piece.h"

class Board {
public:
    Board();
    Piece* getPiece(int row, int col) const;
    bool movePiece(int srcRow, int srcCol, int destRow, int destCol, int& code);
    void print() const;

private:
    std::vector<std::vector<std::shared_ptr<Piece>>> board;
    bool whiteTurn;

    bool causesCheck(bool isWhite);
};
