#pragma once
#include "Move.h"
#include "PriorityQueue.h"
#include "piece.h"
#include <array>
#include <memory>
#include <string>

using namespace std;
class Board
{
private:
    array<shared_ptr<Piece>, SIZE * SIZE> _board;
    bool isPathClear(Position src, Position dst) const;
    bool isCheck(bool target_player) const;
    bool isValidMove(Position src, Position dst) const;

    bool _turn_color = true;

    // scoreMove
    void scoreMove(Move &move);
    int moveTakesHigherValue(Move &move);
    int moveLeavesPieceVulnerable(Move &move);
    int moveMakesPieceAttackHigherValue(Move &move);

    shared_ptr<Piece> findLowestValueAttackerOn(Position dst, bool turn);

public:
    Board();
    Board(string init);
    ~Board();

    bool get_turn_color() const;

    int move(Position src, Position dst);

    PriorityQueue<Move, MoveComparator> getBestMoves(int depth, bool is_white);
};
