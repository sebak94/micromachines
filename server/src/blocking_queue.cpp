#include "../include/blocking_queue.h"
#include <condition_variable>
#include <mutex>
#include <iostream>

BlockingQueue::BlockingQueue() {}

void BlockingQueue::push(char action) {
    std::unique_lock<std::mutex> lock(m);
    q.push(action);
}

char BlockingQueue::pop() {
    std::unique_lock<std::mutex> lock(m);

    while (q.empty()) {
        pop_cv.wait(lock);
    }
    char pa = q.front();
    q.pop();
    return pa;
}

bool BlockingQueue::empty() {
    return q.empty();
}

BlockingQueue::~BlockingQueue() {}
