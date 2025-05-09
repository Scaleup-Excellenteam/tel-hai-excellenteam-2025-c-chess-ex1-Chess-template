//
// Created by GUYYY on 09/05/2025.
//

#ifndef CHESSAI_H
#define CHESSAI_H

#include <string>
#include <vector>
#include "PriorityQueue.h"
#include "Piece.h"

class Chess; // הכרזה קדימה

class ChessAI {
public:
    ChessAI();
    std::string getBestMove(Chess* game, int depth);
    int evaluateMove(Chess* game, Piece* board[8][8], std::string move, bool isPlayerTurn, int depth);
    std::vector<std::string> getAllPossibleMoves(Chess* game, Piece* board[8][8], bool isPlayerTurn);
    int getBasicMoveScore(Piece* board[8][8], std::string move);

private:
    std::pair<int, int> convertInputToCoordinates(const std::string& move);
    std::string convertCoordinatesToInput(int row, int col);
};

struct MoveScore {
    std::string move;
    int score;
};

struct MoveComparator {
    int operator()(const MoveScore& a, const MoveScore& b) const {
        return a.score - b.score;
    }
};

#endif // CHESSAI_H