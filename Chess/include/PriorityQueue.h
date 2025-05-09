#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "../include/exceptions/EmptyQueueException.h"
#include <list>


struct MyComparator { 
 int operator()(int a, int b) const { 
  return a - b; 
 } 
};

class PriorityQueue {

private:

     std::list<int> data;
     MyComparator cmp;

public:
        PriorityQueue() = default;                 // Constructor
        ~PriorityQueue() = default;                // Destructor
        
        void push(int value);         // Insert a value into the queue
        int pull();               // Remove and return the highest priority value
        bool isEmpty() const;          // Check if the queue is empty
        
        
};
#endif // PRIORITYQUEUE_H