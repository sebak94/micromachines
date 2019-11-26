#ifndef __MICROMACHINES_H__
#define __MICROMACHINES_H__

#include "cars/car.h"
#include "box2d-entities/destruction_listener.h"
#include "../client_th.h"
#include "../../../common/include/TrackList.h"
#include "../../../common/include/ModifierList.h"
#include "../../../common/include/Config.h"
#include <vector>
#include <mutex>
#include <cstring>
#include <Box2D/Box2D.h>

typedef std::chrono::time_point<std::chrono::steady_clock> modifierChrono;

class MicroMachinesTh : public Thread {
    private:
    TrackList tracks;
    Track track;
    Config config;
    ModifierList modifiers;
    int modifiersThrown = 0;
    std::vector<std::string> modifierTypes;
    std::mutex m;
    std::vector<ClientTh*> players;
    std::map<ColorType, Car*> cars;
    int laps;
    DestructionListener destruction_listener;
    std::vector<std::string> winners;
    bool running = true;
    std::map<ColorType, Car*>::iterator itCar;
    void removePlayerFromVector(ClientTh *player);
    int numberPlayers = 0;

    public:
    b2World *world;

    explicit MicroMachinesTh(Config &config);
    void setTrack(std::string trackStr);
    void createCars();
    void update();
    void addPlayer(ClientTh *client);
    void updatePlayersState();
    void removePlayer(ClientTh *client);
    void cleanPlayers();
    void sendNewStateToPlayers();
    Point getStartingPoint(int position);
    uint16_t getStartingCarRot(int position);
    void changeCarState(char *new_command);
    int getPlayersNumber();
    void setAllPlayersGameStates(GameState state);
    void monitorTrack();
    int getStartID(int order);
    bool somePlayersInMainMenu();
    void updateWinners();
    void sendWinners();
    bool allPlayersGameEnded();
    void run() override;
    void stop() override;
    std::string trackSerialized();
    Car *getNextCar();
    std::string modifiersSerialized();
    bool allPlayersWaitingEnd();
    bool isAnAvailableMatch();
    Config getConfig();
    void setTotalNumberPlayers(int number);
    int getTotalNumberPlayers();
    void throwModifier();
    direction setModifierDirection(trackPartType type);
    void updateModifiersPosition();
    void sendModifiersToPlayers();
};

#endif
