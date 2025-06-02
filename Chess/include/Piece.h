#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H
#include <string>
#include <iostream>
using std::string;

class Piece {
public:
    virtual ~Piece() = default;
    virtual bool is_legel_movement(const std::string& input,
                                   const std::string& board,
                                   bool isWhiteTurn) = 0;
    virtual char get_type() = 0;
};


#endif //CHESS_PIECE_H