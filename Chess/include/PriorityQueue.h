#include <list>
template <class T>
struct Comparator {
    int operator() (const T& l, const T& r);
};

template <class T, class Comp = Comparator<T> >
class PriorityQueue
{
private:
    std::list<T> queue;

public:
    T poll();
    void push(const T &item);
};
