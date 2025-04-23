#include "Piece.hpp"

Piece::Piece(bool white) : isWhite(white) {}
Piece::~Piece() {}
bool Piece::getColor() const { return isWhite; }
