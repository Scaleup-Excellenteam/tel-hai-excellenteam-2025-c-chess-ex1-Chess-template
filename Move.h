
#pragma once
#include <iostream>

struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
    int score;

    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << "(" << move.fromRow << "," << move.fromCol << ") -> (" 
           << move.toRow << "," << move.toCol << ") = " << move.score;
        return os;
    }
};
