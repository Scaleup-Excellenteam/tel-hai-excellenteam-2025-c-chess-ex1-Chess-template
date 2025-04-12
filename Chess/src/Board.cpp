#include "Board.h"
#include "Rook.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include <cctype>

Board::Board(std::string& boardStr)
        : sharedBoardString(boardStr), isWhiteTurn(true) {
    board_game.resize(8, std::vector<Piece*>(8, nullptr));
    for (int i = 0; i < 64; ++i) {
        int row = i / 8;
        int col = i % 8;
        char pieceChar = boardStr[i];
        board_game[row][col] = createPiece(pieceChar);
    }
}

Board::~Board() {
    for (auto& row : board_game)
        for (auto& piece : row)
            delete piece;
}


Piece* Board::createPiece(char type) {
    if (type == '#') return nullptr;
    switch (tolower(type)) {
        case 'r': return new Rook(type);
        case 'k': return new King(type);
        case 'q': return new Queen(type);
        case 'b': return new Bishop(type);
        case 'n': return new Knight(type);
    }
    return nullptr;
}

bool Board::movePiece(const string& input, int& responseCode) {
    int col_source = (input[1] - '1');
    int row_source = input[0] - 'a';
    int col_dest = (input[3] - '1');
    int row_dest = input[2] - 'a';

    Piece* srcPiece = board_game[row_source][col_source];
    Piece* destPiece = board_game[row_dest][col_dest];
    if (!srcPiece) {
        responseCode = 11;
        return false;
    }

    bool isWhitePiece = isupper(srcPiece->get_type());
    if (isWhitePiece != isWhiteTurn) {
        responseCode = 12;
        return false;
    }

    if (destPiece && (isupper(destPiece->get_type()) == isWhiteTurn)) {
        responseCode = 13;
        return false;
    }

    string flatBoard = toString();
    if (!srcPiece->is_legel_movement(input, flatBoard, isWhiteTurn)) {
        responseCode = 21;
        return false;
    }

    int index_source = row_source * 8 + col_source;
    int index_dest = row_dest * 8 + col_dest;
    char piece = sharedBoardString[index_source];
    sharedBoardString[index_source] = '#';
    sharedBoardString[index_dest] = piece;
    board_game[row_dest][col_dest] = board_game[row_source][col_source];
    board_game[row_source][col_source] = nullptr;
    responseCode = 42;
    isWhiteTurn = !isWhiteTurn;
    return true;
}

std::string Board::toString() const{
    string flat(64, '#');
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board_game[r][c]) {
                flat[(7 - r) * 8 + c] = board_game[r][c]->get_type();
            }
        }
    }
    return flat;
}

