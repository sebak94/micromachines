#ifndef __BLOCKINGQUEUE_H__
#define __BLOCKINGQUEUE_H__

#include <queue>
#include <condition_variable>

class BlockingQueue {
private:
    std::queue<std::string> queue;
    int limit;
    std::condition_variable condVarPop;
    std::mutex m;

public:
    explicit BlockingQueue(int limit);
    BlockingQueue(BlockingQueue &&other);
    virtual ~BlockingQueue();
    BlockingQueue& operator=(BlockingQueue &&other);
    void push(std::string text);
    std::string pop();

private:
    BlockingQueue(const BlockingQueue &other) = delete;
    BlockingQueue& operator=(const BlockingQueue &other) = delete;
};

#endif
