#include "../../include/model/micromachines.h"
#include "../../include/model/interpreter.h"
#include "../../include/model/cars/car.h"
#include "../../include/client_th.h"
#include "../../../common/include/lock.h"
#include <vector>
#include "iostream"

void Micromachines::update() {
    Lock l(m);
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->updateCar();
    }
}

void Micromachines::addClient(ClientTh *client) {
    Lock l(m);
    players.push_back(client);
}

void Micromachines::updatePlayersState() {
    Lock l(m);
    Interpreter interp;
    for (size_t i = 0; i < players.size(); i++) {
        std::shared_ptr<CarState> state_received = interp.interpret(players[i]->popAction());
        players[i]->updateCarState(state_received.get());
    }
}

void Micromachines::cleanPlayers() {
    players.clear();
}
