// ThreadPool.h
#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    // Create a pool with 'numThreads' worker threads.
    explicit ThreadPool(size_t numThreads)
            : shutdownFlag(false)
    {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> job;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        cv.wait(lock, [this] {
                            return !jobs.empty() || shutdownFlag.load();
                        });
                        if (shutdownFlag.load() && jobs.empty())
                            return;
                        job = std::move(jobs.front());
                        jobs.pop();
                    }
                    job();
                }
            });
        }
    }

    // Submit a new job to the queue.
    void submit(std::function<void()> f) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            jobs.push(std::move(f));
        }
        cv.notify_one();
    }

    // Gracefully shut down all threads.
    ~ThreadPool() {
        shutdownFlag.store(true);
        cv.notify_all();
        for (auto &t : workers) {
            if (t.joinable())
                t.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> shutdownFlag;
};
