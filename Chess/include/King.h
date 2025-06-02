#ifndef CHESS_KING_H
#define CHESS_KING_H
#include "Piece.h"

#include "Piece.h"

class King : public Piece {
private:
    char type;
public:
    King(char t) : type(t) {}
    bool is_legel_movement(const string &input, const string &board, bool isWhiteTurn) override;
    char get_type() override;
};


#endif //CHESS_KING_H