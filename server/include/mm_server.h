#ifndef __MM_SERVER_H__
#define __MM_SERVER_H__
#include "../../common/include/thread.h"
#include <vector>

class MMServer {
    private:
    std::vector<Thread*> threads;
    void startThreads();
    void finishThreads();

    public:
    MMServer() = default;
    void start(const char *service);
};

#endif
