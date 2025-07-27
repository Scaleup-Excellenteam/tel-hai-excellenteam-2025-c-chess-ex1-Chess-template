// Chess/test/GameTests.cpp
#include <iostream>
#include <cassert> // For simple assertions
#include <string>
#include <vector>
#include <memory>
#include <tuple>   // For std::tuple

// Include necessary game headers
#include "../include/GameEngine.h"
#include "../include/Board.h"
#include "../include/Utils/CMove.h"
#include "../include/Pieces/Pawn.h"
#include "../include/Pieces/Rook.h"
#include "../include/Pieces/King.h"
#include "../include/Pieces/Queen.h"
#include "../include/Pieces/Bishop.h"
#include "../include/Pieces/Knight.h"
#include "../include/Utils/Colors.h" // For piece symbols and output colors
#include <sstream> // For std::istringstream

// Helper function to set up a custom board state for tests
void setupCustomBoard(GameEngine& engine, const std::vector<std::tuple<int, int, Piece*>>& pieces, bool whiteToMove = true) {
    engine.resetGameForTest(); // Clear board and reset basic game state

    for (const auto& p_tuple : pieces) {
        int r = std::get<0>(p_tuple);
        int c = std::get<1>(p_tuple);
        Piece* piece_raw = std::get<2>(p_tuple);
        engine.setPieceForTest(r, c, piece_raw); // Use the new public setter
    }
    engine.setTurnForTest(whiteToMove); // Set initial turn for the test
    engine.setHalfMoveClockForTest(0); // Reset half-move clock
    engine.clearBoardHistoryForTest(); // Clear history
    engine.addBoardStateToHistoryForTest(); // Add initial state to history for repetition test
}


// --- Test Functions ---

void testCastling() {
    std::cout << Colors::INFO_COLOR << "--- Running Castling Tests ---" << Colors::RESET << std::endl;

    // Test White Kingside Castling
    {
        GameEngine engine;
        setupCustomBoard(engine, {
            {0, 4, new King(false)}, // Black King
            {7, 4, new King(true)},  // White King
            {7, 7, new Rook(true)}   // White Kingside Rook (H8)
        });
        engine.setTurnForTest(true); // Ensure it's White's turn
        engine.makeMove("h5h7"); // King moves from H5 (7,4) to H7 (7,6)

        assert(engine.currentBoard().getPiece(7, 6) != nullptr && dynamic_cast<King*>(engine.currentBoard().getPiece(7, 6)) != nullptr && engine.currentBoard().getPiece(7, 6)->getIsWhite()); // King at H7
        assert(engine.currentBoard().getPiece(7, 5) != nullptr && dynamic_cast<Rook*>(engine.currentBoard().getPiece(7, 5)) != nullptr && engine.currentBoard().getPiece(7, 5)->getIsWhite()); // Rook at H6
        assert(engine.currentBoard().getPiece(7, 4) == nullptr); // Old King position empty
        assert(engine.currentBoard().getPiece(7, 7) == nullptr); // Old Rook position empty
        std::cout << Colors::GREEN << "✔ White Kingside Castling Passed." << Colors::RESET << std::endl;
    }

    // Test Black Queenside Castling
    {
        GameEngine engine;
        setupCustomBoard(engine, {
            {0, 4, new King(false)},  // Black King
            {7, 4, new King(true)},   // White King
            {0, 0, new Rook(false)}   // Black Queenside Rook (A1)
        }, false); // Start as Black's turn
        engine.makeMove("a5a3"); // King moves from A5 (0,4) to A3 (0,2)

        assert(engine.currentBoard().getPiece(0, 2) != nullptr && dynamic_cast<King*>(engine.currentBoard().getPiece(0, 2)) != nullptr && !engine.currentBoard().getPiece(0, 2)->getIsWhite()); // King at A3
        assert(engine.currentBoard().getPiece(0, 3) != nullptr && dynamic_cast<Rook*>(engine.currentBoard().getPiece(0, 3)) != nullptr && !engine.currentBoard().getPiece(0, 3)->getIsWhite()); // Rook at A4
        assert(engine.currentBoard().getPiece(0, 4) == nullptr); // Old King position empty
        assert(engine.currentBoard().getPiece(0, 0) == nullptr); // Old Rook position empty
        std::cout << Colors::GREEN << "✔ Black Queenside Castling Passed." << Colors::RESET << std::endl;
    }
    std::cout << Colors::INFO_COLOR << "--- Castling Tests Complete ---" << Colors::RESET << std::endl << std::endl;
}

