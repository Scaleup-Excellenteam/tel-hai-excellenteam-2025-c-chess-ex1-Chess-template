#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "Piece.h"
class Bishop : public Piece{
private :
    char type;
public:
    Bishop(char t) : type(t) {}
    bool is_legel_movement(const std::string &input, const std::string &board, bool isWhiteTurn) override;
    char get_type() override;


};


#endif //CHESS_BISHOP_H