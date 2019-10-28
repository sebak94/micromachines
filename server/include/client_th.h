#ifndef __CLIENT_TH_H__
#define __CLIENT_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "model/micromachines.h"
#include "model/cars/car.h"
#include "blocking_queue.h"
#include <string>
#include <vector>
#include <atomic>

class ClientTh: public Thread {
    private:
    std::atomic<bool> keep_talking;
    std::atomic<bool> is_running;
    Socket *peer;
    Micromachines micromachines;
    Car *car;
    BlockingQueue actions;

    void receive(char *action);
    void send(std::string &response);

    public:
    ClientTh(Socket *peer, Micromachines &micromachines);
    virtual void run() override;
    virtual void stop() override;
    bool isDead();
    ~ClientTh();
};

#endif
