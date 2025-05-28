#include "Chess.h"
#include <iostream>
#include <string>

using namespace std;

#ifdef _WIN32

// clear the screen "cls"
void Chess::clear() const 
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

// create the GUI - ASCII art
void Chess::setFrames() 
{ 
	for (size_t row = 0; row < _SIZE; ++row)
		for (size_t col = 0; col < _SIZE; ++col)
			m_board[row][col] = 32;

	m_board[0][0] = 201;  m_board[0][20] = 187;
	m_board[20][0] = 200; m_board[20][20] = 188;

	for (size_t i = 1; i < 20; ++i)
	{
		m_board[0][i] = 205;
		m_board[20][i] = 205;
		m_board[i][0] = 186;
		m_board[i][20] = 186;
	} 

	m_board[2][2] = 218;  m_board[2][18] = 191;
	m_board[18][2] = 192; m_board[18][18] = 217;

	for (size_t i = 4; i < 17; i += 2)
	{
		m_board[2][i] = 194;
		m_board[18][i] = 193;
		m_board[i][2] = 195;
		m_board[i][18] = 180;
	}

	for (size_t i = 2; i < 19; i += 2)
		for (size_t j = 3; j < 19; j += 2)
			m_board[i][j] = 196;

	for (size_t i = 3; i < 18; i += 2)
		for (size_t j = 2; j < 19; j += 2)
			m_board[i][j] = 179;

	for (size_t i = 4; i < 17; i += 2)
		for (size_t j = 4; j < 17; j += 2)
			m_board[i][j] = 197;

	for (size_t i = 4; i < 17; i += 2)
		m_board[2][i] = 194;
	for (size_t i = 4; i < 17; i += 2)
		m_board[18][i] = 193;
	for (size_t i = 4; i < 17; i += 2)
		m_board[i][2] = 195;
	for (size_t i = 4; i < 17; i += 2)
		m_board[i][18] = 180;

	for (size_t i = 3, t = 0; i < 19; i += 2, ++t)
		m_board[1][i] = m_board[19][i] = ('1' + t);

	for (size_t i = 3, t = 0; i < 19; i += 2, ++t)
		m_board[i][1] = m_board[i][19] = ('A' + t);
}

void Chess::setPieces()
{
	for (size_t row = 0, t = 0; row < 8; ++row)
		for (size_t col = 0; col < 8; ++col, ++t)
			m_board[(3 + (row * 2))][(3 + (col * 2))] = ((m_boardString[t] == '#') ? 32 : m_boardString[t]);
}

#else // non-Windows

void Chess::clear() const
{
	cout << "\033[2J\033[3J\033[H";
}

void Chess::setFrames()
{
	for (size_t row = 0; row < _SIZE; ++row)
		for (size_t col = 0; col < _SIZE; ++col)
			m_board[row][col] = ' ';

	m_board[0][0] = '+';
	m_board[0][20] = '+';
	m_board[20][0] = '+';
	m_board[20][20] = '+';

	for (size_t i = 1; i < 20; ++i)
	{
		m_board[0][i] = '-';
		m_board[20][i] = '-';
		m_board[i][0] = '|';
		m_board[i][20] = '|';
	}

	m_board[2][2] = '+';
	m_board[2][18] = '+';
	m_board[18][2] = '+';
	m_board[18][18] = '+';

	for (size_t i = 4; i < 17; i += 2)
	{
		m_board[2][i] = '+';
		m_board[18][i] = '+';
		m_board[i][2] = '+';
		m_board[i][18] = '+';
	}

	for (size_t i = 2; i < 19; i += 2)
		for (size_t j = 3; j < 19; j += 2)
			m_board[i][j] = '-';

	for (size_t i = 3; i < 18; i += 2)
		for (size_t j = 2; j < 19; j += 2)
			m_board[i][j] = '|';

	for (size_t i = 4; i < 17; i += 2)
		for (size_t j = 4; j < 17; j += 2)
			m_board[i][j] = '+';

	for (size_t i = 4; i < 17; i += 2)
		m_board[2][i] = '+';
	for (size_t i = 4; i < 17; i += 2)
		m_board[18][i] = '+';
	for (size_t i = 4; i < 17; i += 2)
		m_board[i][2] = '+';
	for (size_t i = 4; i < 17; i += 2)
		m_board[i][18] = '+';

	for (size_t i = 3, t = 0; i < 19; i += 2, ++t)
		m_board[1][i] = m_board[19][i] = ('1' + t);

	for (size_t i = 3, t = 0; i < 19; i += 2, ++t)
		m_board[i][1] = m_board[i][19] = ('A' + t);
}

