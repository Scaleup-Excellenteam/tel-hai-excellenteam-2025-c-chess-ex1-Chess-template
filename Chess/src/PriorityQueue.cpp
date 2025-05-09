#include "../include/PriorityQueue.h"


bool PriorityQueue::isEmpty() const {
     return data.empty();
}

void PriorityQueue::push(int value){
     auto it = data.begin();
     while (it != data.end() && cmp(*it, value) < 0) {
         ++it;
     }
    data.insert(it, value);
}

int PriorityQueue::pull() {
    if (data.empty()) {
        throw EmptyQueueException();  
    }
    int top = data.front();
    data.pop_front();
    return top;
}