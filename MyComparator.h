
#pragma once
#include "Move.h"

struct MyComparator {
    int operator()(const Move& a, const Move& b) const {
        return b.score - a.score;
    }
};
