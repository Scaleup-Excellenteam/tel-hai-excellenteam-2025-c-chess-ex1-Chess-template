#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "PriorityQueue.h"
#include "Piece.h"
#include <vector>
#include <string>



class GameBoard {
    Piece* board[8][8];
    bool isWhiteTurn;

public:
    GameBoard(); 
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
    int weakPeaceThreat(Piece* board[8][8], bool currentPlayerIsWhite);
    int threatStrongerPiece(Piece* board[8][8], bool isWhiteTurn);
    int eatOponentPiece(Piece* board[8][8],int fromX, int fromY, int toX, int toY);
    int evaluateBoard(Piece* board[8][8], bool isWhiteTurn);
    Piece* (*getBoard())[8] {
        return board;
    }
    
    MoveScore minimax(Piece* board[8][8], int depth, bool isMaximizingPlayer);
    void undoMove(int fromRow, int fromCol, int toRow, int toCol, Piece* movedPiece, Piece* capturedPiece);

    
 
};

#endif
