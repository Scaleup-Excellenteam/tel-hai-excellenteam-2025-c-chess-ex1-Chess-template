#include "Chess.h"
#include "Board.h"
#include <iostream>
using namespace std;

int main()
{
    string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    Chess a(board);
    Board logicBoard(board); // נוספה יצירת הלוח ההגיוני
    int codeResponse = 0;
    string res = a.getInput();
    while (res != "exit")
    {

        codeResponse = logicBoard.validateAndMove(res);

        a.setCodeResponse(codeResponse);
        res = a.getInput();
    }

    cout << endl << "Exiting " << endl;
    return 0;
}
// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.