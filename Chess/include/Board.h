#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include <vector>
#include <string>
#include "Piece.h"

class Board {
private:
    std::vector<std::vector<Piece*>> board_game;
    std::string& sharedBoardString;
    bool isWhiteTurn;
    static Piece* createPiece(char type);

public:
    explicit Board(std::string& boardStr);
    ~Board();
    bool movePiece(const string& input, int& responseCode);
    [[nodiscard]] std::string toString() const;

};

#endif //CHESS_BOARD_H
