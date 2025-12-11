// WorkerPoolString.h
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

struct Task {
    std::string *liste;
    int links;
    int rechts;
};

class WorkerPoolString {
private:
    std::vector<std::thread> threads;
    std::queue<Task> taskQueue;
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<int> activeTasks;
    bool finished;

public:
    std::function<void(std::string *, int, int, WorkerPoolString &)> taskHandler;

public:
    WorkerPoolString(int numThreads) : finished(false), activeTasks(0) {
        for (int i = 0; i < numThreads; ++i)
            threads.emplace_back(&WorkerPoolString::worker, this);
    }

    ~WorkerPoolString() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            finished = true;
        }
        cv.notify_all();
        for (auto &t : threads)
            t.join();
    }

    void addTask(const Task &task) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            taskQueue.push(task);
            activeTasks++;
        }
        cv.notify_one();
    }

    void waitUntilDone() {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return activeTasks == 0; });
    }

    void addTaskWaitUntilDone(const Task &task) {
        std::unique_lock<std::mutex> lock(mutex);
        taskQueue.push(task);
        activeTasks++;
        cv.notify_one();
        cv.wait(lock, [this] { return activeTasks == 0; });
    }

private:
    void worker() {
        while (true) {
            Task task;
            {
                std::unique_lock<std::mutex> lock(mutex);
                cv.wait(lock, [this] { return finished || !taskQueue.empty(); });

                if (finished && taskQueue.empty())
                    return;
                if (taskQueue.empty())
                    continue;

                task = taskQueue.front();
                taskQueue.pop();
            }

            // Task bearbeiten
            if (taskHandler) {
                taskHandler(task.liste, task.links, task.rechts, *this);
            }

            activeTasks--;
            cv.notify_all();
        }
    }
};
