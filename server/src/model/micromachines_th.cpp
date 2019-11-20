#include "../../include/model/micromachines_th.h"
#include "../../../common/include/lock.h"
#include "../../../common/include/socket_error.h"
#include "../../include/model/cars/red_car.h"
#include "../../include/model/cars/black_car.h"
#include "../../include/model/cars/white_car.h"
#include "../../include/model/cars/blue_car.h"
#include "../../include/model/cars/yellow_car.h"
#include <Box2D/Box2D.h>
#include <unistd.h>

#define DEGTORAD 0.0174532925199432957f
#define SPEEDREDUCTIONFACTOR 0.9
#define REFRESHTIME 5000000  // us

MicroMachinesTh::MicroMachinesTh() {
    tracks.readTracks();
    track = tracks.getTrack("classic"); //aca hay que poner la track que eligio el cliente
    world = new b2World(b2Vec2(0, 0));
    world->SetDestructionListener(&destruction_listener);

    //Agrego todos los autos disponibles en un mapa de autos
    cars[blue] = new BlueCar(world,
                             getStartingPoint(0),
                             getStartingCarRot(0),
                             getStartID(0));
    cars[white] = new WhiteCar(world,
                               getStartingPoint(1),
                               getStartingCarRot(1),
                               getStartID(1));
    cars[black] = new BlackCar(world,
                               getStartingPoint(2),
                               getStartingCarRot(2),
                               getStartID(2));
    cars[yellow] = new YellowCar(world,
                                 getStartingPoint(3),
                                 getStartingCarRot(3),
                                 getStartID(3));
    cars[red] = new RedCar(world,
                           getStartingPoint(4),
                           getStartingCarRot(4),
                           getStartID(4));
    itCar = cars.begin();
}

void MicroMachinesTh::run() {
    while(running) {
        usleep(REFRESHTIME);
    }
}

void MicroMachinesTh::stop() {
    running = false;
}

void MicroMachinesTh::update() {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->updateCar();
    }
}

Car * MicroMachinesTh::getNextCar() {
    Car * car = itCar->second;
    itCar++;
    return car;
}

void MicroMachinesTh::addPlayer(ClientTh *client) {
    Lock l(m);
    players.push_back(client);
}

std::string MicroMachinesTh::lapsSerialized() {
    return std::to_string(laps) += "\n";
}

std::string MicroMachinesTh::trackSerialized() {
    return track.serialize();
}

void MicroMachinesTh::removePlayer(ClientTh *client) {
    Lock l(m);
    removePlayerFromVector(client);
}

void MicroMachinesTh::removePlayerFromVector(ClientTh *player) {
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

int MicroMachinesTh::getPlayersNumber() {
    Lock l(m);
    return players.size();
}

void MicroMachinesTh::setPlayerGameState(ClientTh *player, GameState state) {
    Lock l(m);
    player->setState(state);
}

void MicroMachinesTh::setAllPlayersGameStates(GameState state) {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->setState(state);
    }
}

void MicroMachinesTh::updatePlayersState() {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->processNextAction();
        if (!track.isOnTrack(players[i]->getCarPosX(),players[i]->getCarPosY()))
            players[i]->reduceSpeed(SPEEDREDUCTIONFACTOR);
    }
}

// Checks if cars jump track parts and updates laps
void MicroMachinesTh::monitorTrack() {
    int x, y, lastID, currentID;
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        x = players[i]->getCarPosX();
        y = players[i]->getCarPosY();
        lastID = players[i]->getCarLastTrackID();
        currentID = track.getTrackPart(x, y).getID();
        if (currentID == lastID || !track.isOnTrack(x,y)) {
            // sigue en el mismo o está fuera de pista
        } else if (track.jumpedTrackPart(x, y, lastID)) {
            // está en pista y salteó pedazos
            players[i]->newCarPosition(track.getTrackPartPoint(lastID) +
                        Point(BLOCKSIZE/2,BLOCKSIZE*(1.1)));
            players[i]->updateLastTrackID(lastID);
        } else {
            // avanzó al siguiente
            if (currentID == 0 && lastID == track.getPartsNumber() - 1) {
                players[i]->updateLaps();
            }
            players[i]->updateLastTrackID(currentID);
        }
    }
}

void MicroMachinesTh::cleanPlayers() {
    Lock l(m);
    players.clear();
}

void MicroMachinesTh::changeCarState(char *new_command) {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++)
        for (int j = 0; j < 10; ++j)
            players[i]->receiveActionPlugin(new_command);
}

void MicroMachinesTh::sendNewStateToPlayers() {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->sendAllCarsToPlayer(players);
    }
}

std::string MicroMachinesTh::allTrackNames() {
    return tracks.serialize();
}

Point MicroMachinesTh::getStartingPoint(int position) {
    return track.getCarStartingPos(position);
}

int MicroMachinesTh::getStartID(int order) {
    return track.getStartingID(order);
}

uint16_t MicroMachinesTh::getStartingCarRot(int position) {
    return track.getCarStartingRotation(position);
}

TrackList& MicroMachinesTh::getTracks() {
    return tracks;
}

bool MicroMachinesTh::somePlayersInMainMenu() {
    for (int i = 0; i < players.size(); i++) {
        if (players[i]->getState() == mainMenu) {
            return true;
        }
    }
    return false;
}
