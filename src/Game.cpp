#include "Game.h"
#include "Board.h"
#include <iostream>
#include <string>

void Game::run() {
    Board board;
    std::string move;
    while (true) {
        board.print();
        std::cout << "Enter move (e.g., a2 a3): ";
        std::string src, dest;
        std::cin >> src >> dest;
        int srcCol = src[0] - 'a';
        int srcRow = 8 - (src[1] - '0');
        int destCol = dest[0] - 'a';
        int destRow = 8 - (dest[1] - '0');
        int code = 0;
        if (!board.movePiece(srcRow, srcCol, destRow, destCol, code)) {
            std::cout << "Move failed. Code: " << code << std::endl;
        }
    }
}
