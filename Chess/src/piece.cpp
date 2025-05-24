#include "piece.h"
#include <cstdlib>

using namespace std;

inline bool isValidPosition(Position pos)
{
    if (pos.x < 0 || pos.y >= SIZE || pos.y < 0 || pos.x >= SIZE) {
        return false;
    }
    return true;
}

bool Bishop::isValidMove(Position src, Position dst)
{
    int dx = dst.x - src.x;
    int dy = dst.y - src.y;

    if (src == dst)
        return false;
    return abs(dx) == abs(dy);
}

std::vector<Move> Bishop::getAllValidMoves(Position src)
{
    std::vector<Move> ret;
    for (int i = 1; i < SIZE; i++) {
        for (int m = -1; m <= 1; m += 2) {
            for (int n = -1; n <= 1; n += 2) {
                Position dst(src.x + m * i, src.y + n * i);
                if (isValidPosition(dst)) {
                    ret.push_back(Move(src, dst));
                }
            }
        }
    }
    return ret;
}

bool Rook::isValidMove(Position src, Position dst)
{
    if (src == dst)
        return false;
    return (src.x == dst.x || src.y == dst.y);
}

std::vector<Move> Rook::getAllValidMoves(Position src)
{
    std::vector<Move> ret;
    for (int i = 1; i < SIZE; i++) {
        for (int m = -1; m <= 1; m += 2) {
            {
                Position dst(src.x + m * i, src.y);
                if (isValidPosition(dst)) {
                    ret.push_back(Move(src, dst));
                }
            }
            {
                Position dst(src.x, src.y + m * i);
                if (isValidPosition(dst)) {
                    ret.push_back(Move(src, dst));
                }
            }
        }
    }
    return ret;
}

bool King::isValidMove(Position src, Position dst)
{
    if (src == dst)
        return false;
    return (abs(src.x - dst.x) <= 1 && abs(src.y - dst.y) <= 1);
}

std::vector<Move> King::getAllValidMoves(Position src)
{
    std::vector<Move> ret;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            Position dst(src.x + i, src.y + j);
            if (dst != src && isValidPosition(dst)) {
                ret.push_back(Move(src, dst));
            }
        }
    }
    return ret;
}

bool Knight::isValidMove(Position src, Position dst)
{
    int dx = dst.x - src.x;
    int dy = dst.y - src.y;

    if (src == dst)
        return false;
    return abs(dx) == 2 && abs(dy) == 1 || abs(dx) == 1 && abs(dy) == 2;
}

std::vector<Move> Knight::getAllValidMoves(Position src)
{
    std::vector<Move> ret;
    for (int i = -1; i < 2; i += 2) {
        for (int j = -1; j < 2; j += 2) {
            {
                Position dst(src.x + 2 * i, src.y + j);
                if (isValidPosition(dst)) {
                    ret.push_back(Move(src, dst));
                }
            }
            {
                Position dst(src.x + i, src.y + 2 * j);
                if (isValidPosition(dst)) {
                    ret.push_back(Move(src, dst));
                }
            }
        }
    }
    return ret;
}

bool Queen::isValidMove(Position src, Position dst)
{
    if (src == dst)
        return false;
    return Rook().isValidMove(src, dst) || Bishop().isValidMove(src, dst);
}

std::vector<Move> Queen::getAllValidMoves(Position src)
{
    std::vector<Move> ret;
    auto vec1 = Rook().getAllValidMoves(src);
    ret.insert(ret.end(), vec1.begin(), vec1.end());
    auto vec2 = Bishop().getAllValidMoves(src);
    ret.insert(ret.end(), vec2.begin(), vec2.end());
    return ret;
}

bool Pawn::isValidMove(Position src, Position dst, bool isAttacking)
{
    int dx = dst.x - src.x;
    int dy = dst.y - src.y;
    bool is_forward;
    bool is_starting;
    const bool WHITE = true;

    // can't move to the sides unless attacking
    if (!isAttacking && dx != 0) {
        return false;
    }
    // can't move more than one block to the side anyway
    // possibly unnecessary check
    if (abs(dx) > 1) {
        return false;
    }

    if (Color() == WHITE) {
        is_forward  = dy > 0;
        is_starting = src.y == 1;
    } else {
        is_forward  = dy < 0;
        is_starting = src.y == 6;
    }

    // can't move backwards
    if (!is_forward) {
        return false;
    }

    // attacking in diagonal or not attacking and can move forward
    if (!isAttacking && ((is_starting && abs(dy) == 2) || abs(dy) == 1) ||
        (isAttacking && abs(dy) == 1 && abs(dx) == 1)) {
        return true;
    }
    return false;
}

bool Pawn::isValidMove(Position src, Position dst)
{
    return isValidMove(src, dst, false);
}

std::vector<Move> Pawn::getAllValidMoves(Position src)
{
    std::vector<Move> ret;
    int direction = Color() ? 1 : -1;
    // Forward moves
    for (int i = 1; i < 3; i++) {
        Position dst(src.x, src.y + direction * i);
        if (isValidPosition(dst)) {
            Move mv(src, dst);
            mv.pawn_move = true;
            ret.push_back(mv);
        }
    }
    // Attacking diagonals
    {
        Position dst(src.x - 1, src.y + direction);
        if (isValidPosition(dst)) {
            ret.push_back(Move(src, dst));
        }
    }
    {
        Position dst(src.x + 1, src.y + direction);
        if (isValidPosition(dst)) {
            ret.push_back(Move(src, dst));
        }
    }

    return ret;
}
