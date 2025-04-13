=====================================CHESS-EX1========================================
By:Dima Nikonov
ID: dimaku@edu.jmc.ac.il

Description:
This project implements a basic chess engine in C++ and consists of three main parts:

*Board: Handles game state, move validation, and simulates moves.

*Piece: An abstract class that defines the common interface for all chess pieces.

*PieceFactory: Creates specific chess pieces based on a character identifier.

Classes structures:

**Board class**
The Board class manages the 8x8 chessboard and is used directly in main.

It builds the board based on a string of characters, each representing a piece.

Validates Moves, The method validateAndPerformAction takes a four-character action
(source and destination squares), checks if the move is legal, simulates it, and then
returns a response code.

Simulates & Undoes Moves: It temporarily performs moves to check if a king would remain in check, and can undo them if necessary. This includes updating the king's position if he moves.

**Piece class**
is the foundation for all chess pieces. Key points:

Abstract Interface: It requires derived classes (like Knight) to implement
checkMoveRange that defines valid moves.

Piece Info: Stores whether the piece is white or not.

Self-Registration: Each specific piece (for example, Knight) registers itself with PieceFactory using a static flag, so the factory can later create the correct
piece type based on a given character.

----All pieces for the chess game have been implemented.----

**PieceFactory class**
The PieceFactory is used to create pieces when building the board.
It maps characters (like "N" for Knight) to functions that instantiate the correct piece type.

**Response Codes**
When a move is validated, the Board returns a code (using CodeType) that tells you what happened (defined in the Board class):

-CT_NO_PAWN: Nothing is at the source.

-CT_ENEMY_PAWN: The piece belongs to the opponent.

-CT_ALLY_PAWN: The destination holds a friendly piece.

-CT_ILLEGAL_MOVE: The move doesn't follow the piece's rules.

-CT_CHECK_OWN: The move leaves the king in check.

-CT_LEGAL_CHECK_MOVE: The move is legal and puts the enemy king in check.

-CT_LEGAL_MOVE: The move is legal and safe.

======================================================================================