#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H
#include "Piece.h"

class Queen : public Piece{
private:
    char type ;

public:
    bool is_legel_movement(const std::string &input, const std::string &board, bool isWhiteTurn) override;
    char get_type() override;
    Queen(char t) : type(t) {}
};


#endif //CHESS_QUEEN_H
