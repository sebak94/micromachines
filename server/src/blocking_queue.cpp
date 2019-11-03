#include "../include/blocking_queue.h"
#include "../include/player_action.h"
#include <condition_variable>
#include <mutex>
#include <iostream>

BlockingQueue::BlockingQueue() {}

void BlockingQueue::push(PlayerAction pa) {
    std::unique_lock<std::mutex> lock(m);
    q.push(pa);
}

PlayerAction BlockingQueue::pop() {
    std::unique_lock<std::mutex> lock(m);

    while (q.empty()) {
        pop_cv.wait(lock);
    }
    PlayerAction pa = q.front();
    q.pop();
    return pa;
}

bool BlockingQueue::empty() {
    return q.empty();
}

BlockingQueue::~BlockingQueue() {}
