#ifndef CHESS_PRIORITYQUEUE_H
#define CHESS_PRIORITYQUEUE_H
#include <list>
#include <cstddef>
#include <mutex>

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
    mutable std::mutex mtx;
public:
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
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
        std::lock_guard<std::mutex> lock(mtx);
        return queue.front();
    }
    T poll() {
        std::lock_guard<std::mutex> lock(mtx);
        T top = queue.front();
        queue.pop_front();
        return top;
    }
    void popWorst() {
        std::lock_guard<std::mutex> lock(mtx);
        queue.pop_back();
    }
    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        queue.clear();
    }
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }
};


#endif //CHESS_PRIORITYQUEUE_H
