#include "Chess.h"
#include "BoardManager.h"

int main() {
	std::string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
	std::string boardCopy = board;

	BoardManager manager;
	Chess* gui = new Chess(boardCopy);
	bool isWhiteTurn = true;

	std::string res = gui->getInput();
	while (res != "exit") {
		int code = manager.validateMove(boardCopy, res);

		// Manually check turn legality (since validateMove doesn't do it)
		int srcRow = res[0] - 'a';
		int srcCol = res[1] - '1';
		char piece = boardCopy[srcRow * 8 + srcCol];

		bool pieceIsWhite = isupper(piece);
		if (piece == '#' || pieceIsWhite != isWhiteTurn) {
			code = 12; // Wrong turn or empty square
		}

		if (code == 41 || code == 42) {
			// Apply move
			int dstRow = res[2] - 'a';
			int dstCol = res[3] - '1';
			int srcIndex = srcRow * 8 + srcCol;
			int dstIndex = dstRow * 8 + dstCol;

			boardCopy[dstIndex] = boardCopy[srcIndex];
			boardCopy[srcIndex] = '#';

			isWhiteTurn = !isWhiteTurn;  // Flip turn only on legal move
		}

		gui = new Chess(boardCopy);
		gui->setCodeResponse(code);
		res = gui->getInput();
	}

	delete gui;
	std::cout << "\nExiting\n";
	return 0;
}
