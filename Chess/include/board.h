#pragma once
#include "Move.h"
#include "PriorityQueue.h"
#include "piece.h"
#include <array>
#include <memory>
#include <string>
#include <vector>

using namespace std;
class Board
{
private:
    array<shared_ptr<Piece>, SIZE * SIZE> _board;
    bool isPathClear(Position src, Position dst) const;
    bool isCheck(bool target_player) const;
    bool canUncheck(bool target_player) const;
    vector<Move> getAllValidMovesByPlayer(bool this_player) const;
    bool isValidMove(Position src, Position dst) const;

    bool _turn_color = true;

    // scoreMove
    void scoreMove(Move &move);
    int moveTakesHigherValue(Move &move);
    int moveLeavesPieceVulnerable(Move &move);
    int moveMakesPieceAttackHigherValue(Move &move);

    shared_ptr<Piece> findLowestValueAttackerOn(Position dst, bool turn);

    // This function has no side effects. Useful for proper reuse of move
    // checking. if 0 then the piece can move; otherwise, it cannot move
    int canMove(Position src, Position dst) const;

public:
    Board();
    Board(string init);
    ~Board();

    bool get_turn_color() const;

    int move(Position src, Position dst);

    PriorityQueue<Move, MoveComparator> getBestMoves(int depth, bool is_white);
};
