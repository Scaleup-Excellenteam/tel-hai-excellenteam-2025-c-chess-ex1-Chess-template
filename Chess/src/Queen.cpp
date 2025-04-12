//
// Created by mohamamdlahwani on 4/7/25.
//
// Queen.cpp
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"

bool Queen::isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) {
    Rook tempR(isWhite);
    Bishop tempB(isWhite);
    return tempR.isLegalMove(srcRow, srcCol, dstRow, dstCol, board) ||
           tempB.isLegalMove(srcRow, srcCol, dstRow, dstCol, board);
}
