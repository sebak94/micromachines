#ifndef __CLIENT_TH_H__
#define __CLIENT_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "model/cars/car.h"
#include "model/cars/states/car_state.h"
#include "blocking_queue.h"
#include <string>
#include <vector>
#include <atomic>

class ClientTh: public Thread {
    private:
    std::atomic<bool> keep_talking;
    std::atomic<bool> is_running;
    Socket *peer;
    Car *car;
    BlockingQueue actions;

    void receive(char *action);
    void send(std::string &response);

    public:
    ClientTh(Socket *peer);
    char popAction();
    void updateCarState(CarState *state_received);
    void updateCar();
    virtual void run() override;
    virtual void stop() override;
    bool isDead();
    ~ClientTh();
};

#endif
