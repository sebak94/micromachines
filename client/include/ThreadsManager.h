#ifndef __THREADS_MANAGER_H__
#define __THREADS_MANAGER_H__

#include "../../common/include/socket.h"
#include "string"
#include "EventLoopSDL.h"
#include "Drawer.h"
#include "Dispatcher.h"
#include "BlockingQueue.h"

class ThreadsManager {
    private:
    Socket skt;
    BlockingQueue queue;
    std::vector<Thread*> threads;

    public:
    ThreadsManager(const char *hostname, const char *service);
    ~ThreadsManager();
    void runThreads();
};

#endif
