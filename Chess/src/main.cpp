// Chess
#include "Chess.h"
#include "Board.h"

int main()
{
	string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
	//string board = "##########K###############################R#############r#r#####";
    auto gui = new Chess(board);
    Board logic(board);
    std::string mode;
    int depth;
    std::cout << "Choose mode: manual / auto\n> ";
    std::cin >> mode;
    std::cout << "Enter depth for move evaluation (e.g., 2):\n> ";
    std::cin >> depth;

    if (mode == "auto") {
        std::cout << "Running auto benchmark...\n";
        for (int threads : {0, 2, 4, 8}) {
            logic.autoPlayBenchmark(threads, depth);
        }
        std::cout << "Benchmark complete.\n";
    } else {
        std::cout << logic << std::endl;
        std::string res = gui->getInput();
        while (res != "exit") {
            int codeResponse = 0;
            logic.movePiece(res, codeResponse);
            gui->setCodeResponse(codeResponse);
            std::cout << logic << std::endl;
            res = gui->getInput();
        }
        std::cout << "\nExiting\n";
    }
    delete gui;
    return 0;
}