void Chess::setPieces()
{
	for (size_t row = 0, t = 0; row < 8; ++row)
		for (size_t col = 0; col < 8; ++col, ++t)
			m_board[(3 + (row * 2))][(3 + (col * 2))] = ((m_boardString[t] == '#') ? ' ' : m_boardString[t]);
}

#endif // WINDOWS

// print the only the board to screen 
void Chess::show() const 
{
	for (size_t row = 0; row < _SIZE; ++row)
	{
		for (size_t col = 0; col < _SIZE; ++col)
			cout << m_board[row][col];
		cout << endl;
	}
}
// clear screen and print the board and the relevant msg 
void Chess::displayBoard() const
{
	clear();
		show();
	cout << m_msg<< m_errorMsg;
	
}
// print the who is turn before getting input 
void Chess::showAskInput() const 
{
	if (m_turn)
		cout << "Player 1 (White - Capital letters) >> ";
	else
		cout << "Player 2 (Black - Small letters)   >> ";
}
// check if the source and dest are the same 
bool Chess::isSame() const 
{
	return ((m_input[0] == m_input[2]) && (m_input[1] == m_input[3]));
} 
// check if the input is lockations at board
bool Chess::isValid() const
{
	return ((('A' <= m_input[0]) && (m_input[0] <= 'H')) || (('a' <= m_input[0]) && (m_input[0] <= 'h')) &&
		(('1' <= m_input[1]) && (m_input[1] <= '8')) &&
		(('A' <= m_input[2]) && (m_input[2] <= 'H')) || (('a' <= m_input[2]) && (m_input[2] <= 'h')) &&
		(('1' <= m_input[3]) && (m_input[3] <= '8')));
}
	
// check if the input is exit or quit  
bool Chess::isExit() const 
{
	return ((m_input == "exit") || (m_input == "quit") || (m_input == "EXIT") || (m_input == "QUIT"));
}
// execute the movement on board
void Chess::excute()
{
	int row = (m_input[0] - 'a');
	int col = (m_input[1] - '1');
	char pieceInSource = m_boardString[(row * 8) + col];
	m_boardString[(row * 8) + col] = '#';

	row = (m_input[2] - 'a');
	col = (m_input[3] - '1');
	m_boardString[(row * 8) + col] = pieceInSource;

	setPieces();
}
// check the response code and switch turn if needed 
void Chess::doTurn()
{
	m_errorMsg = "\n"; 
	switch (m_codeResponse)
	{
	case 11:
	{
		m_msg = "there is not piece at the source \n";
		break;
	}
	case 12:
	{
		m_msg = "the piece in the source is piece of your opponent \n";
		break;
	}
	case 13:
	{
		m_msg = "there one of your pieces at the destination \n";
		break;
	}
	case 21:
	{
		m_msg = "illegal movement of that piece \n";
		break;
	}
	case 31:
	{
		m_msg = "this movement will cause you checkmate \n";
		break;
	}
	case 41:
	{
		excute();
		m_turn = !m_turn;
		m_msg = "the last movement was legal and cause check \n";
		break;
	}
	case 42:
	{
		excute();
		m_turn = !m_turn;
		m_msg = "the last movement was legal \n";
		break;
	}
	}
}

// C'tor
Chess::Chess(const string& start)
	: m_boardString(start),m_codeResponse(-1)
{
	setFrames();
	setPieces();
}

