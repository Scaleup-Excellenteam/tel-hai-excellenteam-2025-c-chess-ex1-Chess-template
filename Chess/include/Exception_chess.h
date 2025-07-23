
#ifndef CHESS_EXCEPTION_CHESS_H
#define CHESS_EXCEPTION_CHESS_H

#include <exception>
#include <string>

enum class ExceptionType {
    NoPieceAtSource,
    OpponentPieceAtSource,
    FriendlyPieceAtDestination,
    InvalidPieceMovement,
    CheckViolation
};

class Exception_chess : public std::exception {
private:
    ExceptionType type;
    std::string message;

public:
    explicit Exception_chess(ExceptionType t);
    [[nodiscard]]const char* what() const noexcept override;
    [[nodiscard]]ExceptionType getType() const noexcept;
};


#endif //CHESS_EXCEPTION_CHESS_H
