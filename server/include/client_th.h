#ifndef __CLIENT_TH_H__
#define __CLIENT_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "../../common/include/TrackList.h"
#include "../../common/include/gameState.h"
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
    Car *car = nullptr;
    std::queue<char> actions;
    std::mutex m;
    GameState state = mainMenu;
    TrackList tracks;
    std::string trackSelected;
    std::string availableGames{};
    int gameNumber = -1;
    std::vector<std::string> winners;
    std::string matchSelection;
    int numberPlayersSelected = 0;

    void sendWelcomeMsg();
    void receive(char *action);
    void send(std::basic_string<char> response);
    std::string parse(const std::string &str, size_t &pos, const char delim);

    public:
    ClientTh(Socket *peer, TrackList const &tracks);
    ~ClientTh();
    bool stillTalking();
    void setMatch();
    void receiveActionPlugin(char *action);
    void processNextAction();
    void updateCar();
    void sendCarData();
    void sendAllCarsToPlayer(std::vector<ClientTh*> players);
    void sendTrackData(std::string const &track_serialized);
    void sendModifiers(std::string const &modifiers_serialized);
    void sendAllTrackNames(std::string const &tracks);
    std::string getTrackSelected();
    int getNumberPlayersSelected();
    virtual void run() override;
    virtual void stop() override;
    bool isDead();
    void setState(GameState s);
    int getCarPosX();
    int getCarPosY();
    void modifySpeedByFactor(float32 factor);
    int getCarLastTrackID();
    void newCarPosition(Point point);
    void updateLastTrackID(int ID);
    void updateLaps();
    void sendLapsData(std::string const &laps_serialized);
    GameState getState();
    void sendGameState(GameState &previousSt, GameState &st);
    void setCar(Car *matchCar);
    void setAvailableGames(std::string const &g);
    void sendAvailableGames();
    int getGameNumber();
    void setPlayerMode();
    int getLaps();
    std::string carColor();
    void setWinners(std::vector<std::string> &w);
    void sendWinners();
    void clean();
    bool receiveMatchSelection();

    bool updateHealth();

    void giftHealth(int health);

    void reduceHealth(int health);

    int getBoost();

    void setBoost(int b);

    void sendGameState(GameState st);
};

#endif
