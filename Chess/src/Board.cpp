#include "Board.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include "King.hpp"
#include "Pawn.hpp"
#include <iostream>
using namespace std;

enum MoveCode {
    NO_PIECE = 11,
    OPPONENT_PIECE = 12,
    OWN_PIECE_AT_DEST = 13,
    INVALID_MOVEMENT = 21,
    SELF_CHECK = 31,
    VALID_CHECK = 41,
    VALID_MOVE = 42
};

Board::Board() : whiteTurn(true) {
    board.resize(8, std::vector<Piece*>(8, nullptr));
    setupPieces();
}

Board::~Board() {
    for (auto& row : board)
        for (auto& piece : row)
            delete piece;
}

void Board::setupPieces() {
    board[0][0] = new Rook(true);   board[0][7] = new Rook(true);
    board[0][1] = new Knight(true); board[0][6] = new Knight(true);
    board[0][2] = new Bishop(true); board[0][5] = new Bishop(true);
    board[0][3] = new Queen(true);  board[0][4] = new King(true);
    for (int i = 0; i < 8; ++i) board[1][i] = new Pawn(true);  // white pawns

    for (int i = 0; i < 8; ++i) board[6][i] = new Pawn(false); // black pawns
    board[7][0] = new Rook(false);   board[7][7] = new Rook(false);
    board[7][1] = new Knight(false); board[7][6] = new Knight(false);
    board[7][2] = new Bishop(false); board[7][5] = new Bishop(false);
    board[7][3] = new Queen(false);  board[7][4] = new King(false);

}

int Board::move(const std::string& fromStr, const std::string& toStr) {
    Position from(fromStr), to(toStr);
    Piece* p = board[from.row][from.col];

    if (!p) return NO_PIECE;
    if (p->getColor() != whiteTurn) return OPPONENT_PIECE;
    if (board[to.row][to.col] && board[to.row][to.col]->getColor() == whiteTurn)
        return OWN_PIECE_AT_DEST;
    if (!p->isValidMove(from, to, board)) return INVALID_MOVEMENT;

    Piece* captured = board[to.row][to.col];
    board[to.row][to.col] = p;
    board[from.row][from.col] = nullptr;

    if (isCheck(whiteTurn)) {
        board[from.row][from.col] = p;
        board[to.row][to.col] = captured;
        return SELF_CHECK;
    }

    delete captured;
    whiteTurn = !whiteTurn;
    return isCheck(!whiteTurn) ? VALID_CHECK : VALID_MOVE;
}

bool Board::isCheck(bool white) const {
    Position king = findKing(white);
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (board[r][c] && board[r][c]->getColor() != white)
                if (board[r][c]->isValidMove(Position(r, c), king, board))
                    return true;
    return false;
}

Position Board::findKing(bool white) const {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (board[r][c] && board[r][c]->getColor() == white &&
                tolower(board[r][c]->getChar()) == 'k')
                return Position(r, c);
    return Position(-1, -1);
}

void Board::printBoard() const {
    cout << "\n  a b c d e f g h" << endl;
    for (int row = 0; row < 8; ++row) {
        cout << 8 - row << " ";
        for (int col = 0; col < 8; ++col) {
            cout << (board[row][col] ? board[row][col]->getChar() : '.') << " ";
        }
        cout << 8 - row << endl;
    }
    cout << "  a b c d e f g h\n" << endl;
}

