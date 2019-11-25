#ifndef __COMMUNICATOR_H__
#define __COMMUNICATOR_H__

#include <string>
#include <vector>
#include "BlockingQueue.h"
#include "../../common/include/thread.h"
#include "../../common/include/socket.h"

class Dispatcher : public Thread {
private:
    bool running;
    BlockingQueue &queue;
    Socket &socket;

public:
    Dispatcher(BlockingQueue &queue, Socket &socket);
    ~Dispatcher();
    void run() override;
    void stop() override;
};

#endif
