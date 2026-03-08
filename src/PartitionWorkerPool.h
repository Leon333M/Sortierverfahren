// PartitionWorkerPool.h
#pragma once
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class PartitionWorkerPool {
private:
    struct TaskWrapper {
        std::function<void()> func;
        std::shared_ptr<std::atomic<bool>> done;
    };

    std::vector<std::thread> workers;
    std::queue<TaskWrapper> tasks;

    std::mutex mutex;
    std::condition_variable cv;
    bool stop = false;

public:
    // ================= HANDLE =================
    class TaskHandle {
        std::shared_ptr<std::atomic<bool>> done;

    public:
        TaskHandle(std::shared_ptr<std::atomic<bool>> d) : done(d) {}

        void wait() const {
            while (!done->load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
        }

        bool finished() const {
            return done->load(std::memory_order_acquire);
        }
    };
    // ===========================================

    explicit PartitionWorkerPool(int numThreads) {
        for (int i = 0; i < numThreads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    TaskWrapper task;

                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        cv.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty())
                            return;

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task.func();
                    task.done->store(true, std::memory_order_release);
                }
            });
        }
    }

    ~PartitionWorkerPool() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            stop = true;
        }
        cv.notify_all();

        for (auto &t : workers)
            t.join();
    }

    TaskHandle addTask(std::function<void()> func) {
        auto done = std::make_shared<std::atomic<bool>>(false);

        {
            std::lock_guard<std::mutex> lock(mutex);
            tasks.push({std::move(func), done});
        }

        cv.notify_one();
        return TaskHandle(done);
    }

    int threadCount() const {
        return workers.size();
    }
};
