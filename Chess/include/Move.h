#pragma once
#include "Position.h"

struct Move {
    Position src;
    Position dst;
    int score = 0;
    // For pawn where I needed to separate diagonal and forward moves
    bool pawn_move = false;

    bool operator==(const Move &rhs) const;
};

struct MoveComparator {
    int operator()(const Move &l, const Move &r) const
    {
        return l.score - r.score;
    }
};
