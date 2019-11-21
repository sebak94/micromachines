#include "../include/BlockingQueue.h"

BlockingQueue::BlockingQueue(int limit) {
    this->limit = limit;
}

BlockingQueue::BlockingQueue(BlockingQueue &&other) {
    this->limit = other.limit;
    other.limit = 0;
}

BlockingQueue::~BlockingQueue() {}

BlockingQueue& BlockingQueue::operator=(BlockingQueue &&other) {
    if (this == &other)
        return *this;
    //Me robo los punteros
    this->limit = other.limit;
    other.limit = 0;
    return *this;
}

void BlockingQueue::push(std::string text) {
    this->queue.push(text);
    this->condVarPop.notify_all();
}

std::string BlockingQueue::pop() {
    std::unique_lock<std::mutex> lock(this->m);
    while (this->queue.empty()) {
        this->condVarPop.wait(lock);
    }
    std::string text = this->queue.front();
    this->queue.pop();
    return text;
}
