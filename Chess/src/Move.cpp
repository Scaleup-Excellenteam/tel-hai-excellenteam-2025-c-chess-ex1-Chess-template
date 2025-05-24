#include "Move.h"

bool Move::operator==(const Move &rhs) const
{
    return src == rhs.src && dst == rhs.dst;
}
