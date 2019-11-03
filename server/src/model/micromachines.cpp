#include "../../include/model/micromachines.h"
#include "../../include/model/interpreter.h"
#include "../../include/model/cars/car.h"
#include "../../include/client_th.h"
#include <vector>
#include "iostream"

void Micromachines::update() {
    for (size_t i = 0; i < players.size(); i++) {
        players[i]->updateCar();
    }
}

void Micromachines::addClient(ClientTh *client) {
    players.push_back(client);
}

void Micromachines::updatePlayersState() {
    Interpreter interp;
    for (size_t i = 0; i < players.size(); i++) {
        std::cout << "action: " << players[i]->popAction().getAction() << "\n";
        CarState* state_received = interp.interpret(players[i]->popAction());
        players[i]->updateCarState(state_received);
    }
}
