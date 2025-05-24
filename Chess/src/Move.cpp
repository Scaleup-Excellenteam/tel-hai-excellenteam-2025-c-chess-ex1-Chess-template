#include "Move.h"

bool Move::operator==(const Move &rhs) const
{
    return src == rhs.src && dst == rhs.dst;
}

int MoveComparator::operator()(const Move &l, const Move &r) const
{
    return l.score - r.score;
}
