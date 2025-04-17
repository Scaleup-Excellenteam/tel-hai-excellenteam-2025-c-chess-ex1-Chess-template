#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <memory>
#include <vector>
#include <iostream>

class Board {
private:
    std::vector<std::vector<std::unique_ptr<Piece>>> board;

public:
    Board();
    const Piece* getPiece(int row, int col) const;
    void setPiece(int row, int col, std::unique_ptr<Piece> piece);
    void removePiece(int row, int col);
    std::string toString() const;

    bool isInsideBoard(int row, int col) const;
    bool isPathClear(int srcRow, int srcCol, int destRow, int destCol) const;
    void movePiece(int srcRow, int srcCol, int destRow, int destCol);
};

#endif // BOARD_H
