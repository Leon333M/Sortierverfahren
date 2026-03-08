// WorkerPool.h
#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <variant>
#include <vector>

struct Task {
    int *liste;
    int links;
    int rechts;
};

// Fusionierte Task-Struktur
struct LambdaTaskWrapper {
    std::function<void()> func;
    std::shared_ptr<std::atomic<bool>> done;
};

using PoolTask = std::variant<Task, LambdaTaskWrapper>;

class WorkerPool {
private:
    std::vector<std::thread> threads;
    std::queue<PoolTask> taskQueue;
    std::mutex sperre;
    std::condition_variable cv;
    std::condition_variable cvMain;
    std::atomic<int> activeTasks;
    std::atomic<int> freieThreads;
    bool finished;

public:
    std::function<void(int *, int, int, WorkerPool &)> taskHandler;

    // Das Handle aus dem PartitionWorkerPool fur die Lambdas
    class TaskHandle {
        std::shared_ptr<std::atomic<bool>> done;

    public:
        TaskHandle(std::shared_ptr<std::atomic<bool>> d) : done(d) {}
        void wait() const {
            while (!done->load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
        }
    };

    WorkerPool(int numThreads) : finished(false), activeTasks(0), freieThreads(0) {
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&WorkerPool::worker, this);
        }
        // Warten, bis alle Threads "bereit" (im wait-Zustand) sind
        while (freieThreads.load() < numThreads) {
            std::this_thread::yield();
        }
    }

    ~WorkerPool() {
        {
            std::lock_guard<std::mutex> lock(sperre);
            finished = true;
        }
        cv.notify_all();
        for (auto &t : threads) {
            t.join();
        }
    }

    // Fur Quicksort (Rekursion)
    void addTask(const Task &task) {
        std::lock_guard<std::mutex> lock(sperre);
        taskQueue.push(task);
        activeTasks++;
        cv.notify_one();
    }

    // Fur Bereiche (Partitionierung)
    TaskHandle addLambdaTask(std::function<void()> func) {
        auto done = std::make_shared<std::atomic<bool>>(false);
        {
            // std::lock_guard<std::mutex> lock(sperre); // Sperre vorher holen
            taskQueue.push(LambdaTaskWrapper{std::move(func), done});
            activeTasks++;
            cv.notify_one();
        }
        return TaskHandle(done);
    }

    void addTaskWaitUntilDone(const Task &task) {
        std::unique_lock<std::mutex> lock(sperre);
        taskQueue.push(task);
        activeTasks++;
        cv.notify_one();
        cvMain.wait(lock, [this] { return activeTasks == 0; });
    }

    int getFreieThreads() {
        return freieThreads.load();
    }

    std::mutex &getSperre() {
        return sperre;
    };

    int getTaskQueueSize() {
        return taskQueue.size();
    };

private:
    void worker() {
        while (true) {
            PoolTask currentTask;
            {
                std::unique_lock<std::mutex> lock(sperre);
                freieThreads++;
                cv.wait(lock, [this] { return finished || !taskQueue.empty(); });
                freieThreads--;
                if (finished && taskQueue.empty()) {
                    return;
                }
                if (taskQueue.empty()) {
                    continue;
                }

                currentTask = std::move(taskQueue.front());
                taskQueue.pop();
            }

            // Fallunterscheidung: Was ist in der Queue?
            if (std::holds_alternative<Task>(currentTask)) {
                Task &t = std::get<Task>(currentTask);
                if (taskHandler)
                    taskHandler(t.liste, t.links, t.rechts, *this);
            } else if (std::holds_alternative<LambdaTaskWrapper>(currentTask)) {
                auto &lt = std::get<LambdaTaskWrapper>(currentTask);
                lt.func();
                lt.done->store(true, std::memory_order_release);
            }

            activeTasks--;
            if (activeTasks == 0) {
                cvMain.notify_all();
            }
        }
    }
};
