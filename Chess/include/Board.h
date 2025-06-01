#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H
#include <vector>
#include <string>
#include "Piece.h"
#include "PriorityQueue.h"
#include <utility>
class Board {
private:
    std::vector<std::vector<Piece*>> board_game;
    std::string& sharedBoardString;
    bool isWhiteTurn;
    [[nodiscard]] bool isPathClear(int rowSrc, int colSrc, int rowDst, int colDst) const;
    PriorityQueue<std::pair<std::string,int>> bestMoves;
    [[nodiscard]] int scorePosition(const std::string& flatBoard) const;
    int minimax(int depth);
    void rebuildBoard();
    static Piece* createPiece(char type);

public:
    explicit Board(std::string& boardStr);
    ~Board();
    bool movePiece(const std::string& input, int& responseCode);
    [[nodiscard]] std::string toString() const;
    std::vector<std::pair<std::string,int>>
    recommendMoves(int maxDepth = 2, int topN = 3);
    friend std::ostream& operator<<(std::ostream& os, Board& board);
    void autoPlayBenchmark(int numThreads, int maxDepth);

};

#endif //CHESS_BOARD_H
