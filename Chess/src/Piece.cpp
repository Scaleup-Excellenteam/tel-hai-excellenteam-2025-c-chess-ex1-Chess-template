#include "Board.h"
#include "Piece.h"

Piece::Piece(const string& name  , bool is_white): name(name), is_white(is_white) {}

Piece::~Piece() {}

bool Piece::isWhite() const {
    return is_white;
}
string Piece::getName() const {
    return name;
}
