#include "Board.h"
#include "Rook.h"
#include <cmath>

Board::Board() {
    board.resize(8);
    for (int i = 0; i < 8; ++i) {
        board[i].resize(8);
    }
    // Example: placing white and black rooks for testing
    board[0][0] = std::make_unique<Rook>(true);  // White rook at a1
    board[7][0] = std::make_unique<Rook>(false); // Black rook at a8
}

const Piece* Board::getPiece(int row, int col) const {
    if (!isInsideBoard(row, col)) return nullptr;
    return board[row][col].get();
}

void Board::setPiece(int row, int col, std::unique_ptr<Piece> piece) {
    if (isInsideBoard(row, col)) {
        board[row][col] = std::move(piece);
    }
}

void Board::removePiece(int row, int col) {
    if (isInsideBoard(row, col)) {
        board[row][col].reset();
    }
}

bool Board::isInsideBoard(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool Board::isPathClear(int srcRow, int srcCol, int destRow, int destCol) const {
    if (srcRow == destRow) {
        int step = (destCol > srcCol) ? 1 : -1;
        for (int col = srcCol + step; col != destCol; col += step) {
            if (getPiece(srcRow, col)) return false;
        }
    }
    else if (srcCol == destCol) {
        int step = (destRow > srcRow) ? 1 : -1;
        for (int row = srcRow + step; row != destRow; row += step) {
            if (getPiece(row, srcCol)) return false;
        }
    }
    else {
        return false;
    }
    return true;
}

void Board::movePiece(int srcRow, int srcCol, int destRow, int destCol) {
    if (!isInsideBoard(srcRow, srcCol) || !isInsideBoard(destRow, destCol)) return;
    board[destRow][destCol] = std::move(board[srcRow][srcCol]);
}


std::string Board::toString() const {
    std::string result;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = getPiece(row, col);
            result += piece ? piece->getSymbol() : '#';
        }
    }
    return result;
}