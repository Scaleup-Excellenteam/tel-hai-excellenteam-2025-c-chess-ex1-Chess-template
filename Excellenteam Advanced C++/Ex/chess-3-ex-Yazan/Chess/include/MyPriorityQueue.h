#ifndef CHESS_EX_YAZANHEIB_MYPRIORITYQUEUE_H
#define CHESS_EX_YAZANHEIB_MYPRIORITYQUEUE_H

#include <queue>
#include <mutex>
#include <vector>

using namespace std;


template<typename T>
class MyPriorityQueue {

private:

    priority_queue <T> myQueue;
    mutex myMtx;

public:

    /**
     * Constructor That Will Initialize A Empty Priority Queue.
     */
    MyPriorityQueue() {}

    
    /**
     * Destructor.
     */
    ~MyPriorityQueue() {}


    /**
     * Push An Element To The Priority Queue.
     * @param value The Element That Will Be Pushed To The Queue.
     */
    void push(const T &value)
    {
        // Lock The Queue, And Push The Value To The Queue.
        lock_guard <mutex> lock(myMtx);
        myQueue.push(value);
    }


    /**
     * Try To Pop The Top Element From The Queue.
     * @param value Will Hold That Value Will Be Pop, If The Queue If Not Empty.
     * @return True If The Value Popped, And False Otherwise.
     */
    bool tryPop(T &value)
    {
        // Lock The Queue.
        lock_guard <mutex> lock(myMtx);

        // And Check If The Queue Is Not Empty, And Pop The Parameter Element From the Queue.
        if (!myQueue.empty())
        {
            value = myQueue.top();
            myQueue.pop();
            return true;
        }
        // Otherwise Return False.
        return false;
    }


    /**
     * At This Method Will Get The Top Element
     * @return Reference To The Top Element.
     */
    T top()
    {
        // Lock The Queue, And get The Top Value From To The Queue.
        lock_guard <mutex> lock(myMtx);
        return myQueue.top();
    }


    /**
     * At This Method Will Check And Return If The Queue Is Empty.
     * @return True If The Queue Is Empty, Otherwise False.
     */
    bool isEmpty()
    {
        // Lock The Queue, And Check If The Queue Is Empty.
        lock_guard <mutex> lock(myMtx);
        return myQueue.empty();
    }

};


#endif //CHESS_EX_YAZANHEIB_MYPRIORITYQUEUE_H

