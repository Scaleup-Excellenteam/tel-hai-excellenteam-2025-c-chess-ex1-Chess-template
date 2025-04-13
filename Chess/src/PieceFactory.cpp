#include "PieceFactory.h"
#include <algorithm>
#include "Utility.h"
#include "Rook.h"

bool PieceFactory::registerPiece(const std::string& name, pieceFunction fnct)
{
	auto result = getPieceMap().emplace(name, fnct);
	if (result.second) {
		return true;
	}
	else
	{
		throw std::invalid_argument("Registration failed");
	}
}

std::unique_ptr<Piece> PieceFactory::createPiece(const std::string& name)
{
	std::cout << getPieceMap().size() << std::endl;
	bool isWhite = std::isupper(name[0]);
	char pieceToUpper = Utility::stringToUpperChar(name[0]);

	auto it = getPieceMap().find(std::string(1, pieceToUpper));

	if (it == getPieceMap().end()) { return nullptr; }
	std::cout << "Creating piece: " << name + " upper " + pieceToUpper << std::endl;

	return it->second(isWhite);
}
