#ifndef INVALID_DEPTH_EXCEPTION_H
#define INVALID_DEPTH_EXCEPTION_H

#include <exception>

class InvalidDepthException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid depth selected. Please choose a number greater then 0.";
    }
};

#endif // INVALID_DEPTH_EXCEPTION_H
