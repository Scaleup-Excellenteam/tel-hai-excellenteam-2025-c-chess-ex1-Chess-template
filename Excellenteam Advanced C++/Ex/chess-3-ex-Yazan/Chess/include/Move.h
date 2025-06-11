#ifndef CHESS_EX_YAZANHEIB_MOVE_H
#define CHESS_EX_YAZANHEIB_MOVE_H


#include <string>

using namespace std;


class Move {

private:

    int score;
    string fromWhere, toWhere;

public:

    /**
     * Constructor.
     */
    Move(int score, string from, string to);


    /**
     * Destructor.
     */
    ~Move() = default;


    /**
     * Getter Of The Filed Score.
     * @return The Move Score.
     */
    int getScore() const;


    /**
     * Setter Of The Filed Score.
     * @param score The New Score Will Be Replaced.
     */
    void setScore(int score);


    /**
     * Getter Of The Filed fromWhere.
     * @return String Thar Describe From Where The Move.
     */
    string getFrom() const;


    /**
     * Setter Of The Filed fromWhere.
     * @param newFrom The New Move From Location.
     */
    void setFrom(string &newFrom);


    /**
     * Getter Of The Filed toWhere.
     * @return String Thar Describe to Where The Move.
     */
    string getTo() const;


    /**
     * Setter Of The Filed toWhere.
     * @param toWhere To Where The New Move Location.
     */
    void setTo(string &newTo);


    /**
     * Compares Two Move Objects For Priority In A Priority Queue.
     * @param other The Other Move Object To Compare.
     * @return True If Move Has Lower Score Than Other, False Otherwise.
     */
    bool operator<(const Move &other) const;

};

#endif //CHESS_EX_YAZANHEIB_MOVE_H
