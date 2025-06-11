#include "ThreadPool.h"


// Add A New Thread To The Pool Method.
void ThreadPool::addNewWorker()
{
    the_workers.emplace_back([this]() { workerThreadLoop(); });
}


// Main Loop For The Worker Thread Method.
void ThreadPool::workerThreadLoop()
{
    while (true)
    {
        function<void()> task;
        {
            // Lock The Queue And Wait For The New Tasks Or To Stop Signal.
            unique_lock <mutex> lock(this->queue_mutex);
            this->condition.wait(lock, [this]
            {
                // Will Constitute Just If We Have A Tasks, Or We Have Stop Request.
                return this->stopier.load() || !this->the_tasks.empty();
            });

            // If We Has A Stop Request, Stop And Return.
            if (this->stopier && this->the_tasks.empty())
            {
                return;
            }

            // Take A New Task From The Queue.
            task = move(this->the_tasks.front());
            this->the_tasks.pop();
        }
        // And Here Will Run The New Task We Had Take.
        task();
    }
}


// At This Method Will Stop And Shutdown All Threads.
void ThreadPool::shutDown()
{
    // Lock The Queue And Update That No New Tasks Can Enter The Queue.
    {
        unique_lock <mutex> lock(queue_mutex);
        stopier = true;
    }
    // Notify All The Waiting Threads To Wake Up, And Check The Shutdown Status.
    condition.notify_all();
}


// Wait For All The Threads To Finish Method.
void ThreadPool::joinAllThreads()
{
    for (thread &worker: the_workers)
    {
        // Check If The Threads Is Joinable, And Join It.
        if (worker.joinable())
        {
            worker.join();
        }
    }
}


// Add A New Thread To The Pool Method.
void ThreadPool::addNewTask(function<void()> f)
{
    // Lock The Queue For The Thread Safety.
    unique_lock <mutex> lock(queue_mutex);

    // Check If Pool Is Stop, If Stop Throw A 'runtime_error'.
    if (stopier)
    {
        throw runtime_error("Error : Enqueue On Thread Pool");
    }
    the_tasks.push(move(f));
}


// Notifies Waiting Worker Thread To Wake Method.
void ThreadPool::notifyOneWorker()
{
    condition.notify_one();
}


// Constructor
ThreadPool::ThreadPool(size_t threads) : threads_counter(threads), stopier(false)
{
    for (size_t i = 0; i < threads_counter; i++)
    {
        addNewWorker();
    }
}


// Destructor.
ThreadPool::~ThreadPool()
{
    shutDown();
    joinAllThreads();
}


// Add A Task To The Pool Method.
void ThreadPool::enqueue(function<void()> f)
{
    addNewTask(f);
    notifyOneWorker();
}