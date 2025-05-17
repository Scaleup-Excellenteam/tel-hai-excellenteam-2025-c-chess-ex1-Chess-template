//
// Created by HALAA on 14/05/2025.
//

#ifndef CHESS_INVALIDSUICIDEEXCEPTION_H
#define CHESS_INVALIDSUICIDEEXCEPTION_H
#include <exception>
#include <string>

class InvalidSuicideException : public std::exception {
private:
    std::string message;
public:
    InvalidSuicideException(const std::string& msg = "Invalid move: this move puts or leaves your king in check.")
            : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif //CHESS_INVALIDSUICIDEEXCEPTION_H
