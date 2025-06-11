//
// Created by HALAA on 12/05/2025.
//

#ifndef CHESS_MODEL_H
#define CHESS_MODEL_H
#include "Board.h"
#include "Move.h"
#include <vector>

class Model {
public:
    static int evaluateMove(int fromX, int fromY, int toX, int toY, const Board& board);
    static int getPieceValue(const std::string& name);
    static std::vector<Move> suggestMovesDepth(const Board& board, bool isWhiteTurn, int numThreads, int depth, int const thresholdScore);
    static int countPlayerPieces(const Board& board, bool isWhiteTurn);

};

#endif //CHESS_MODEL_H
