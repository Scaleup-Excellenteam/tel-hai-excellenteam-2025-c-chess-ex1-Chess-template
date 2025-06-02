#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "Piece.h"
class Knight : public Piece{

private:
    char type;
public:
    Knight(char t) : type(t) {}
    bool is_legel_movement(const string &input, const string &board, bool isWhiteTurn) override;
    char get_type() override;

};


#endif //CHESS_KNIGHT_H