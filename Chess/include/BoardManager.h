//
// Created by mohamamdlahwani on 4/7/25.
//

#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H
// BoardManager.h
#include <string>
#include "Piece.h"

class BoardManager {
public:
    BoardManager();
    ~BoardManager();
    int validateMove(const std::string& boardString, const std::string& move);
    bool isWhiteTurn() const { return turn; }
    void flipTurn() { turn = !turn; }

private:
    Piece* board[8][8];

    void clearBoard();
    void buildBoard(const std::string& boardString);
    std::pair<int, int> toCoords(char file, char rank);
    bool turn;


};

#endif //BOARDMANAGER_H
