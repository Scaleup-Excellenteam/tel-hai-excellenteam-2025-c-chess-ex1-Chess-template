#include "piece.h"
#include <array>
#include <memory>
#include <string>

using namespace std;
class Board {
private:
    static const int SIZE = 8;
    array<shared_ptr<Piece>, SIZE * SIZE> _board;
    bool isPathClear(Position src, Position dst) const;
    bool isCheck(bool target_player) const;
    bool hasLineOfSight(Position src, Position dst) const;

    bool _turn_color = true;

public:
    Board();
    Board(string init);
    ~Board();

    int move(Position src, Position dst);
};
