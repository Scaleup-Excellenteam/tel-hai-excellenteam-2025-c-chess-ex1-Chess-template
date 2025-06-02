#include "Exception_chess.h"


Exception_chess::Exception_chess(ExceptionType t) : type(t) {
    switch (t) {
        case ExceptionType::NoPieceAtSource:
            message = "There is no piece at the source location";
            break;
        case ExceptionType::OpponentPieceAtSource:
            message = "The piece at the source belongs to your opponent";
            break;
        case ExceptionType::FriendlyPieceAtDestination:
            message = "The destination contains one of your own pieces";
            break;
        case ExceptionType::InvalidPieceMovement:
            message = "Illegal movement of that piece";
            break;
        case ExceptionType::CheckViolation:
            message = "This movement would put your king in check";
            break;
    }
}

const char* Exception_chess::what() const noexcept {
    return message.c_str();
}

ExceptionType Exception_chess::getType() const noexcept {
    return type;
}