#pragma once
const int SIZE = 8;
struct Position {
    int x, y;
    bool operator==(const Position &other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};
