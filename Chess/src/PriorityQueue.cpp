#include "../include/PriorityQueue.h"


std::ostream& operator<<(std::ostream& os, const Move& move) {
    os << static_cast<char>('a' + move.fromRow) 
       << static_cast<char>('1' + move.fromCol) 
       << " "
       << static_cast<char>('a' + move.toRow) 
       << static_cast<char>('1' + move.toCol);
    return os;
}

bool PriorityQueue::isEmpty() const {
    return data.empty();
}

void PriorityQueue::push(const Move& move, int score) {
    MoveScore newMove = { move, score };
    auto it = data.begin();
    
    while (it != data.end() && cmp(it->score, newMove.score) < 0) {
        ++it;
    }
    data.insert(it, newMove);

    if (data.size() > 5) {
        data.pop_back(); 
    }
}

MoveScore PriorityQueue::pull() {
    if (data.empty()) {
        throw EmptyQueueException(); 
    }
    
    MoveScore top = data.front();  
    data.pop_front();              
    return top;                 
}

int PriorityQueue::size() const {
    return data.size();
}
