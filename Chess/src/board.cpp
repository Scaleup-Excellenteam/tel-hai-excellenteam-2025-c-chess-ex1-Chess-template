#include "board.h"
#include "Chess.h"
#include "Move.h"
#include "PriorityQueue.h"
#include <cassert>
#include <cctype>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

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

// This function has no side effects. Useful for proper reuse of move checking
// if 0 then the piece can move
// otherwise, it cannot move
int Board::canMove(Position src, Position dst)
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

    _board[src.y * SIZE + src.x] = piece;
    _board[dst.y * SIZE + dst.x] = dst_piece;
    return 0;
}

int Board::move(Position src, Position dst)
{
    int canMoveRet = canMove(src, dst);
    if (canMoveRet != 0) {
        return canMoveRet;
    }

    auto piece = _board[src.y * SIZE + src.x];

    _board[dst.y * SIZE + dst.x] = piece;
    _board[src.y * SIZE + src.x] = nullptr;

    _turn_color = !_turn_color;

    if (isCheck(_turn_color)) {
        return 41;
    }
    return 42;
}

PriorityQueue<Move, MoveComparator> Board::getBestMoves(int depth,
                                                        bool is_white)
{
    PriorityQueue<Move, MoveComparator> pq;

    for (int i = 0; i < SIZE * SIZE; i++) {
        Position curr(i % SIZE, i / SIZE);

        auto piece_ptr = _board[i];
        if (!piece_ptr) {
            continue;
        }
        if (piece_ptr->Color() != is_white) {
            continue;
        }
        vector<Move> valid_moves = piece_ptr->getAllValidMoves(curr);
        for (Move move : valid_moves) {
            if (!isValidMove(move.src, move.dst)) {
                continue;
            }
            scoreMove(move);
            if (depth > 0) {
                PriorityQueue<Move, MoveComparator> oponentBestMoves =
                    getBestMoves(depth - 1, !is_white);
                Move oponentBestMove = oponentBestMoves.poll();
                move.score -= oponentBestMove.score;
            }
            pq.push(move);
        }
    }

    // ensure only 5 moves are listed
    PriorityQueue<Move, MoveComparator> ret;
    for (int i = 0; i < 5; i++) {
        try {
            ret.push(pq.poll());
        } catch (runtime_error) {
            // here to ensure polling doesn't crash
            break;
        }
    }

    return ret;
}

int Board::moveTakesHigherValue(Move &move)
{
    auto src_piece = _board[move.src.x + move.src.y * SIZE];
    if (!src_piece) {
        return 0;
    }
    auto dst_piece = _board[move.dst.x + move.dst.y * SIZE];
    if (!dst_piece) {
        return 0;
    }

    int diff = dst_piece->value - src_piece->value;
    if (diff > 0) {
        return diff;
    }
    return 0;
}

shared_ptr<Piece> Board::findLowestValueAttackerOn(Position dst, bool turn)
{
    for (int i = 0; i < SIZE * SIZE; i++) {
        auto piece = _board[i];
        if (piece == nullptr || piece->Color() == turn) {
            continue;
        }
        Position cur = {i % SIZE, i / SIZE};
        if (isValidMove(cur, dst)) {
            return piece;
        }
    }
    return nullptr;
}

int Board::moveLeavesPieceVulnerable(Move &move)
{
    auto lowest_value_attacker =
        findLowestValueAttackerOn(move.dst, _turn_color);
    if (!lowest_value_attacker) {
        return 0;
    }

    auto piece = _board[move.src.x + move.src.y * SIZE];

    int diff = lowest_value_attacker->value - piece->value;
    if (diff < 0) {
        return diff;
    }
    return 0;
}

int Board::moveMakesPieceAttackHigherValue(Move &move)
{
    auto piece = _board[move.src.x + move.src.y * SIZE];
    // get all valid moves from destination to predict whether it attacks a
    // higher value piece
    shared_ptr<Piece> highest_cost_enemy_piece = nullptr;
    vector<Move> piece_moves = piece->getAllValidMoves(move.dst);
    for (auto piece_move : piece_moves) {
        if (piece_move.pawn_move) {
            continue;
        }
        if (!isValidMove(piece_move.src, piece_move.dst)) {
            continue;
        }
        auto enemy_piece = _board[move.dst.x + move.dst.y * SIZE];
        if (!enemy_piece) {
            continue;
        }
        // now there is definitely a piece
        if (!highest_cost_enemy_piece ||
            highest_cost_enemy_piece->value < enemy_piece->value) {
            highest_cost_enemy_piece = enemy_piece;
        }
    }
    return highest_cost_enemy_piece == nullptr
               ? 0
               : highest_cost_enemy_piece->value;
}

void Board::scoreMove(Move &move)
{
    auto src_piece = _board[move.src.x + move.src.y * SIZE];
    auto dst_piece = _board[move.dst.x + move.dst.y * SIZE];

    int score = moveTakesHigherValue(move);
    move.score += score;
    score = moveLeavesPieceVulnerable(move);
    move.score += score;
    score = moveMakesPieceAttackHigherValue(move);
    move.score += score;
}

bool Board::get_turn_color() const { return _turn_color; }
