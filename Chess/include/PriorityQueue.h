//
// Created by GUYYY on 09/05/2025.
//

#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <list>
#include <stdexcept>

template <typename T>
struct DefaultComparator {
    int operator()(const T& a, const T& b) const {
        if (a > b) return 1;
        if (a < b) return -1;
        return 0;
    }
};


template <typename T, typename Comparator = DefaultComparator<T>>
class PriorityQueue {
private:
    std::list<T> data;
    Comparator comp;

public:
    void push(const T& value) {
        auto it = data.begin();
        for (; it != data.end(); ++it) {
            if (comp(value, *it) > 0) {
                break;
            }
        }
        data.insert(it, value);
    }

    T pull() {
        if (data.empty()) {
            throw std::runtime_error("PriorityQueue is empty");
        }
        T top = data.front();
        data.pop_front();
        return top;
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};


#endif //PRIORITYQUEUE_H
