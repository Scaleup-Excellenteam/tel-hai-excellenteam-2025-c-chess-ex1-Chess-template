#pragma once

#include <string>


namespace Colors {
    const std::string PURPLE = "\033[38;5;135m";  // Deep purple
    const std::string GREEN = "\033[38;5;114m";   // Pastel green
    const std::string ORANGE = "\033[38;5;215m";  // Pastel orange
    const std::string CYAN = "\033[38;5;80m";     // Deep cyan
    const std::string PINK = "\033[38;5;219m";    // Soft pink
    const std::string YELLOW = "\033[38;5;222m";  // Pastel yellow
    const std::string BLUE = "\033[38;5;111m";    // Soft blue - Keeping for potential future use

    // Utility colors
    const std::string RED = "\033[38;5;167m";     // Deep red for errors
    const std::string RESET = "\033[0m";          // Reset all formatting
    const std::string BOLD = "\033[1m";
    const std::string DIM = "\033[2m";

    /* ~~~
       Informational messages:
       These colors are used for system-related messages like startup messages,
       main menus, user prompts, and exit confirmations.
    ~~~ */
    const std::string INFO_COLOR = CYAN;    // System messages, general info
    const std::string MENU_COLOR = CYAN;    // Main menus, user prompts

    // Chess piece colors
    const std::string WHITE_PIECE = YELLOW; 
    const std::string BLACK_PIECE = PURPLE; 
    const std::string BOARD_FRAME = ORANGE; 
    const std::string COORD_COLOR = GREEN;  
    const std::string HINT_COLOR = PINK;    

    // Unicode chess pieces
    namespace Pieces {
        // White Unicode pieces filled characters - typically used for Black in diagrams,
        // but often used for White in console for better visibility
        const std::string WHITE_KING   = "♔"; //
        const std::string WHITE_QUEEN  = "♕"; //
        const std::string WHITE_ROOK   = "♖"; //
        const std::string WHITE_BISHOP = "♗"; //
        const std::string WHITE_KNIGHT = "♘"; //
        const std::string WHITE_PAWN   = "♙"; //

        // Black Unicode pieces - outlined characters - typically used for White in diagrams,
        // but often used for Black in console for better visibility
        const std::string BLACK_KING   = "♚"; //
        const std::string BLACK_QUEEN  = "♛"; //
        const std::string BLACK_ROOK   = "♜"; //
        const std::string BLACK_BISHOP = "♝"; //
        const std::string BLACK_KNIGHT = "♞"; //
        const std::string BLACK_PAWN   = "♟"; //
    }
}