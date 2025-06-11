#ifndef EMPTYQUEUEEXCEPTION_H
#define EMPTYQUEUEEXCEPTION_H

#include <exception>

class EmptyQueueException : public std::exception {
public:
    const char* what() const noexcept override {
        return "PriorityQueue is empty";
    }
};

#endif // EMPTYQUEUEEXCEPTION_H