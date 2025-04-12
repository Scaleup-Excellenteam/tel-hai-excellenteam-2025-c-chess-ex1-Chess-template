//
// Created by mohamamdlahwani on 4/7/25.
//
// BoardManager.cpp
#include "BoardManager.h"
#include "Rook.h"
#include "King.h"
#include "Bishop.h"
#include "Queen.h"
#include "Knight.h"
#include "Pawn.h"
#include <iostream>

BoardManager::BoardManager() {
    // Initialize all pointers to nullptr
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            board[r][c] = nullptr;

    turn = true;
}

BoardManager::~BoardManager() {
    clearBoard();
}

void BoardManager::clearBoard() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            delete board[r][c];
            board[r][c] = nullptr;
        }
}

std::pair<int, int> BoardManager::toCoords(char file, char rank) {
    return {file - 'a', rank - '1'};
}

void BoardManager::buildBoard(const std::string& boardString) {
    clearBoard();
    for (int i = 0; i < 64; ++i) {
        char ch = boardString[i];
        if (ch == '#') continue;
        int row = i / 8;
        int col = i % 8;
        bool isWhite = isupper(ch);
        ch = tolower(ch);

        switch (ch) {
            case 'r': board[row][col] = new Rook(isWhite); break;
            case 'k': board[row][col] = new King(isWhite); break;
            case 'b': board[row][col] = new Bishop(isWhite); break;
            case 'q': board[row][col] = new Queen(isWhite); break;
            case 'n' : board[row][col]= new knight(isWhite);break;
            case  'p' : board[row][col]= new Pawn(isWhite); break;
            default: break;
        }
    }
}

int BoardManager::validateMove(const std::string& boardString, const std::string& move) {
    buildBoard(boardString);

    auto [srcRow, srcCol] = toCoords(move[0], move[1]);
    auto [dstRow, dstCol] = toCoords(move[2], move[3]);

    Piece* src = board[srcRow][srcCol];
    Piece* dst = board[dstRow][dstCol];

    if (!src) return 11;
    char pieceChar = boardString[srcRow * 8 + srcCol];
    bool isWhitePiece = isupper(pieceChar);

    if (isWhitePiece != turn) {
        return 12; // this is your opponent's piece
    }
    if (dst && dst->isWhite == src->isWhite) return 13;
    if (!src->isLegalMove(srcRow, srcCol, dstRow, dstCol, board)) return 21;

    // ✅ Flip turn after legal move
    flipTurn();

    return 42;
}
