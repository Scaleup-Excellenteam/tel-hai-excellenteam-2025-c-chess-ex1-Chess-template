#include "GameUtils.h"
#include "Rook.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"

#include <cctype>
#include <memory>
#include <iostream>


void loadBoardFromString(const std::string& boardString, Board& board) {
    if (boardString.size() < 64) {
        std::cerr << "Error: boardString must have exactly 64 characters!" << std::endl;
        return;
    }

    for (int i = 0; i < 64; ++i) {
        char c = boardString[i];
        int col = i / 8;
        int row = i % 8;

        if (c == '#') continue;

        bool isWhite = std::isupper(c);
        char type = std::toupper(c);

        std::unique_ptr<Piece> piece = nullptr;

        switch (type) {
        case 'R':
            piece = std::make_unique<Rook>(isWhite);
            break;
        case 'K':
            piece = std::make_unique<King>(isWhite);
            break;
        case 'Q':
            piece = std::make_unique<Queen>(isWhite);
            break;
        case 'B':
            piece = std::make_unique<Bishop>(isWhite);
            break;
        case 'N':
            piece = std::make_unique<Knight>(isWhite);
            break;
        }
        


        if (piece) {
            board.setPiece(row, col, std::move(piece));
        }
    }
}


int validateMove(const std::string& move, Board& board, bool& isWhiteTurn) {

    int srcCol = move[0] - 'a';
    int srcRow = move[1] - '1';
    int destCol = move[2] - 'a';
    int destRow = move[3] - '1';

    const Piece* srcPiece = board.getPiece(srcRow, srcCol);
    const Piece* dstPiece = board.getPiece(destRow, destCol);

    if (!srcPiece) return 11;
    if (srcPiece->isWhitePiece() != isWhiteTurn) return 12;
    if (dstPiece && dstPiece->isWhitePiece() == isWhiteTurn) return 13;
    if (!srcPiece->isValidMove(srcRow, srcCol, destRow, destCol, board)) return 21;

    // Step 1: Simulate the move on a temporary board
    std::string currentState = board.toString();
    Board tempBoard;
    loadBoardFromString(currentState, tempBoard);
    tempBoard.movePiece(srcRow, srcCol, destRow, destCol);

    // Step 2: check if this move puts our own king in check
    if (isKingInCheck(isWhiteTurn, tempBoard)) return 31;

    // Step 3: apply the move to the real board
    board.movePiece(srcRow, srcCol, destRow, destCol);

    // Step 4: check if the opponent's king is now in check
    bool opponentIsWhite = !isWhiteTurn;
    bool opponentInCheck = isKingInCheck(opponentIsWhite, board);

    // Step 5: switch turn
    isWhiteTurn = !isWhiteTurn;

    // Step 6: return result
    return opponentInCheck ? 41 : 42;
}


bool isKingInCheck(bool isWhiteKing, const Board& board) {


    int kingRow = -1, kingCol = -1;

    // Step 1: Find the king on the board
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = board.getPiece(row, col);
            if (piece && piece->isWhitePiece() == isWhiteKing) {
                if (dynamic_cast<const King*>(piece)) {
                    kingRow = row;
                    kingCol = col;
                    break;
                }
            }
        }
    }

    if (kingRow == -1 || kingCol == -1) {
        std::cerr << "Error: King not found on the board." << std::endl;
        return false;
    }

    // Step 2: Check if any enemy piece can attack the king
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece* attacker = board.getPiece(row, col);
            if (attacker && attacker->isWhitePiece() != isWhiteKing) {
                if (attacker->isValidMove(row, col, kingRow, kingCol, board)) {
                    return true;  // King is under attack
                }
            }
        }
    }

    return false;  // King is safe
}