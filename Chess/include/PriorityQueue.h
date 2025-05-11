#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "../include/exceptions/EmptyQueueException.h"
#include <list>
#include <string>
#include <iostream>


struct Move {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
};

std::ostream& operator<<(std::ostream& os, const Move& move);

struct MoveScore { 
    Move move;  
    int score; 
};

template <typename T>
struct MyComparator { 
    int operator()(const T& a, const T& b) const { 
        return a - b;  
    } 
};


class PriorityQueue {
private:
    std::list<MoveScore> data;
    MyComparator<int> cmp;

public:
    PriorityQueue() = default;
    ~PriorityQueue() = default;
    
    void push(const Move& move, int score);
    MoveScore pull();
    bool isEmpty() const;
    int size() const;
};

#endif // PRIORITYQUEUE_H
