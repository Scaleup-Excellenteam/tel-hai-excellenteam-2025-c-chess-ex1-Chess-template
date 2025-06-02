#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H


#include "Piece.h"
#include "Chess.h"
using namespace std;

class Rook  : public Piece {
private:
    char type;
public:
    explicit Rook(char t) : type(t) {}
    bool is_legel_movement(const string& input, const string& board, bool isWhiteTurn) override;
    char get_type() override;



};


#endif //CHESS_ROOK_H