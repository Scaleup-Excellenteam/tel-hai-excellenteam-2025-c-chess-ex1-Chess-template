#include "board.h"
#include "Chess.h"
#include <cassert>
#include <cctype>
#include <iostream>
#include <memory>

Board::Board() {}

Board::Board(string init)
{
    assert(init.length() == SIZE * SIZE);
    bool is_white;
    for (int i = 0; i < SIZE * SIZE; i++) {
        if (init[i] == '#')
            continue;

        is_white = !islower(init[i]);

        switch (tolower(init[i])) {
        case 'r':
            _board[i] = make_shared<Rook>(is_white);
            break;
        case 'n':
            _board[i] = make_shared<Knight>(is_white);
            break;
        case 'b':
            _board[i] = make_shared<Bishop>(is_white);
            break;
        case 'q':
            _board[i] = make_shared<Queen>(is_white);
            break;
        case 'k':
            _board[i] = make_shared<King>(is_white);
            break;
        case 'p':
            _board[i] = make_shared<Pawn>(is_white);
            break;
        }
    }
}

Board::~Board() {}

bool Board::isPathClear(Position src, Position dst) const
{
    int dx = dst.x - src.x;
    int dy = dst.y - src.y;

    int xStep = dx == 0 ? 0 : dx / abs(dx);
    int yStep = dy == 0 ? 0 : dy / abs(dy);

    Position cur = {src.x + xStep, src.y + yStep};

    while (cur != dst) {
        if (_board[cur.y * SIZE + cur.x]) {
            return false;
        }
        cur.x += xStep;
        cur.y += yStep;
    }
    return true;
}

bool Board::isValidMove(Position src, Position dst) const
{
    auto piece = _board[src.y * SIZE + src.x];

    if (dynamic_pointer_cast<Rook>(piece) ||
        dynamic_pointer_cast<Bishop>(piece) ||
        dynamic_pointer_cast<Queen>(piece)) {
        return piece->isValidMove(src, dst) && isPathClear(src, dst);
    } else if (dynamic_pointer_cast<King>(piece) ||
               dynamic_pointer_cast<Knight>(piece)) {
        return piece->isValidMove(src, dst);
    } else if (dynamic_pointer_cast<Pawn>(piece)) {
        auto pawn      = dynamic_pointer_cast<Pawn>(piece);
        auto dst_piece = _board[dst.y * SIZE + dst.x];

        if (dst.x == src.x && dst_piece != nullptr) {
            return false;
        }
        bool isAttacking = abs(dst.x - src.x) == 1 && dst_piece != nullptr;
        return pawn->isValidMove(src, dst, isAttacking);
    }
    return true;
}

bool Board::isCheck(bool target_player) const
{
    // needed if a game is initialized with no kings
    bool has_king = false;
    Position target_king_position;
    for (int i = 0; i < SIZE * SIZE; i++) {
        auto piece = _board[i];
        if (dynamic_pointer_cast<King>(piece) &&
            piece->Color() == target_player) {
            has_king             = true;
            target_king_position = Position{i % SIZE, i / SIZE};
            break;
        }
    }
    if (has_king == false) {
        return false;
    }
    for (int i = 0; i < SIZE * SIZE; i++) {
        auto piece = _board[i];
        if (piece == nullptr || piece->Color() == target_player) {
            continue;
        }
        Position cur = {i % SIZE, i / SIZE};
        if (isValidMove(cur, target_king_position)) {
            return true;
        }
    }
    return false;
}

int Board::move(Position src, Position dst)
{
    auto piece = _board[src.y * SIZE + src.x];
    if (!piece) {
        return 11;
    }

    if (piece->Color() != _turn_color) {
        return 12;
    }

    auto dst_piece = _board[dst.y * SIZE + dst.x];
    if (dst_piece != nullptr && dst_piece->Color() == _turn_color) {
        return 13;
    }
    cout << "debug" << endl;

    if (!isValidMove(src, dst)) {
        return 21;
    }

    _board[dst.y * SIZE + dst.x] = piece;
    _board[src.y * SIZE + src.x] = nullptr;

    // check if current player will be checked
    if (isCheck(_turn_color)) {
        // return to previous state
        _board[src.y * SIZE + src.x] = piece;
        _board[dst.y * SIZE + dst.x] = dst_piece;
        return 31;
    }

    _turn_color = !_turn_color;

    if (isCheck(_turn_color)) {
        return 41;
    }
    return 42;
}
