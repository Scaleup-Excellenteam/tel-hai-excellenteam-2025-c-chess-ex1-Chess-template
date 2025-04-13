#include "Utility.h"

namespace Utility {
    const char stringToUpperChar(char c)
    {
        if (c >= 'a' && c <= 'z') {return c - ('a' - 'A');}
        else if (c >= 'A' && c <= 'Z') { return c; }
    }

    const int charToIntBoard(char c)
    {
        if (c >= 'a' && c <= 'h') {
            return c - 'a';
        }
        else if (c >= 'A' && c <= 'H') {
            return c - 'A';
        }
        return -1;
    }

    const int stringToInt(char c)
    {
        int result = 0;
        if (c >= '1' && c <= '8') {
            return c - '1';
        }
        else {return -1;}
    }
}
