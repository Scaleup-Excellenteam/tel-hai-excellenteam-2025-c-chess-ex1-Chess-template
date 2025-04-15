#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Piece.h"
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

    
 
};

#endif
