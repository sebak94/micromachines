#ifndef __MICROMACHINES_H__
#define __MICROMACHINES_H__

#include "cars/car.h"
#include "box2d-entities/destruction_listener.h"
#include "../client_th.h"
#include "../../../common/include/TrackList.h"
#include <vector>
#include <mutex>
#include <Box2D/Box2D.h>

class Micromachines {
    private:
    TrackList tracks;
    std::mutex m;
    std::vector<ClientTh*> players;
    DestructionListener destruction_listener;

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
    std::string trackSerialized();
};

#endif
