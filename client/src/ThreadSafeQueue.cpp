#include "../include/ThreadSafeQueue.h"

ThreadSafeQueue::ThreadSafeQueue(int limit) {
    this->limit = limit;
}

ThreadSafeQueue::ThreadSafeQueue(ThreadSafeQueue &&other) {
    this->limit = other.limit;
    other.limit = 0;
}

ThreadSafeQueue::~ThreadSafeQueue() {}

ThreadSafeQueue& ThreadSafeQueue::operator=(ThreadSafeQueue &&other) {
    if (this == &other)
        return *this;
    //Me robo los punteros
    this->limit = other.limit;
    other.limit = 0;
    return *this;
}

void ThreadSafeQueue::push(std::string text) {
    this->queue.push(text);
    this->condVarPop.notify_all();
}

std::string ThreadSafeQueue::pop() {
    std::unique_lock<std::mutex> lock(this->m);
    while (this->queue.empty()) {
        this->condVarPop.wait(lock);
    }
    std::string text = this->queue.front();
    this->queue.pop();
    return text;
}
