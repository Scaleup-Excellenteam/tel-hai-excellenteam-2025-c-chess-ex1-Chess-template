#define FIRST_COLL 'a'               ///< First valid column on the chessboard
#define LAST_COLL 'h'                ///< Last valid column on the chessboard
#define FIRST_ROW 1                  ///< First valid row on the chessboard
#define LAST_ROW 8                   ///< Last valid row on the chessboard
#define BOARD_CELL_NUMBER 64         ///< Total number of cells on an 8x8 chessboard
#define ROW_SIZE 8                   ///< Number of columns or rows on a chessboard

#define ERROR_NO_PIECE_AT_SOURCE 11  ///< No piece at source coordinate
#define ERROR_WRONG_COLOR_AT_SOURCE 12 ///< Piece at source is of wrong color
#define ERROR_TARGET_HAS_FRIENDLY 13 ///< Target square has a friendly piece
#define ERROR_ILLEGAL_MOVE 21        ///< Illegal move according to piece rules
#define ERROR_MOVE_CAUSES_CHECK 31   ///< Move results in self-check

#define STATUS_CHECK 41              ///< Move places opponent in check
#define STATUS_OK 42                 ///< Move is valid

#define COLL_PLACE_HOLDER 'x'        ///< Placeholder for invalid column
#define ROW_PLACE_HOLDER (-1)        ///< Placeholder for invalid row
