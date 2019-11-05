#include "../../include/model/micromachines.h"
#include "../../include/model/cars/car.h"
#include "../../include/client_th.h"
#include "../../../common/include/lock.h"
#include "../../../common/include/TrackList.h"
#include "../../../common/include/Track.h"
#include <vector>
#include "iostream"

Micromachines::Micromachines() {
    tracks.readTracks();
}

void Micromachines::update() {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->updateCar();
    }
}

void Micromachines::addPlayer(ClientTh *client) {
    Lock l(m);
    players.push_back(client);
}

void Micromachines::removePlayer(ClientTh *client) {
    Lock l(m);
    size_t index_to_remove = -1;

    for (size_t i = 0; i < players.size(); i++) {
        if (players[i] == client) {
            index_to_remove = i;
            break;
        }
    }

    if (index_to_remove != -1) {
        players.erase(players.begin() + index_to_remove);
    }
}

void Micromachines::updatePlayersState() {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->processNextAction();
    }
}

void Micromachines::cleanPlayers() {
    Lock l(m);
    players.clear();
}

void Micromachines::sendNewStateToPlayers() {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->sendCarData();
    }
}

std::string Micromachines::trackSerialized() {
    return tracks.getTrack("mini").serialize();
}
