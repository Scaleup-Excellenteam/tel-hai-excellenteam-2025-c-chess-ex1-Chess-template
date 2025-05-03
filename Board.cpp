
#include "Board.h"
#include "Rook.h"
#include "King.h"
#include "Bishop.h"
#include "Queen.h"
#include "Knight.h"
#include "Pawn.h"
#include <cctype>

Board::Board(const std::string& boardString) : grid(8, std::vector<Piece*>(8, nullptr)) {
    for (int i = 0; i < 64; ++i) {
        char c = boardString[i];
        int row = i / 8;
        int col = i % 8;
        switch (c) {
            case 'R': grid[row][col] = new Rook(true); break;
            case 'r': grid[row][col] = new Rook(false); break;
            case 'K': grid[row][col] = new King(true); break;
            case 'k': grid[row][col] = new King(false); break;
            case 'B': grid[row][col] = new Bishop(true); break;
            case 'b': grid[row][col] = new Bishop(false); break;
            case 'Q': grid[row][col] = new Queen(true); break;
            case 'q': grid[row][col] = new Queen(false); break;
            case 'N': grid[row][col] = new Knight(true); break;
            case 'n': grid[row][col] = new Knight(false); break;
            case 'P': grid[row][col] = new Pawn(true); break;
            case 'p': grid[row][col] = new Pawn(false); break;
            default: break;
        }
    }
}

Board::~Board() {
    for (auto& row : grid)
        for (auto* p : row)
            delete p;
}

bool Board::isInCheck(bool white) const {
    int kingRow = -1, kingCol = -1;
    for (int row = 0; row < 8; ++row)
        for (int col = 0; col < 8; ++col)
            if (grid[row][col] && grid[row][col]->symbol() == (white ? 'K' : 'k')) {
                kingRow = row;
                kingCol = col;
            }

    if (kingRow == -1) return false;

    for (int row = 0; row < 8; ++row)
        for (int col = 0; col < 8; ++col)
            if (grid[row][col] && grid[row][col]->isWhite() != white)
                if (grid[row][col]->isLegalMove(row, col, kingRow, kingCol, grid))
                    return true;

    return false;
}

bool Board::causesCheck(int srcRow, int srcCol, int destRow, int destCol) {
    Piece* srcPiece = grid[srcRow][srcCol];
    Piece* destPiece = grid[destRow][destCol];
    grid[destRow][destCol] = srcPiece;
    grid[srcRow][srcCol] = nullptr;
    bool result = isInCheck(srcPiece->isWhite());
    grid[srcRow][srcCol] = srcPiece;
    grid[destRow][destCol] = destPiece;
    return result;
}

int Board::validateAndMove(const std::string& move) {
    int srcCol = move[0] - 'a';
    int srcRow = move[1] - '1';
    int destCol = move[2] - 'a';
    int destRow = move[3] - '1';

    Piece* srcPiece = grid[srcRow][srcCol];
    Piece* destPiece = grid[destRow][destCol];

    if (!srcPiece) return 11;
    if (srcPiece->isWhite() != whiteTurn) return 12;
    if (destPiece && destPiece->isWhite() == whiteTurn) return 13;
    if (!srcPiece->isLegalMove(srcRow, srcCol, destRow, destCol, grid)) return 21;
    if (causesCheck(srcRow, srcCol, destRow, destCol)) return 31;

    delete destPiece;
    grid[destRow][destCol] = srcPiece;
    grid[srcRow][srcCol] = nullptr;

    whiteTurn = !whiteTurn;
    return isInCheck(!whiteTurn) ? 41 : 42;
}

std::vector<std::vector<Piece*>> Board::getGrid() const {
    return grid;
}
