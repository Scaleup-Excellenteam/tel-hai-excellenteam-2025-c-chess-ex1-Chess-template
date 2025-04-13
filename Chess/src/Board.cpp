/* File: Board.Cpp
	utilizes the Board.h methods and the PieceFactory.h methods.
*/

#include "Board.h"
#include "PieceFactory.h"
#include "Utility.h"
#include "Piece.h"
#include "indexStruct.h"

enum class CodeType
{
	CT_NO_PAWN = 11,
	CT_ENEMY_PAWN = 12,
	CT_ALLY_PAWN = 13,
	CT_ILLEGAL_MOVE = 21,
	CT_CHECK_OWN = 31,
	CT_LEGAL_CHECK_MOVE = 41,
	CT_LEGAL_MOVE = 42,
};
//===============================PUBLIC-METHODS===============================
Board::Board(const std::string& players)
	:m_isWhiteTurn(true),m_whiteKing(nullptr),
	m_blackKing(nullptr), m_whiteKingPos({ -1, -1 }),
	m_blackKingPos({ -1, -1 })
{
	initializeBoard(players);
}

/*Validates the players actions and gives the code response back to main engine.
 *the logic flow is the same as the CodeType struct order. */
int Board::validateAndPerformAction(const std::string& action)
{
	MoveIndices move = parseAction(action);

	Piece* piece = getPiece(move.source);
	if (!piece)
		return (int)CodeType::CT_NO_PAWN;
	if (piece->isWhite() != m_isWhiteTurn)
		return (int)CodeType::CT_ENEMY_PAWN;
	if (!piece->checkMoveRange(move.source, move.destination, *this))
		return (int)CodeType::CT_ILLEGAL_MOVE;

	Piece* destPiece = getPiece(move.destination);
	if (destPiece && piece->isWhite() == destPiece->isWhite())
		return (int)CodeType::CT_ALLY_PAWN;

	std::unique_ptr<Piece> capturedPiece = simulateMove(move.source, move.destination);

	if (isKingInCheck(piece->isWhite()))
	{
		undoMove(move.source, move.destination, capturedPiece);
		return (int)CodeType::CT_CHECK_OWN;
	}

	m_isWhiteTurn = !m_isWhiteTurn;

	if (isKingInCheck(m_isWhiteTurn))
		return (int)CodeType::CT_LEGAL_CHECK_MOVE;
	else
		return (int)CodeType::CT_LEGAL_MOVE;
}

Piece* Board::getPiece(indexPair index) const
{
	return m_Board[index.row][index.col].get();
}

/// Returns true if the path between source and destination is clear of pieces that my interfer.
bool Board::isPathClear(const indexPair& source, const indexPair& destination) const {
	int rowDiff = destination.row - source.row;
	int colDiff = destination.col - source.col;

	int rowStep = rowDiff == 0 ? 0 : rowDiff / abs(rowDiff);
	int colStep = colDiff == 0 ? 0 : colDiff / abs(colDiff);

	int currRow = source.row + rowStep;
	int currCol = source.col + colStep;

	while (currRow != destination.row || currCol != destination.col) {
		if (m_Board[currRow][currCol]) return false;
		currRow += rowStep;
		currCol += colStep;
	}
	return true;
}


//===============================PRIVATE-METHODS===============================
void Board::initializeBoard(const std::string& players)
{
	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col < BOARD_SIZE; ++col)
		{
			const char pieceChar = players[row * BOARD_SIZE + col];
			m_Board[row][col] = std::move(PieceFactory::createPiece(std::string(1, pieceChar)));
			initializeKing(row, col);
		}
	}
}

// If this slot is not null and is a king, store its pointer and position.
void Board::initializeKing(int row, int col)
{
	Piece* piece = m_Board[row][col].get();
	if (piece && piece->isKing())
	{
		if (piece->isWhite())
		{
			m_whiteKing = piece;
			m_whiteKingPos = { row, col };
		}
		else
		{
			m_blackKing = piece;
			m_blackKingPos = { row, col };
		}
	}
}

void Board::movePiece(indexPair source, indexPair destination)
{
	m_Board[destination.row][destination.col] = std::move(m_Board[source.row][source.col]);
	m_Board[source.row][source.col] = nullptr;
}

indexPair Board::findKingPosition(bool isWhite) const
{
	return isWhite ? m_whiteKingPos : m_blackKingPos;
}

/// Returns true if the king of the given color is under attack.
bool Board::isKingInCheck(bool isWhite) const
{
	indexPair kingPos = findKingPosition(isWhite);

	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col < BOARD_SIZE; ++col)
		{
			Piece* enemy = m_Board[row][col].get();
			if (enemy && enemy->isWhite() != isWhite)
			{
				indexPair pos = { row, col };
				if (enemy->checkMoveRange(pos, kingPos, *this))
					return true;
			}
		}
	}
	return false;
}

/// Simulates a move by moving the piece from source to destination.
std::unique_ptr<Piece> Board::simulateMove(const indexPair& source, const indexPair& destination)
{
	std::unique_ptr<Piece> captured = std::move(m_Board[destination.row][destination.col]);
	if (m_Board[source.row][source.col]->isKing())
	{
		setKingPosition(m_Board[source.row][source.col]->isWhite(), destination);
	}
	m_Board[destination.row][destination.col] = std::move(m_Board[source.row][source.col]);
	return captured;
}


void Board::setKingPosition(bool isWhite, const indexPair& position)
{
	if (isWhite)
		m_whiteKingPos = position;
	else
		m_blackKingPos = position;
}

/// Restores the board to its previous state by moving the piece back to its original position
void Board::undoMove(const indexPair& source, const indexPair& destination, std::unique_ptr<Piece>& capturedPiece)
{
	// Retrieve the moved piece from the destination
	std::unique_ptr<Piece> movedPiece = std::move(m_Board[destination.row][destination.col]);

	// If the moved piece is a king, restore its old position (source)
	if (movedPiece && movedPiece->isKing())
	{
		setKingPosition(movedPiece->isWhite(), source);
	}

	// Move the piece back to the source square
	m_Board[source.row][source.col] = std::move(movedPiece);

	// Restore any captured piece back to the destination
	m_Board[destination.row][destination.col] = std::move(capturedPiece);
}



/// Parses a 4-character action string into source and destination indices.
MoveIndices Board::parseAction(const std::string& action) const
{
	MoveIndices mi = { indexPair{ Utility::charToIntBoard(action[0]), Utility::stringToInt(action[1])} ,
		indexPair{ Utility::charToIntBoard(action[2]), Utility::stringToInt(action[3]) }};
	return mi;
}