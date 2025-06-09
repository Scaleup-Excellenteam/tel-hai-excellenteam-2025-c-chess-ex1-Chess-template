//
// Created by GUYYY on 13/04/2025.
//

#ifndef PIECE_H
#define PIECE_H


class Piece {
    bool hasMoved = false;
public:
    enum Color { WHITE, BLACK };

    Piece(char symbol);
    virtual ~Piece();

    Color getColor() const;
    char getSymbol() const;

    virtual bool isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                             Piece* board[8][8]) const = 0;

    virtual Piece* clone() const = 0;

    bool getHasMoved() const { return hasMoved; }
    void setHasMoved() { hasMoved = true; }

protected:
    Color _color;
    char _symbol;
};



#endif //PIECE_H
