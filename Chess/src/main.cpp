// Chess
#include "Chess.h"
#include "board.h"

void parseInput(const string& res, Position &pos1, Position &pos2);

int main() {
    //string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    string board = "##########K###############################R#############r#r#####";
    Board b(board);
    Chess a(board);
    int codeResponse = 0;
    string res = a.getInput();
    while (res != "exit") {
        /*
        codeResponse value :
        Illegal movements :
        11 - there is not piece at the source
        12 - the piece in the source is piece of your opponent
        13 - there one of your pieces at the destination
        21 - illegal movement of that piece
        31 - this movement will cause you checkmate

        legal movements :
        41 - the last movement was legal and cause check
        42 - the last movement was legal, next turn
        */

        /**/
        { // put your code here instead that code
            Position pos1, pos2;

            parseInput(res, pos1, pos2);

            a.setCodeResponse(b.move(pos1, pos2));
        }
        /**/

        a.setCodeResponse(codeResponse);
        res = a.getInput();
    }

    cout << endl << "Exiting " << endl;
    return 0;
}

void parseInput(const string& res, Position &pos1, Position &pos2)
{
    pos1.y = tolower(res[0]) - 'a';
    pos1.x = res[1] - '1';
    pos2.y = tolower(res[2]) - 'a';
    pos2.x = res[3] - '1';
}
