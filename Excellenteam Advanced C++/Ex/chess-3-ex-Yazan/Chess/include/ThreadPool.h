#ifndef CHESS_EX_YAZANHEIB_THREADPOOL_H
#define CHESS_EX_YAZANHEIB_THREADPOOL_H


#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

using namespace std;


class ThreadPool {

private:

    vector <thread> the_workers;

    queue <function<void()>> the_tasks;

    mutex queue_mutex;

    condition_variable condition;

    atomic<bool> stopier;

    size_t threads_counter;


    /**
     * At This Method Will Add A New Worker To The Pool.
     * Will Use It To Activate Process That Wait To New Tasks And Operation Them.
     */
    void addNewWorker();


    /**
     * This Method Will Be The Main Loop For Each Worker Thread.
     * And Will Wait For The New Tasks, Run Them.
     */
    void workerThreadLoop();


    /**
     * At This Method Will Notifies All Worker Threads To Stop And Get Ready To Close.
     * Will Use It To Start The Shutdown Process, By Not Accepting A New Tasks And Wake Up Any Waiting Threads.
     */
    void shutDown();


    /**
     * At This Method Will Wait For All The Threads To Finish.
     * And Join Them To Make Sure That They Have Stopped Before Continuing.
     */
    void joinAllThreads();


    /**
     * At This Method Will Add A New Task To The Thread Pool.
     * @param f The Task Will Be Add.
     */
    void addNewTask(function<void()> f);


    /**
     * At This Method Will Notifies One Waiting Worker To Wake.
     */
    void notifyOneWorker();


public:

    /**
     * Constructor.
     * Will Create The Thread Pool With Parameter Threads Number.
     * @param threads The The Threads Number Of Worker Threads To Create.
     */
    ThreadPool(size_t threads);


    /**
     * Destructor.
     * Clean And Stop All The Threads.
     */
    ~ThreadPool();


    /**
     * At This Method Will Add A New Task To The Threasd Pool.
     * And Throw A Run Time Error If The Pool Is Stopped.
     * @param f The Task Will Execute.
     */
    void enqueue(function<void()> f);

};

#endif //CHESS_EX_YAZANHEIB_THREADPOOL_H
