#ifndef __COMMUNICATOR_H__
#define __COMMUNICATOR_H__

#include "ThreadSafeQueue.h"
#include "../../common/include/thread.h"
#include "../../common/include/socket.h"

class Dispatcher : public Thread {
private:
    bool running;
    ThreadSafeQueue &queue;
    Socket &socket;

public:
    Dispatcher(ThreadSafeQueue &queue, Socket &socket);
    ~Dispatcher();
    virtual void run() override;
};

#endif
