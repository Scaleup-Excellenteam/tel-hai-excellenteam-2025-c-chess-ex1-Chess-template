#include "piece.h"
#include <cstdlib>

// remove
#include <iostream>
using namespace std;

//bool Bishop::isValidMove(Position src, Position dst) {
//    int dx = dst.x - src.x;
//    int dy = dst.y - src.y;
//
//    // assuming pos1 != pos2 was checked at input
//
//    return (x1 == x2 || y1 == y2);
//}

bool Rook::isValidMove(Position src, Position dst) {
    if (src == dst) return false;
    cout << "debug ivm rook" << endl;
    return (src.x == dst.x || src.y == dst.y);
}

bool King::isValidMove(Position src, Position dst) {
    if (src == dst) return false;
    cout << "debug ivm king" << endl;
    return (abs(src.x - dst.x) <= 1 && abs(src.y - dst.y) <= 1);
}

// bool Piece::isValid(int n) {
//     return n >= 0 && n <= 7;
// }
//
// set<tuple<int, int>> King::getWalk(tuple<int, int> position) {
//     int x, y;
//     set<tuple<int, int>> output;
//
//     tie(x, y) = position;
//     for (int i = y - 1; i <= y + 1; i++) {
//         if (i < 0 || i > 7) {
//             continue;
//         }
//
//         for (int j = x - 1; j <= x + 1; j++) {
//             if (j < 0 || j > 7) {
//                 continue;
//             }
//
//             output.insert(tuple<int, int>(i, j));
//         }
//     }
//
//     return output;
// }
//
// set<tuple<int, int>> King::getAttack(tuple<int, int> position) {
//     return this->getWalk(position);
// }
//
// set<tuple<int, int>> Queen::getWalk(tuple<int, int> position) {
//     throw "unimplemented";
//     int x, y;
//     set<tuple<int, int>> output;
//
//     tie(x, y) = position;
//     for (int i = y - 1; i <= y + 1; i++) {
//         if (i < 0 || i > 7) {
//             continue;
//         }
//
//         for (int j = x - 1; j <= x + 1; j++) {
//             if (j < 0 || j > 7) {
//                 continue;
//             }
//
//             output.insert(tuple<int, int>(j, i));
//         }
//     }
//
//     return output;
// }
//
// set<tuple<int, int>> Queen::getAttack(tuple<int, int> position) {
//     return this->getWalk(position);
// }
//
// set<tuple<int, int>> Rook::getWalk(tuple<int, int> position) {
//     int x, y;
//     set<tuple<int, int>> output;
//
//     tie(x, y) = position;
//     for (int d = 1; d <= 7; d++) {
//         for (int i = -1; i <= 1; i += 2) {
//             int cur_y = y + d * i;
//             if (!isValid(cur_y)) continue;
//             for (int j = -1; j <= 1; j += 2) {
//                 int cur_x = x + d * j;
//                 if (!isValid(cur_x)) continue;
//
//                 output.insert(tuple(cur_y, cur_x));
//             }
//         }
//     }
//
//     return output;
// }
//
// set<tuple<int, int>> Rook::getAttack(tuple<int, int> position) {
//     return this->getWalk(position);
// }