// get the source and destination 
string Chess::getInput()
{
	// static bool isFirst = true;
	//
	// if (isFirst)
	// 	isFirst = false;
	// else
	// 	doTurn();

	// displayBoard();

	// The suggested move
	// std::string bestMove = getBestMove(depth);
	// std::cout << "Recommended move: " << bestMove << std::endl;

	showAskInput();

	cin >> m_input;
	// if (isExit())
	// 	return "exit";
	// while (!isValid() || isSame())
	// {
	// 	if (!isValid())
	// 		m_errorMsg = "Invalid input !! \n";
	// 	else
	// 		m_errorMsg = "The source and the destination are the same !! \n";
	// 	displayBoard();
	// 	showAskInput();
	// 	cin >> m_input;
	// 	if (isExit())
	// 		return "exit";
	// }

	// if (m_input != "exit")
	// {
	// 	if (('A' <= m_input[0]) && (m_input[0] <= 'H'))
	// 		m_input[0] = (m_input[0] - 'A' + 'a');
	// 	if (('A' <= m_input[2]) && (m_input[2] <= 'H'))
	// 		m_input[2] = (m_input[2] - 'A' + 'a');
	// }

	return m_input;
}

void Chess::setCodeResponse(int codeResponse)
{
	if (((11 <= codeResponse) && (codeResponse <= 13)) ||
		((21 == codeResponse) || (codeResponse == 31)) ||
		((41 == codeResponse) || (codeResponse == 42)))
		m_codeResponse = codeResponse;
}

void Chess::createBoardFromString(const std::string& boardStr, Piece* board[8][8]) {
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			int index = row * 8 + col;
			char symbol = boardStr[index];
			if (symbol == '#' || symbol == ' ') {
				board[row][col] = nullptr;
				continue;
			}

			char lower = std::tolower(symbol);
			switch (lower) {
				case 'r': board[row][col] = new Rook(symbol); break;
				case 'b': board[row][col] = new Bishop(symbol); break;
				case 'q': board[row][col] = new Queen(symbol); break;
				case 'k': board[row][col] = new King(symbol); break;
				case 'n': board[row][col] = new Knight(symbol); break;
				case 'p': board[row][col] = new Pawn(symbol); break;
				default: board[row][col] = nullptr; break;
			}
		}
	}
}

enum ResponseCode {
	NO_PIECE = 11,
	WRONG_TURN = 12,
	SAME_COLOR = 13,
	ILLEGAL_MOVE = 21,
	VALID_MOVE = 42
};


void Chess::calculateResponseCode() {
	Piece* board[8][8] = { nullptr };
	createBoardFromString(m_boardString, board);
	int srcRow = m_input[0] - 'a';
	int srcCol = m_input[1] - '1';
	int destRow = m_input[2] - 'a';
	int destCol = m_input[3] - '1';



	Piece* srcPiece = board[srcRow][srcCol];
	Piece* destPiece = board[destRow][destCol];

	if (!srcPiece)
		m_codeResponse = NO_PIECE;
	else if ((m_turn && srcPiece->getColor() != Piece::WHITE) ||
			 (!m_turn && srcPiece->getColor() != Piece::BLACK))
		m_codeResponse = WRONG_TURN;
	else if (destPiece && destPiece->getColor() == srcPiece->getColor())
		m_codeResponse = SAME_COLOR;
	else if (!srcPiece->isMoveLegal(srcRow, srcCol, destRow, destCol, board))
		m_codeResponse = ILLEGAL_MOVE;
	else
		m_codeResponse = VALID_MOVE;

	std::cout << srcRow << srcCol << std::endl;


	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			delete board[i][j];
}

std::string Chess::getBestMove(int depth) {
	return m_ai.getBestMove(this, depth);
}


std::vector<std::string> Chess::getPiecesOfCurrentTurn() {
	std::vector<std::string> pieces;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			char symbol = m_boardString[row * 8 + col];
			if (symbol == '#' || symbol == ' ')
				continue;
			if (m_turn && isupper(symbol)) // white's turn
				pieces.push_back(std::string() + (char)('a' + row) + (char)('1' + col));
			else if (!m_turn && islower(symbol)) // black's turn
				pieces.push_back(std::string() + (char)('a' + row) + (char)('1' + col));
		}
	}
	return pieces;
}

std::string Chess::getBestMoveForPiece(const std::string& piecePos) {
	return m_ai.getBestMoveForPiece(this, piecePos, depth);

}

bool Chess::playMove(const std::string& move) {
	if (move.size() != 4) return false;
	m_input = move;
	calculateResponseCode();
	if (m_codeResponse == 42 || m_codeResponse == 41) {
		doTurn();
		return true;
	}
	return false;
}

