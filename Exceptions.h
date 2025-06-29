
#pragma once
#include <exception>

class InvalidMoveException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid move attempted!";
    }
};

class EmptyQueueException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Priority queue is empty!";
    }
};
