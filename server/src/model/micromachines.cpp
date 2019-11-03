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
    std::cout << "clientes: " << players.size() << "\n";
    for (size_t i = 0; i < players.size(); i++) {
        std::cout << "action: " << players[i]->popAction().getAction() << "\n";
        CarState* state_received = interp.interpret(players[i]->popAction());
        players[i]->updateCarState(state_received);
    }
}
