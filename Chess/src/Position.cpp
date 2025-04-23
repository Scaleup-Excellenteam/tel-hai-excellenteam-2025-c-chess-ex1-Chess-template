#include "Position.hpp"

Position::Position(std::string pos) {
    row = pos[0] - 'a';               // 'a' = 0 (top), ..., 'h' = 7 (bottom)
    col = pos[1] - '1';               // '1' = 0 (left), ..., '8' = 7 (right)
}

Position::Position(int r, int c) : row(r), col(c) {}