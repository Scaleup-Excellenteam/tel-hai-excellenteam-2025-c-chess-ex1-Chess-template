#include "Move.h"


// C'tor.
Move::Move(int score, string from, string to) : score(score), fromWhere(from), toWhere(to) {}


// Getter Of The Filed Score.
int Move::getScore() const
{
    return score;
}


// Setter Of The Filed Score.
void Move::setScore(int newScore)
{
    score = newScore;
}


// Getter Of The Filed From Where.
string Move::getFrom() const
{
    return fromWhere;
}


// Setter Of The Filed From Where.
void Move::setFrom(string &newFrom)
{
    fromWhere = newFrom;
}


// Getter Of The Filed To Where.
string Move::getTo() const
{
    return toWhere;
}


// Setter Of The Filed To Where.
void Move::setTo(string &newTo)
{
    toWhere = newTo;
}


// '<' operator.
bool Move::operator<(const Move &other) const
{
    return score < other.score;
}