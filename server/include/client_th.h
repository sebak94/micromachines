#ifndef __CLIENT_TH_H__
#define __CLIENT_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include <string>
#include <vector>
#include <atomic>

class ClientTh: public Thread {
    private:
    std::atomic<bool> keep_talking;
    std::atomic<bool> is_running;
    Socket *peer;

    void receive(std::vector<char> &command);
    void send(std::string &response);

    public:
    ClientTh(Socket *peer);
    virtual void run() override;
    void stop();
    bool isDead();
    ~ClientTh();
};

#endif
