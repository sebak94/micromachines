#ifndef __CLIENT_TH_H__
#define __CLIENT_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "model/cars/car.h"
#include <string>
#include <vector>
#include <atomic>
#include <queue>
#include <mutex>

typedef enum {
    mainMenu,
    selectingTrack,
    selectingCar,
    waitingPlayers,
    startCountdown,
    playing,
    waitingEnd,
    gameEnded
} GameState;

class ClientTh: public Thread {
    private:
    std::atomic<bool> keep_talking;
    std::atomic<bool> is_running;
    Socket *peer;
    Car *car;
    std::queue<char> actions;
    std::mutex m;
    GameState state = mainMenu;

    void sendWelcomeMsg();
    void receive(char *action);
    void send(std::string &response);

    public:
    ClientTh(Socket *peer, Car* car);
    void receiveActionPlugin(char *action);
    void processNextAction();
    void updateCar();
    void sendCarData();
    void sendAllCarsToPlayer(std::vector<ClientTh*> players);
    void sendTrackData(std::string track_serialized);
    virtual void run() override;
    virtual void stop() override;
    bool isDead();
    ~ClientTh();

    void setState(GameState s);

    int getCarPosX();

    int getCarPosY();

    int getCarLastTrackID();

    void newCarPosition(Point point);

    void updateLastTrackID(int ID);

    void updateLaps();

    void sendLapsData(std::string laps_serialized);
};

#endif
