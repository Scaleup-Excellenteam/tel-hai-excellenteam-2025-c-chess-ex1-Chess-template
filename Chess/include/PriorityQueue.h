#ifndef CHESS_PRIORITYQUEUE_H
#define CHESS_PRIORITYQUEUE_H
#include <list>
#include <cstddef>

template<typename T>
struct MyComparator {
    int operator()(const T& a, const T& b) const {
        return b.second - a.second;
    }
};

template<typename T, typename Comparator = MyComparator<T>>
class PriorityQueue {
private:
    std::list<T> queue;
    Comparator   comp;
public:
    void push(const T& value) {
        auto it = queue.begin();
        for (; it != queue.end(); ++it) {
            if (comp(*it, value) > 0)
                continue;
            else
                break;
        }
        queue.insert(it, value);
    }
    T pull() const {
        return queue.front();
    }
    T poll() {
        T top = queue.front();
        queue.pop_front();
        return top;
    }
    void popWorst() {
        queue.pop_back();
    }
    void clear() {
        queue.clear();
    }
    bool empty() const {
        return queue.empty();
    }
    std::size_t size() const {
        return queue.size();
    }
};


#endif //CHESS_PRIORITYQUEUE_H
