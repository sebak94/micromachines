#include "../include/client_th.h"
#include "../include/blocking_queue.h"
#include "../include/player_action.h"
#include "../include/model/cars/blue_car.h"
#include "../../common/include/socket.h"
#include "../../common/include/socket_error.h"
#include "iostream"
#include "vector"
#include "string"

ClientTh::ClientTh(Socket *peer, Micromachines &micromachines):
    keep_talking(true), is_running(true), peer(peer),
    micromachines(micromachines) {
    car = new BlueCar();
    micromachines.addCar(car);
    std::string welcome_msg = "Bienvenido!\n";
    send(welcome_msg);
}

void ClientTh::run() {
    while (keep_talking) {
        try {
            char action;
            receive(&action);
            actions.push(PlayerAction(action));
        } catch(const SocketError &e) {
            keep_talking = false;
            std::cout << e.what() << "\n";
        }
    }

    is_running = false;
}

void ClientTh::receive(char *action) {
    peer->Receive(action, 1);
}

void ClientTh::send(std::string &response) {
    const char *resp = response.c_str();
    peer->Send(resp, response.length());
}

void ClientTh::stop() {
    keep_talking = false;
    peer->Release();
}

bool ClientTh::isDead() {
    return !is_running;
}

ClientTh::~ClientTh() {
    micromachines.removeCar(car);
    delete car;
}
