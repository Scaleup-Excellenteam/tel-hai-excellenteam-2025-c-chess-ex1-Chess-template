//
// Created by HALAA on 14/05/2025.
//

#ifndef CHESS_INVALIDMOVEBASEEXCEPTION_H
#define CHESS_INVALIDMOVEBASEEXCEPTION_H

#include <exception>
#include <string>

class InvalidMoveBaseException : public std::exception {
private:
    std::string message;
public:
    InvalidMoveBaseException(const std::string& msg = "Invalid move: this piece does not belong to you.")
            : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif //CHESS_INVALIDMOVEBASEEXCEPTION_H
