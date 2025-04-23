#pragma once
#include <string>

struct Position {
    int row, col;
    Position(std::string pos);
    Position(int r, int c);
};