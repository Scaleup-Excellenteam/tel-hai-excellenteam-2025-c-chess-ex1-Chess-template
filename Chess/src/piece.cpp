#include "piece.h"
#include <cstdlib>

using namespace std;

bool Bishop::isValidMove(Position src, Position dst)
{
    int dx = dst.x - src.x;
    int dy = dst.y - src.y;

    if (src == dst)
        return false;
    return abs(dx) == abs(dy);
}

bool Rook::isValidMove(Position src, Position dst)
{
    if (src == dst)
        return false;
    return (src.x == dst.x || src.y == dst.y);
}

bool King::isValidMove(Position src, Position dst)
{
    if (src == dst)
        return false;
    return (abs(src.x - dst.x) <= 1 && abs(src.y - dst.y) <= 1);
}

bool Knight::isValidMove(Position src, Position dst)
{
    int dx = dst.x - src.x;
    int dy = dst.y - src.y;

    if (src == dst)
        return false;
    return abs(dx) == 2 && abs(dy) == 1 || abs(dx) == 1 && abs(dy) == 2;
}

bool Queen::isValidMove(Position src, Position dst)
{
    if (src == dst)
        return false;
    return Rook().isValidMove(src, dst) || Bishop().isValidMove(src, dst);
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
