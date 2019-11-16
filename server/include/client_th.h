#ifndef __CLIENT_TH_H__
#define __CLIENT_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "../../common/include/TrackList.h"
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
    TrackList& tracks;

    void sendWelcomeMsg();
    void receive(char *action);
    void send(std::string &response);
    std::string parse(const std::string &str, size_t &pos, const char delim);
    void setMatch();

    public:
    ClientTh(Socket *peer, Car* car, TrackList& tracks);
    void receiveActionPlugin(char *action);
    void processNextAction();
    void updateCar();
    void sendCarData();
    void sendAllCarsToPlayer(std::vector<ClientTh*> players);
    void sendTrackData(std::string track_serialized);
    void sendAllTrackNames(std::string tracks);
    virtual void run() override;
    virtual void stop() override;
    bool isDead();
    ~ClientTh();

    void setState(GameState s);
    GameState getState();
};

#endif
