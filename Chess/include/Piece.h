// Piece.h
#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <utility>

class Piece {
protected:
    bool m_isWhite;  
public:
    Piece() : m_isWhite(true) {} 
    Piece(bool isWhite) : m_isWhite(isWhite) {}
    virtual ~Piece() = default;
    bool getIsWhite() const { return m_isWhite; }
    virtual bool isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const = 0;
    virtual char getSymbol() const = 0;
    virtual std::vector<std::pair<int, int>> getLegalMoves(int x, int y, const Piece* const board[8][8]) const = 0;
    virtual Piece* clone() const = 0;



    static bool isSameColor(const Piece* a, const Piece* b) {
        if (a == nullptr || b == nullptr)
            return false;
        return a->m_isWhite == b->m_isWhite;
    }
};

#endif // PIECE_H