void testPawnPromotion() {
    std::cout << Colors::INFO_COLOR << "--- Running Pawn Promotion Tests ---" << Colors::RESET << std::endl;

    // Test White Pawn Promotion to Queen (Human Player)
    {
        GameEngine engine;
        setupCustomBoard(engine, {
            {0, 4, new King(false)}, // Black King
            {7, 4, new King(true)},  // White King
            {1, 6, new Pawn(true)}   // White Pawn at B7 (row 1, col 6) (one step before promotion)
        });
        engine.setTurnForTest(true); // Ensure it's White's turn

        // Simulate user input for 'Q'
        std::streambuf* origCin = std::cin.rdbuf(); // Save original cin buffer
        std::istringstream testInput("Q\n");
        std::cin.rdbuf(testInput.rdbuf()); // Redirect cin to testInput

        engine.makeMove("b7a7"); // Move pawn from B7 to A7 (row 0, col 6), triggering promotion

        std::cin.rdbuf(origCin); // Restore original cin buffer

        assert(engine.currentBoard().getPiece(0, 6) != nullptr && dynamic_cast<Queen*>(engine.currentBoard().getPiece(0, 6)) != nullptr && engine.currentBoard().getPiece(0, 6)->getIsWhite()); // Queen at A7
        std::cout << Colors::GREEN << "✔ White Pawn Promotion (Human) Passed." << Colors::RESET << std::endl;
    }

    // Test Black Pawn Promotion to Queen (AI Player)
    {
        GameEngine engine;
        setupCustomBoard(engine, {
            {0, 4, new King(false)}, // Black King
            {7, 4, new King(true)},  // White King
            {6, 1, new Pawn(false)}  // Black Pawn at G2 (row 6, col 1) (one step before promotion)
        }, false); // Start as Black's turn
        engine.setPlayAgainstAIForTest(true); // Enable AI for this test

        engine.makeMove("g2h2"); // Move pawn from G2 to H2 (row 7, col 1), triggering promotion

        assert(engine.currentBoard().getPiece(7, 1) != nullptr && dynamic_cast<Queen*>(engine.currentBoard().getPiece(7, 1)) != nullptr && !engine.currentBoard().getPiece(7, 1)->getIsWhite()); // Queen at H2
        std::cout << Colors::GREEN << "✔ Black Pawn Promotion (AI) Passed." << Colors::RESET << std::endl;
    }
    std::cout << Colors::INFO_COLOR << "--- Pawn Promotion Tests Complete ---" << Colors::RESET << std::endl << std::endl;
}

void testEnPassant() {
    std::cout << Colors::INFO_COLOR << "--- Running En Passant Tests ---" << Colors::RESET << std::endl;

    // Test White Captures Black En Passant
    {
        GameEngine engine;
        // Setup board: White pawn at D2, Black pawn at D1 (just moved there)
        setupCustomBoard(engine, {
            {0, 4, new King(false)}, // Black King
            {7, 4, new King(true)},  // White King
            {3, 1, new Pawn(true)},  // White Pawn at D2 (row 3, col 1)
            {3, 0, new Pawn(false)}  // Black Pawn at D1 (row 3, col 0) - this pawn is the one to be captured
        });
        // Manually set lastMove for the board to simulate Black's double move to D1
        engine.setLastMoveForTest(CMove(1, 0, 3, 0)); // Black pawn from B1 (1,0) to D1 (3,0)
        engine.setTurnForTest(true); // Ensure it's White's turn for capture

        engine.makeMove("d2c1"); // White pawn from D2 (3,1) captures D1, lands at C1 (2,0)

        assert(engine.currentBoard().getPiece(2, 0) != nullptr && dynamic_cast<Pawn*>(engine.currentBoard().getPiece(2, 0)) != nullptr && engine.currentBoard().getPiece(2, 0)->getIsWhite()); // White pawn at C1
        assert(engine.currentBoard().getPiece(3, 0) == nullptr); // Black pawn at D1 (captured square) should be empty
        std::cout << Colors::GREEN << "✔ White En Passant Capture Passed." << Colors::RESET << std::endl;
    }

    // Test Black Captures White En Passant
    {
        GameEngine engine;
        // Setup board: Black pawn at E2, White pawn at E1 (just moved there)
        setupCustomBoard(engine, {
            {0, 4, new King(false)}, // Black King
            {7, 4, new King(true)},  // White King
            {4, 1, new Pawn(false)}, // Black Pawn at E2 (row 4, col 1)
            {4, 0, new Pawn(true)}   // White Pawn at E1 (row 4, col 0) - this pawn is the one to be captured
        }, false); // Start as Black's turn
        // Manually set lastMove for the board to simulate White's double move to E1
        engine.setLastMoveForTest(CMove(6, 0, 4, 0)); // White pawn from G1 (6,0) to E1 (4,0)
        engine.setTurnForTest(false); // Ensure it's Black's turn for capture

        engine.makeMove("e2f1"); // Black pawn from E2 (4,1) captures E1, lands at F1 (5,0)

        assert(engine.currentBoard().getPiece(5, 0) != nullptr && dynamic_cast<Pawn*>(engine.currentBoard().getPiece(5, 0)) != nullptr && !engine.currentBoard().getPiece(5, 0)->getIsWhite()); // Black pawn at F1
        assert(engine.currentBoard().getPiece(4, 0) == nullptr); // White pawn at E1 (captured square) should be empty
        std::cout << Colors::GREEN << "✔ Black En Passant Capture Passed." << Colors::RESET << std::endl;
    }
    std::cout << Colors::INFO_COLOR << "--- En Passant Tests Complete ---" << Colors::RESET << std::endl << std::endl;
}

