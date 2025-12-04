// MergeWorkerPoolString.h
#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

struct MergeTask {
    std::string *liste;
    int links;
    int rechts;
};

class MergeTaskHandle {
public:
    std::shared_ptr<std::atomic<bool>> done;
    void wait() const {
        while (!done->load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }
};

class MergeWorkerPoolString {
private:
    std::vector<std::thread> threads;
    std::queue<MergeTask> taskQueue;
    std::queue<std::shared_ptr<std::atomic<bool>>> doneFlags;

    std::mutex mutex;
    std::condition_variable cv;

    std::atomic<int> activeTasks;
    bool finished;

public:
    std::function<void(std::string *, int, int, MergeWorkerPoolString &)> taskHandler;

public:
    MergeWorkerPoolString(int numThreads) : activeTasks(0), finished(false) {
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&MergeWorkerPoolString::worker, this);
        }
    }

    ~MergeWorkerPoolString() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            finished = true;
        }
        cv.notify_all();
        for (auto &t : threads)
            t.join();
    }

    // intelligente Task-Anmeldung
    MergeTaskHandle addTaskSmart(const MergeTask &task) {
        std::unique_lock<std::mutex> lock(mutex);

        // Kein freier Thread selber ausfuhren
        if (activeTasks >= threads.size()) {
            lock.unlock();
            taskHandler(task.liste, task.links, task.rechts, *this);

            MergeTaskHandle h;
            h.done = std::make_shared<std::atomic<bool>>(true);
            return h;
        }

        // Worker frei → auf Queue legen
        MergeTaskHandle h;
        h.done = std::make_shared<std::atomic<bool>>(false);
        taskQueue.push(task);
        doneFlags.push(h.done);

        activeTasks++;
        lock.unlock();
        cv.notify_one();
        return h;
    }

private:
    void worker() {
        while (true) {
            MergeTask task;
            std::shared_ptr<std::atomic<bool>> doneFlag;

            {
                std::unique_lock<std::mutex> lock(mutex);
                cv.wait(lock, [this] { return finished || !taskQueue.empty(); });

                if (finished && taskQueue.empty())
                    return;

                task = taskQueue.front();
                taskQueue.pop();
                doneFlag = doneFlags.front();
                doneFlags.pop();
            }

            if (taskHandler)
                taskHandler(task.liste, task.links, task.rechts, *this);

            doneFlag->store(true, std::memory_order_release);

            activeTasks--;
            cv.notify_all();
        }
    }
};
