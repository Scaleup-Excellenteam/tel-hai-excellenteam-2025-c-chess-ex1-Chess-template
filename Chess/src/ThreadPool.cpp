//
// Created by HALAA on 01/06/2025.
//
#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadCount) : stop(false) {
    for (size_t i = 0; i < threadCount; ++i) {
        workers.emplace_back([this]() { this->workerThread(); });
    }
}

void ThreadPool::enqueue(const std::function<void()>& task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push(task);
    }
    condition.notify_one();
}

void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this]() { return stop || !tasks.empty(); });

            if (stop && tasks.empty())
                return;

            task = tasks.front();
            tasks.pop();
        }

        task();
    }
}

ThreadPool::~ThreadPool() {
    stop = true;
    condition.notify_all();
    for (std::thread& worker : workers)
        if (worker.joinable())
            worker.join();
}

