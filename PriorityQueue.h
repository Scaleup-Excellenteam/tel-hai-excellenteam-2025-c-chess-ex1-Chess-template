
#pragma once
#include <list>
#include <stdexcept>

template<typename T, typename Comparator>
class PriorityQueue {
private:
    std::list<T> _list;
    Comparator comp;

public:
    void push(const T& value) {
        auto it = _list.begin();
        while (it != _list.end() && comp(*it, value) < 0) {
            ++it;
        }
        _list.insert(it, value);
    }

    T poll() {
        if (_list.empty()) {
            throw std::runtime_error("Polling from empty queue");
        }
        T val = _list.front();
        _list.pop_front();
        return val;
    }

    bool empty() const { return _list.empty(); }
    size_t size() const { return _list.size(); }

    typename std::list<T>::const_iterator begin() const { return _list.begin(); }
    typename std::list<T>::const_iterator end() const { return _list.end(); }
};
