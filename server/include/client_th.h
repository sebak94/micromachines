#ifndef __CLIENT_TH_H__
#define __CLIENT_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "../../common/include/TrackList.h"
#include "../../common/include/gameState.h"
#include "../../common/include/ModifierList.h"
#include "model/cars/car.h"
#include <string>
#include <vector>
#include <atomic>
#include <queue>
#include <mutex>

#define MSG_ST_MAINMENU "G,mainMenu\n"
#define MSG_ST_CREATING "G,creating\n"
#define MSG_ST_JOINING "G,joining\n"
#define MSG_ST_WAITINGPLAYERS "G,waitingPlayers\n"
#define MSG_ST_COUNTDOWN "G,startCountdown\n"
#define MSG_ST_PLAYING "G,playing\n"
#define MSG_ST_WAITINGEND "G,waitingEnd\n"
#define MSG_ST_GAMEENDED "G,gameEnded\n"

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
    std::string availableGames{};
    int gameNumber = -1;
    std::vector<std::string> winners;

    void sendWelcomeMsg();
    void receive(char *action);
    void send(std::basic_string<char> response);
    std::string parse(const std::string &str, size_t &pos, const char delim);

    public:
    void setMatch();
    ClientTh(Socket *peer, TrackList &tracks);
    void receiveActionPlugin(char *action);
    void processNextAction();
    void updateCar();
    void sendCarData();
    void sendAllCarsToPlayer(std::vector<ClientTh*> players);
    void sendTrackData(std::string track_serialized);
    void sendModifiers();
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

    void sendGameState(GameState &previousSt, GameState &st);

    void setCar(Car *matchCar);

    void setAvailableGames(std::string g);

    void sendAvailableGames();

    int getGameNumber();

    void setPlayerMode();

    int getLaps();

    std::string carColor();

    void setWinners(std::vector<std::string> w);
    void sendWinners();

    void clean();
};

#endif
