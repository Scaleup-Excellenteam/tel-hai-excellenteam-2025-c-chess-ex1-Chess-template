//
// Created by HALAA on 14/05/2025.
//

#ifndef CHESS_INVALIDPROMOTIONEXCEPTION_H
#define CHESS_INVALIDPROMOTIONEXCEPTION_H
#include <exception>
#include <string>

class InvalidPromotionException : public std::exception {
private:
    std::string message;
public:
    InvalidPromotionException(const std::string& msg = "Invalid promotion choice.")
            : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};
#endif //CHESS_INVALIDPROMOTIONEXCEPTION_H
