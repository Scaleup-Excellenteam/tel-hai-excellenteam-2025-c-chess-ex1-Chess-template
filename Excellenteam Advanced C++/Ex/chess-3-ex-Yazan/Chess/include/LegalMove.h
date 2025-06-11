#ifndef CHESS_EX_YAZANHEIB_LEGALMOVE_H
#define CHESS_EX_YAZANHEIB_LEGALMOVE_H

#include <cctype>
#include <sstream>
#include "Chess.h"
#include "Move.h"

using namespace std;


class LegalMove {

private:

    /**
     * At This Method Will Check If The Parameter Char Representing A Valid Piece On The Board.
     * @param piece Char That Representing Player Piece At The Board.
     * @return True If The Piece Is Valid, False Otherwise.
     */
    bool isValidPiece(char piece);


    /**
     * At This Method Will Calculate The Row Number From The Board Index.
     * @param i The Board Index.
     * @return The Row Number.
     */
    inline int calcFromRow(int i);


    /**
     * At This Method Will Calculate The Column Number From The Board Index.
     * @param i The Board Index.
     * @return The Column Number.
     */
    inline int calcFromCol(int i);


    /**
     * At This Method Will Convert To Standard Chess Notation.
     * @param index The Board Index.
     * @return The Compatible Chess Square At This Chess Board.
     */
    string indexToChessSquare(int index);


    /**
     * At This Method Will Try To Generate A Forward Move For Parameter Piece At Index i.
     * @param boardStr The Current Board In String Format.
     * @param i The Starting Square Index.
     * @param from The Source Chess Square.
     * @param to The Target Chess Square.
     * @return True If We Find A Valid Move, False Otherwise.
     */
    bool findForwardMove(const string &boardStr, int i, string &from, string &to);


public:

    /**
     * This Method Will Receives A Chess Board, And Will Find The Best Legal Move For The Board.
     * @param chess The Current Chess Board.
     * @return The Best That Found For The Current Player.
     */
    Move FindTheBestMove(const Chess &chess, int pieceIndex);

};

#endif //CHESS_EX_YAZANHEIB_LEGALMOVE_H
