// Chess
#include "Chess.h"
#include "Pieces.h"
#include "Common.h"
#include "Move.h"
#include "Constants.h"

int main()
{
	string strBoard = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
//	string strBoard = "##########K###############################R#############r#r#####";
    Chess a(strBoard);
	int codeResponse = 0;
    Pieces mainBoard(strBoard);
    COLOR turn = WHITE;
    mainBoard.updatePotenMoves(turn);

    string res = a.getInput();
	while (res != "exit")
	{
        std::pair<Coordinate ,Coordinate> move = stringToCoordinate(res);
        Coordinate source = flipChessCoord(move.first);
        Coordinate target = flipChessCoord(move.second);

        const std::shared_ptr<Piece>& sourcePiece = mainBoard.getPiece(source);
        const std::shared_ptr<Piece>& targetPiece = mainBoard.getPiece(target);

        if(sourcePiece == nullptr)codeResponse = ERROR_NO_PIECE_AT_SOURCE;
        else if(sourcePiece->getColor() != turn)codeResponse = ERROR_WRONG_COLOR_AT_SOURCE;
        else if(targetPiece != nullptr && targetPiece->getColor() == turn)codeResponse = ERROR_TARGET_HAS_FRIENDLY;
        else if(!sourcePiece->isValidMove(target , mainBoard))codeResponse = ERROR_ILLEGAL_MOVE;
        else if(sourcePiece->willBeInCheck(target , mainBoard))codeResponse = ERROR_MOVE_CAUSES_CHECK;
        else {
            mainBoard.pieceMove(target , sourcePiece);
            turn = colorNot(turn);
            mainBoard.updatePotenMoves(turn);
            mainBoard.isCheck(turn) ? codeResponse = STATUS_CHECK : codeResponse = STATUS_OK;
        }

        a.setCodeResponse(codeResponse);
		res = a.getInput();
	}

	cout << endl << "Exiting " << endl;
	return 0;
}










