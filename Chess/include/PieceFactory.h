#pragma once

#include "Piece.h"
#include <map>
#include <memory>
#include <functional>

/*
* File: PieceFactory.h
* Description: Header file for the PieceFactory class, which is responsible for creating chess pieces.
*			   It uses a static map to register and create pieces based on their names.
*/

class PieceFactory
{
public:
	using pieceFunction = std::function<std::unique_ptr<Piece>(bool isWhite)>;
	using pieceMap = std::map<std::string, pieceFunction>;

	static bool registerPiece(const std::string& name, pieceFunction);
	static std::unique_ptr<Piece> createPiece(const std::string& name);

private:
	static pieceMap& getPieceMap()
	{
		static pieceMap m_map;
		return m_map;
	}
};