void testFiftyMoveRule() {
    std::cout << Colors::INFO_COLOR << "--- Running Fifty-move Rule Test ---" << Colors::RESET << std::endl;

    GameEngine engine;
    // Set up a board with only Kings to ensure no pawn moves or captures
    setupCustomBoard(engine, {
        {0, 4, new King(false)}, // Black King
        {7, 4, new King(true)}   // White King
    });
    engine.setHalfMoveClockForTest(99); // Set clock to 99 (after 99 half-moves)

    // Next move will be the 100th half-move.
    engine.makeMove("h5h6"); // White King moves (100th half-move)
    assert(engine.isFiftyMoveDraw()); // Should be a draw now!
    std::cout << Colors::GREEN << "✔ Fifty-move Rule Test Passed." << Colors::RESET << std::endl;
    std::cout << Colors::INFO_COLOR << "--- Fifty-move Rule Test Complete ---" << Colors::RESET << std::endl << std::endl;
}


void testThreefoldRepetition() {
    std::cout << Colors::INFO_COLOR << "--- Running Threefold Repetition Test ---" << Colors::RESET << std::endl;

    GameEngine engine;
    // Set up a simple board to easily repeat moves
    setupCustomBoard(engine, {
        {0, 4, new King(false)}, // Black King
        {7, 4, new King(true)},  // White King
        {6, 1, new Knight(true)}, // White Knight at G2
        {1, 6, new Knight(false)} // Black Knight at B7
    });

    // Sequence of 2 moves that returns to original position after 4 half-moves
    // Initial state: S1 (count 1 from setupCustomBoard)
    // Move 1 (W): G2-F3 (Knight) -> S2
    // Move 2 (B): B7-C6 (Knight) -> S3
    // Move 3 (W): F3-G2 (Knight) -> S1 (2nd occurrence)
    // Move 4 (B): C6-B7 (Knight) -> S2 (2nd occurrence)
    // Move 5 (W): G2-F3 (Knight) -> S3 (2nd occurrence)
    // Move 6 (B): B7-C6 (Knight) -> S1 (3rd occurrence) - Draw!

    // The setupCustomBoard adds the initial state once.
    // So we need 2 more occurrences.

    engine.makeMove("g2f3"); // S2
    engine.makeMove("b7c6"); // S3
    assert(!engine.isThreefoldRepetition()); // Should not be draw yet

    engine.makeMove("f3g2"); // S1 (2nd occurrence)
    assert(!engine.isThreefoldRepetition()); // Should not be draw yet

    engine.makeMove("c6b7"); // S2 (2nd occurrence)
    assert(!engine.isThreefoldRepetition()); // Should not be draw yet

    engine.makeMove("g2f3"); // S3 (2nd occurrence)
    assert(!engine.isThreefoldRepetition()); // Should not be draw yet

    engine.makeMove("b7c6"); // S1 (3rd occurrence) - This move creates the third occurrence of S1 state.
    assert(engine.isThreefoldRepetition()); // Should be draw now!
    std::cout << Colors::GREEN << "✔ Threefold Repetition Test Passed." << Colors::RESET << std::endl;
    std::cout << Colors::INFO_COLOR << "--- Threefold Repetition Test Complete ---" << Colors::RESET << std::endl << std::endl;
}


// Main function to run all tests
int main() {
    std::cout << Colors::BOLD << Colors::CYAN << "Starting Chess Game Special Operation Tests" << Colors::RESET << std::endl;
    std::cout << "---------------------------------------------" << std::endl << std::endl;

    testCastling();
    testPawnPromotion();
    testEnPassant();
    testFiftyMoveRule();
    testThreefoldRepetition();

    std::cout << Colors::BOLD << Colors::GREEN << "All special operation tests completed successfully!" << Colors::RESET << std::endl;
    return 0;
}