#ifndef PIECE_H
#define PIECE_H

#include <string>
using std::string;

class Piece {
public:
    virtual ~Piece() {}

    virtual bool isValidMove(int fromX, int fromY, int toX, int toY, const string& board) const = 0;
    virtual char getSymbol() const = 0;
};

#endif
