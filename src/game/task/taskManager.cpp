#include "game/task/taskManager.h"

#include <algorithm>

TaskManager &TaskManager::instance() {
    static TaskManager instance;
    return instance;
}

TaskManager::TaskManager() : stopping(false) {
    unsigned int count = std::max(1u, std::thread::hardware_concurrency());
    for (unsigned int i = 0; i < count; i++)
        workers.emplace_back(&TaskManager::workerLoop, this);
}

TaskManager::~TaskManager() {
    {
        std::lock_guard<std::mutex> lock(tasksMutex);
        stopping = true;
    }
    tasksCv.notify_all();

    for (std::thread &worker : workers)
        if (worker.joinable())
            worker.join();
}

void TaskManager::run(Task task) {
    {
        std::lock_guard<std::mutex> lock(tasksMutex);
        tasks.push(std::move(task));
    }
    tasksCv.notify_one();
}

void TaskManager::workerLoop() {
    while (true) {
        Task task;
        {
            std::unique_lock<std::mutex> lock(tasksMutex);
            tasksCv.wait(lock, [this] { return stopping || !tasks.empty(); });

            if (stopping && tasks.empty())
                return;

            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}
