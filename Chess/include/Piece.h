//
// Created by GUYYY on 13/04/2025.
//

#ifndef PIECE_H
#define PIECE_H


class Piece {
public:
    enum Color { WHITE, BLACK };

    Piece(char symbol);
    virtual ~Piece();

    Color getColor() const;
    char getSymbol() const;

    virtual bool isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                             Piece* board[8][8]) const = 0;

protected:
    Color _color;
    char _symbol;
};



#endif //PIECE_H
