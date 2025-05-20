#include "PriorityQueue.h"
#include <algorithm>
#include <stdexcept>

template <class T>
int Comparator<T>::operator()(const T &l, const T &r)
{
    return r - l;
}

template <class T, class Comp>
T PriorityQueue<T, Comp>::poll()
{
    if (queue.empty()) {
        throw std::runtime_error("empty list");
    }
    T ret = queue.front();
    queue.pop_front();
    return ret;
}

template <class T, class Comp>
void PriorityQueue<T, Comp>::push(const T &item)
{
    Comp comp;
    queue.insert(
        std::find_if(queue.begin(), queue.end(),
                     [&item, &comp](const T &r) { return comp(item, r) > 0; }),
        item);
}
