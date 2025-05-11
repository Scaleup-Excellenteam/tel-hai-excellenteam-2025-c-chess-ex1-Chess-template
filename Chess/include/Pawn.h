#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H


#include "Piece.h"
#include <string>

class Pawn : public Piece {
private:
    char type;
public:
    explicit Pawn(char t) : type(t) {}
    bool is_legel_movement(const std::string &input,const std::string &board,bool isWhiteTurn) override;
    char get_type() override ;
};

#endif //CHESS_PAWN_H
