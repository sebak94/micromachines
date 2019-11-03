#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include <queue>
#include <condition_variable>
#include <mutex>

class BlockingQueue {
    private:
    std::queue<char> q;
    std::mutex m;
    std::condition_variable pop_cv;

    public:
    BlockingQueue();
    void push(char action);
    char pop();
    bool empty();
    ~BlockingQueue();
};

#endif
