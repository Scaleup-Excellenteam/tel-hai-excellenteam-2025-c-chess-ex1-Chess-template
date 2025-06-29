
#include <iostream>
#include <chrono>
#include "Board.h"
#include "ChessMultithread.h"

int main() {
    Board board;

    // הדגמה עם לוח פתיחה בסיסי (אפשר לעדכן לפי צורך)
    board.setInitialPosition();

    int threadCount = 4;
    int maxMoves = 3;

    ChessMultithread cm(board, true); // true = הלבן בתור

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Move> moves = cm.getTopMoves(threadCount, maxMoves);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Top " << maxMoves << " moves (calculated with " << threadCount << " threads):\n";
    for (const auto& move : moves) {
        std::cout << move << std::endl;
    }

    std::cout << "Duration: " << duration.count() << " ms" << std::endl;

    return 0;
}
