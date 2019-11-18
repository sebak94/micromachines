#include "../../include/model/micromachines.h"
#include "../../../common/include/lock.h"
#include "../../../common/include/socket_error.h"
#include <Box2D/Box2D.h>

#define DEGTORAD 0.0174532925199432957f
#define SPEEDREDUCTIONFACTOR 0.9

Micromachines::Micromachines() {
    tracks.readTracks();
    track = tracks.getTrack("classic"); //aca hay que poner la track que eligio el cliente
    world = new b2World(b2Vec2(0, 0));
    world->SetDestructionListener(&destruction_listener);
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

std::string Micromachines::lapsSerialized() {
    return std::to_string(laps) += "\n";
}

void Micromachines::removePlayer(ClientTh *client) {
    Lock l(m);
    removePlayerFromVector(client);
}

void Micromachines::removePlayerFromVector(ClientTh *player) {
    size_t index_to_remove = -1;

    for (size_t i = 0; i < players.size(); i++) {
        if (players[i] == player) {
            index_to_remove = i;
            break;
        }
    }
    if (index_to_remove != -1) {
        players.erase(players.begin() + index_to_remove);
    }
}

int Micromachines::getPlayersNumber() {
    Lock l(m);
    return players.size();
}

void Micromachines::setPlayerGameState(ClientTh *player, GameState state) {
    Lock l(m);
    player->setState(state);
}

void Micromachines::setAllPlayersGameStates(GameState state) {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->setState(state);
    }
}

void Micromachines::updatePlayersState() {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->processNextAction();
        if (!track.isOnTrack(players[i]->getCarPosX(),players[i]->getCarPosY()))
            players[i]->reduceSpeed(SPEEDREDUCTIONFACTOR);
    }
}

// Checks if cars jump track parts
void Micromachines::monitorTrack() {
    int x, y, lastID, currentID;
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        x = players[i]->getCarPosX();
        y = players[i]->getCarPosY();
        lastID = players[i]->getCarLastTrackID();
        currentID = track.getTrackPart(x, y).getID();
        //std::cout << "Auto " << i << "("<<x<<","<<y<<"). " << ". LastID:" << lastID << ". Current ID: " << track.getCurrentID(x, y) << "   " << std::endl;
        //if (i==0)
            //std::cout << x << "," << y << std::endl;
        if (currentID == lastID || !track.isOnTrack(x,y)) {
            // sigue en el mismo o está fuera de pista
        } else if (track.jumpedTrackPart(x, y, lastID)) {
            // está en pista y salteó pedazos
            players[i]->newCarPosition(track.getTrackPartPoint(lastID) + Point(BLOCKSIZE/2,BLOCKSIZE*(1.1)));
            players[i]->updateLastTrackID(lastID);
        } else {
            // avanzó al siguiente
            if (currentID == 0 && lastID == track.getPartsNumber() - 1) {
                players[i]->updateLaps();
            }
            players[i]->updateLastTrackID(currentID);
        }
    }
    /*for (size_t i = 0; i < players.size(); i++) {
        printf("\r                            \033[F");
    }*/

}

void Micromachines::updateWinners() {
    for (int i = 0; i < players.size(); i++) {
        //Si alcanzó la cantidad de vueltas y no contiene el elemento, le cambio el estado y agrego a los ganadores
        std::string color = players[i]->carColor();
        if (players[i]->getLaps() == laps &&
        (std::find(winners.begin(), winners.end(), color) == winners.end())) {
            players[i]->setState(waitingEnd);
            winners.push_back(color);
        }
        players[i]->setWinners(winners);
    }
}

void Micromachines::sendWinners() {
    //Mando los ganadores cuando todos hayan llegado a la meta
    bool allWaiting = true;
    for (int i = 0; i < players.size(); i++) {
        if (players[i]->getState() != waitingEnd) {
            allWaiting = false;
        }
    }
    if (allWaiting) {
        for (int i = 0; i < players.size(); i++) {
            players[i]->sendWinners();
        }
    }
}

void Micromachines::cleanPlayers() {
    Lock l(m);
    players.clear();
}

void Micromachines::changeCarState(char *new_command) {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++)
        for (int j = 0; j < 10; ++j)
            players[i]->receiveActionPlugin(new_command);
}

void Micromachines::sendNewStateToPlayers() {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->sendAllCarsToPlayer(players);
    }
}

std::string Micromachines::allTrackNames() {
    return tracks.serialize();
}

Point Micromachines::getStartingPoint(int position) {
    return track.getCarStartingPos(position);
}

int Micromachines::getStartID(int order) {
    return track.getStartingID(order);
}

uint16_t Micromachines::getStartingCarRot(int position) {
    return track.getCarStartingRotation(position);
}

TrackList& Micromachines::getTracks() {
    return tracks;
}

bool Micromachines::somePlayersInMainMenu() {
    for (int i = 0; i < players.size(); i++) {
        if (players[i]->getState() == mainMenu) {
            return true;
        }
    }
    return false;
}
