// WorkerPool.h
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

struct Task {
    int *liste;
    int links;
    int rechts;
};

class WorkerPool {
private:
    std::vector<std::thread> threads;
    std::queue<Task> taskQueue;
    std::mutex sperre;
    std::condition_variable cv;
    std::condition_variable cvMain;
    std::atomic<int> activeTasks;
    std::atomic<int> freieThreads;
    bool finished;

public:
    std::function<void(int *, int, int, WorkerPool &)> taskHandler;

public:
    WorkerPool(int numThreads) : finished(false), activeTasks(0), freieThreads(0) {
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&WorkerPool::worker, this);
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

    void addTask(const Task &task) {
        {
            std::lock_guard<std::mutex> lock(sperre);
            taskQueue.push(task);
            activeTasks++;
            cv.notify_one();
        }
    }

    void addTaskWaitUntilDone(const Task &task) {
        std::unique_lock<std::mutex> lock(sperre);
        taskQueue.push(task);
        activeTasks++;
        cv.notify_one();
        cvMain.wait(lock, [this] { return activeTasks == 0; });
    }

    int getFreieThreads() {
        return freieThreads;
    }

private:
    void worker() {
        while (true) {
            Task task;
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

                task = taskQueue.front();
                taskQueue.pop();
            }

            // Task bearbeiten
            if (taskHandler) {
                taskHandler(task.liste, task.links, task.rechts, *this);
            }

            activeTasks--;
            if (activeTasks == 0) {
                cvMain.notify_all();
            }
        }
    }
};
