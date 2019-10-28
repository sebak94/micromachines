#ifndef __THREADSAFEQUEUE_H__
#define __THREADSAFEQUEUE_H__

#include <queue>
#include <condition_variable>

class ThreadSafeQueue {
private:
    std::queue<std::string> queue;
    int limit;
    std::condition_variable condVarPop;
    std::mutex m;

public:
    explicit ThreadSafeQueue(int limit);

    ThreadSafeQueue(ThreadSafeQueue &&other);

    virtual ~ThreadSafeQueue();

    ThreadSafeQueue& operator=(ThreadSafeQueue &&other);

    void push(std::string text);

    std::string pop();

private:
    ThreadSafeQueue(const ThreadSafeQueue &other) = delete;

    ThreadSafeQueue& operator=(const ThreadSafeQueue &other) = delete;
};

#endif
