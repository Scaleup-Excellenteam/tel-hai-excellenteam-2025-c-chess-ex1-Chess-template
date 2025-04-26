#pragma once
#include <string>
#include <utility>

class Board;

class Piece {
public:
    Piece(bool isWhite) : isWhite(isWhite) {}
    virtual ~Piece() = default;
    virtual char getSymbol() const = 0;
    virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const = 0;
    bool isWhitePiece() const { return isWhite; }

protected:
    bool isWhite;
};
