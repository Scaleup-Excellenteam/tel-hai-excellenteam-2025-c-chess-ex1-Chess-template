//
// Created by GUYYY on 09/05/2025.
//

#include "../include/ChessAI.h"
#include "Chess.h"
#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;

ChessAI::ChessAI() {}

std::pair<int, int> ChessAI::convertInputToCoordinates(const std::string& move) {
    int row = move[0] - 'a';
    int col = move[1] - '1';
    return {row, col};
}

std::string ChessAI::convertCoordinatesToInput(int row, int col) {
    char rowChar = 'a' + row;
    char colChar = '1' + col;
    return std::string() + rowChar + colChar;
}

std::vector<std::string> ChessAI::getAllPossibleMoves(Chess* game, Piece* board[8][8], bool isPlayerTurn) {
    std::vector<std::string> moves;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] && ((isPlayerTurn && board[row][col]->getColor() == Piece::WHITE) ||
                                    (!isPlayerTurn && board[row][col]->getColor() == Piece::BLACK))) {
                for (int destRow = 0; destRow < 8; ++destRow) {
                    for (int destCol = 0; destCol < 8; ++destCol) {
                        if (board[row][col]->isMoveLegal(row, col, destRow, destCol, board)) {
                            moves.push_back(convertCoordinatesToInput(row, col) + convertCoordinatesToInput(destRow, destCol));
                        }
                    }
                }
            }
        }
    }
    return moves;
}

int ChessAI::getBasicMoveScore(Piece* board[8][8], std::string move) {
    int score = 0;
    std::pair<int, int> dest = convertInputToCoordinates(move.substr(2, 2));
    if (board[dest.first][dest.second]) {
        switch (tolower(board[dest.first][dest.second]->getSymbol())) {
            case 'q': score += 9; break;
            case 'r': score += 5; break;
            case 'b': score += 3; break;
            case 'n': score += 3; break;
            case 'p': score += 1; break;
            case 'k': score += 100; break;
        }
    }
    return score;
}

int ChessAI::evaluateMove(Chess* game, Piece* board[8][8], std::string move, bool isPlayerTurn, int depth) {
    if (depth == 0) {
        return getBasicMoveScore(board, move);
    }

    Piece* tempBoard[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            tempBoard[i][j] = nullptr;

    game->createBoardFromString(game->m_boardString, tempBoard);

    std::pair<int, int> src = convertInputToCoordinates(move.substr(0, 2));
    std::pair<int, int> dest = convertInputToCoordinates(move.substr(2, 2));
    tempBoard[dest.first][dest.second] = tempBoard[src.first][src.second];
    tempBoard[src.first][src.second] = nullptr;

    int bestOpponentMoveScore = INT_MIN;
    std::vector<std::string> opponentMoves = getAllPossibleMoves(game, tempBoard, !isPlayerTurn);
    for (const std::string& opponentMove : opponentMoves) {
        int score = evaluateMove(game, tempBoard, opponentMove, !isPlayerTurn, depth - 1);
        bestOpponentMoveScore = std::max(bestOpponentMoveScore, score);
    }

    int score = getBasicMoveScore(board, move);
    score -= bestOpponentMoveScore;

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            delete tempBoard[i][j];

    return score;
}

std::string ChessAI::getBestMove(Chess* game, int depth) {
    PriorityQueue<MoveScore, MoveComparator> bestMoves;
    std::vector<std::string> possibleMoves;

    Piece* tempBoard[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            tempBoard[i][j] = nullptr;

    game->createBoardFromString(game->m_boardString, tempBoard);

    possibleMoves = getAllPossibleMoves(game, tempBoard, game->m_turn);

    for (const std::string& move : possibleMoves) {
        int score = evaluateMove(game, tempBoard, move, game->m_turn, depth);
        bestMoves.push({move, score});
        if (bestMoves.size() > 5) {
            bestMoves.pull();
        }
    }

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            delete tempBoard[i][j];

    return bestMoves.pull().move;
}

