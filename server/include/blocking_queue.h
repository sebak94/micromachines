#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include "player_action.h"
#include <queue>
#include <condition_variable>
#include <mutex>

class BlockingQueue {
    private:
    std::queue<PlayerAction> q;
    std::mutex m;
    std::condition_variable pop_cv;

    public:
    BlockingQueue();
    void push(PlayerAction pa);
    PlayerAction pop();
    bool empty();
    ~BlockingQueue();
};

#endif
