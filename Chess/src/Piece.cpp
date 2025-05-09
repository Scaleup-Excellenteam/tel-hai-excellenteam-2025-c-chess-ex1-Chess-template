//
// Created by GUYYY on 13/04/2025.
//

#include "Piece.h"
#include <cctype>

Piece::Piece(char symbol) : _symbol(symbol) {
    _color = (std::isupper(symbol) ? WHITE : BLACK);
}

Piece::~Piece() = default;


Piece::Color Piece::getColor() const {
    return _color;
}

char Piece::getSymbol() const {
    return _symbol;
}