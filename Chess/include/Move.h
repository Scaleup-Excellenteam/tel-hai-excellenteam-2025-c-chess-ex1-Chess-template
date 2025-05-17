//
// Created by HALAA on 12/05/2025.
//

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <iostream>

struct Move {
    int fromX, fromY;
    int toX, toY;
    int score;

    bool operator>(const Move& other) const { return score > other.score; }
    bool operator<(const Move& other) const { return score < other.score; }

    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << "(" << move.fromX << "," << move.fromY << ") -> ("
           << move.toX << "," << move.toY << ") Score: " << move.score;
        return os;
    }
};

inline std::string moveToNotation(const Move& move) {
    char fromRow = 'a' + move.fromX;
    char fromCol = '1' + move.fromY;
    char toRow   = 'a' + move.toX;
    char toCol   = '1' + move.toY;

    return std::string() + fromRow + fromCol + toRow + toCol;
}



#endif //CHESS_MOVE_H
