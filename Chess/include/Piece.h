#ifndef PIECE_H
#define PIECE_H

class Board; // Forward declaration to avoid circular dependency

class Piece {
protected:
    bool isWhite;

public:
    Piece(bool isWhite) : isWhite(isWhite) {}

    virtual ~Piece() {}

    bool isWhitePiece() const {
        return isWhite;
    }

    virtual char getSymbol() const = 0;

    virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const = 0;
};

#endif // PIECE_H
