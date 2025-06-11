#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "PriorityQueue.h"
#include "Piece.h"
#include <vector>
#include <string>
#include "ThreadPool.h"
#include <chrono>
#include "Chess.h"
#include <unordered_map>






class Chess;


class GameBoard {
    Piece* board[8][8];
    bool isWhiteTurn;


public:
    GameBoard(); 
    GameBoard(const GameBoard& other);
    ~GameBoard();

    Piece* getPiece(int row, int col) const;
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    int handleMove(const std::string& move);
    Piece* const (*getRawBoard())[8] { return board; }
    bool isOwnKingInCheckAfterMove(int fromX, int fromY, int toX, int toY);
    bool doesMoveCauseCheckToOpponent(int fromX, int fromY, int toX, int toY);
    bool getisWhiteTurn(){ return isWhiteTurn; };
    void switchTurn() { isWhiteTurn = !isWhiteTurn; };
    bool isCheckmate(bool isWhite);
    //second part
    int getPieceValue(char symbol);
    int weakPeaceThreat( bool currentPlayerIsWhite);
    int threatStrongerPiece(bool isWhiteTurn);
    int eatOponentPiece(int fromX, int fromY, int toX, int toY);
    int evaluateBoard( bool isWhiteTurn);
    Piece* (*getBoard())[8] {
        return board;
    }
    
    MoveScore minimax(int depth, bool isMaximizingPlayer);
    void undoMove(int fromRow, int fromCol, int toRow, int toCol, Piece* movedPiece, Piece* capturedPiece);

    //third part

    std::vector<std::pair<int, int>> getPlayerPieces(bool isWhite) const;
    MoveScore getBestMoveMultithreaded(int depth, int numThreads);
    void runAutoGame(Chess& chess, int depth, int numThreads);
    std::string getCurrentPositionHash();
   
    //forth part
    int tryCastling(bool isWhite, bool isKingside);
    

    
 
};

#endif
