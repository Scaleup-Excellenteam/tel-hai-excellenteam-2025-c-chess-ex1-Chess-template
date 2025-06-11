#ifndef CHESS_EX_YAZANHEIB_RUNCHESS_H
#define CHESS_EX_YAZANHEIB_RUNCHESS_H

#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include <chrono>
#include <future>
#include "Chess.h"
#include "LegalMove.h"
#include "Move.h"
#include "MyPriorityQueue.h"
#include "ThreadPool.h"

using namespace std;


class RunChess {

private:

    /**
     * At This Method Will Checks If The Given Piece Belongs To The Player Of The Given Color.
     * @param piece Char Of The Describe The Piece At The Board.
     * @param isWhitePlayer True If Player Turn Is The White Turn, And False For The Black.
     * @return True If The Piece Is For The Player, False Otherwise.
     */
    inline bool isThePlayerPiece(char piece, bool isWhitePlayer);


    /**
     * At This Method Will Return All The Indices Of The Pieces Belonging To The Current Player.
     * @param chess The Current Chess Board.
     * @param whiteTurn True If Player Turn Is The White Turn, And False For The Black.
     * @return Vector Of The Player Pieces Index's.
     */
    vector<int> getAllThePieces(const Chess &chess, bool whiteTurn);


    /**
     * At This Method Will Calcaute The Best Move For Single Piece, And Add To The Queue.
     * @param chess The Current Chess Board.
     * @param movesQueue The Priority Queue To Save The Move.
     * @param pieceIndex Index Of The Piece W'll Calauate The Move For.
     */
    void calcBestMoveForPiece(const Chess &chess, MyPriorityQueue<Move> &movesQueue, int pieceIndex);


    /**
     * @param chess The Current Chess Board.
     * @param movesQueue The Priority Queue To Save The Move.
     * @param pool ThreadPool Object To Manage The Processes.
     * @param pieceIndices Vector Of Indexes Of The player Tools.
     */
    void findMovesParallel(const Chess &chess, MyPriorityQueue<Move> &movesQueue, ThreadPool &pool, const vector<int> &pieceIndices);


    /**
     * At This Method Will Execute An Move And Print The Display The Board.
     * @param chess The Current Chess Board.
     * @param move The Move To Execute.
     */
    void printBoardApplyMove(Chess &chess, const Move &move);


    /**
     * At This Method Will Execute The Best Move From The Moves Queue.
     * @param chess  The Current Chess Board.
     * @param movesQueue Priority Queue Of The Moves
     * @return True If We Executed The Move, False Otherwise.
     */
    bool runTheBestMove(Chess &chess, MyPriorityQueue<Move> &movesQueue);

public:

    /**
     * At This Method Will Run Auto Game, Using The Given Number Of Threads And Move Limit.
     * @param nThreads The Threads Number For The ThreadPool.
     * @param maxMoves The Maximum Number Of Moves Before Automatic Stop.
     */
    void runTheAutoGame(int nThreads, int maxMoves);

};

#endif //CHESS_EX_YAZANHEIB_RUNCHESS_H
