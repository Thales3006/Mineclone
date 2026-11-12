#ifndef TASK_MANAGER_CLASS_H
#define TASK_MANAGER_CLASS_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class TaskManager {
  public:
    using Task = std::function<void()>;

    static TaskManager &instance();

    TaskManager(const TaskManager &) = delete;
    TaskManager &operator=(const TaskManager &) = delete;

    void run(Task task);

  private:
    TaskManager();
    ~TaskManager();

    std::vector<std::thread> workers;
    std::queue<Task> tasks;
    std::mutex tasksMutex;
    std::condition_variable tasksCv;
    bool stopping;

    void workerLoop();
};

#endif
