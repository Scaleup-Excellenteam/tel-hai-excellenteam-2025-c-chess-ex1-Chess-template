#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include <string>
#include "Board.h"

void loadBoardFromString(const std::string& boardString, Board& board);
int validateMove(const std::string& move, Board& board, bool& isWhiteTurn);
bool isKingInCheck(bool isWhiteKing, const Board& board);


#endif // GAME_UTILS_H
