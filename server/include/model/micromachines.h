#ifndef __MICROMACHINES_H__
#define __MICROMACHINES_H__

#include "cars/car.h"
#include "tracks/track.h"
#include "../client_th.h"
#include <vector>
#include <mutex>

class Micromachines {
    private:
    std::mutex m;
    Track track;
    std::vector<ClientTh*> players;

    public:
    void update();
    void addPlayer(ClientTh *client);
    void updatePlayersState();
    void removePlayer(ClientTh *client);
    void cleanPlayers();
    void sendNewStateToPlayers();
};

#endif
