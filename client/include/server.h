#ifndef __SERVER_H__
#define __SERVER_H__

#include "../../common/include/socket.h"
#include "string"
#include "EventLoopSDL.h"
#include "Drawer.h"
#include "Dispatcher.h"

class Server {
private:
    Socket skt;
    ThreadSafeQueue queue;
    std::vector<Thread *> threads;

public:
    Server(const char *hostname, const char *service);

    ~Server();

    void runThreads();
};

#endif
