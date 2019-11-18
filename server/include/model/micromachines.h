#ifndef __MICROMACHINES_H__
#define __MICROMACHINES_H__

#include "cars/car.h"
#include "box2d-entities/destruction_listener.h"
#include "../client_th.h"
#include "../../../common/include/TrackList.h"
#include <vector>
#include <mutex>
#include <cstring>
#include <Box2D/Box2D.h>

class Micromachines {
    private:
    TrackList tracks;
    Track track;
    std::mutex m;
    std::vector<ClientTh*> players;
    int laps = 2;
    DestructionListener destruction_listener;
    std::vector<std::string> winners;

    void removePlayerFromVector(ClientTh *player);

    public:
    b2World *world;

    Micromachines();
    void update();
    void addPlayer(ClientTh *client);
    void updatePlayersState();
    void removePlayer(ClientTh *client);
    void cleanPlayers();
    void sendNewStateToPlayers();
    std::string allTrackNames();
    Point getStartingPoint(int position);
    uint16_t getStartingCarRot(int position);
    void changeCarState(char *new_command);
    int getPlayersNumber();
    void setPlayerGameState(ClientTh *player, GameState state);
    void setAllPlayersGameStates(GameState state);
    std::string lapsSerialized();
    void monitorTrack();
    int getStartID(int order);
    TrackList& getTracks();
    bool somePlayersInMainMenu();
    void updateWinners();
    void sendWinners();
};

#endif
