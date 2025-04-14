#include "Common.h"
#include "Constants.h"

// Compares two coordinates for equality.
// @param c1 The first coordinate to compare.
// @param c2 The second coordinate to compare.
// @return True if both coordinates are equal, false otherwise.
bool operator==(const Coordinate& c1, const Coordinate& c2) {
    return (c1.first == c2.first && c1.second == c2.second);
}

// Checks if a given coordinate is within the bounds of the chessboard.
// @param location The coordinate to check.
// @return True if the coordinate is within the valid range on the board (within rows and columns),
//         false if the coordinate is out of bounds.
bool inBoard(const Coordinate &location) {
    char coll = location.second;
    int row = location.first;
    return (coll >= FIRST_COLL &&
            coll <= LAST_COLL &&
            row >= FIRST_ROW &&
            row <= LAST_ROW);
}

// Returns the opposite color to the given color.
// @param color The color to get the opposite of (either WHITE or BLACK).
// @return The opposite color (BLACK if WHITE is given, and WHITE if BLACK is given).
COLOR colorNot(COLOR color) {
    return (color == WHITE) ? BLACK : WHITE;
}

// Converts a chess coordinate from standard chess notation (e.g., "a1", "d5") to a custom format.
// @param coord The coordinate in the original chess notation format.
// @return A coordinate with flipped row and column, converting chess notation to a custom coordinate format.
Coordinate flipChessCoord(const Coordinate& coord) {
    char coll = static_cast<char>('a' + (coord.first - 1)); // Converts numeric row to char column
    int row = static_cast<int>(tolower(coord.second) - 'a' + 1); // Converts char column to numeric row
    return {row, coll}; // Returns a flipped coordinate
}
