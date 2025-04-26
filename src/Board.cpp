#include "Board.h"
#include "Rook.h"
#include "King.h"
#include <iostream>

Board::Board() : board(8, std::vector<std::shared_ptr<Piece>>(8, nullptr)), whiteTurn(true) {
    board[0][0] = std::make_shared<Rook>(false);
    board[0][7] = std::make_shared<Rook>(false);
    board[0][4] = std::make_shared<King>(false);
    board[7][0] = std::make_shared<Rook>(true);
    board[7][7] = std::make_shared<Rook>(true);
    board[7][4] = std::make_shared<King>(true);
}

Piece* Board::getPiece(int row, int col) const {
    return board[row][col].get();
}

bool Board::movePiece(int srcRow, int srcCol, int destRow, int destCol, int& code) {
    auto piece = getPiece(srcRow, srcCol);
    if (!piece) { code = 11; return false; }
    if (piece->isWhitePiece() != whiteTurn) { code = 12; return false; }

    auto target = getPiece(destRow, destCol);
    if (target && target->isWhitePiece() == whiteTurn) { code = 13; return false; }

    if (!piece->isValidMove(srcRow, srcCol, destRow, destCol, *this)) { code = 21; return false; }

    // move and simulate check (not fully implemented here)
    auto backup = board[destRow][destCol];
    board[destRow][destCol] = board[srcRow][srcCol];
    board[srcRow][srcCol].reset();

    whiteTurn = !whiteTurn;
    code = 42;
    return true;
}

void Board::print() const {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            std::cout << (cell ? cell->getSymbol() : '#') << " ";
        }
        std::cout << std::endl;
    }
}
