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

#define MSG_ST_MAINMENU "G\nmainMenu\n"
#define MSG_ST_SELECTINGTRACK "G\nselectingTrack\n"
#define MSG_ST_SELECTINGCAR "G\nselectingCar\n"
#define MSG_ST_WAITINGPLAYERS "G\nwaitingPlayers\n"
#define MSG_ST_COUNTDOWN "G\nstartCountdown\n"
#define MSG_ST_PLAYING "G\nplaying\n"
#define MSG_ST_WAITINGEND "G\nwaitingEnd\n"
#define MSG_ST_GAMEENDED "G\ngameEnded\n"

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
    void send(std::basic_string<char> response);
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

    int getCarPosX();

    int getCarPosY();

    void reduceSpeed(float32 factor);

    int getCarLastTrackID();

    void newCarPosition(Point point);

    void updateLastTrackID(int ID);

    void updateLaps();

    void sendLapsData(std::string laps_serialized);

    GameState getState();

    void updateGameState(GameState &previousSt, GameState &st);
};

#endif
