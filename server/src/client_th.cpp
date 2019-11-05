#include "../include/client_th.h"
#include "../include/model/cars/blue_car.h"
#include "../include/model/cars/states/car_state.h"
#include "../../common/include/socket.h"
#include "../../common/include/socket_error.h"
#include "iostream"
#include "vector"
#include "string"

ClientTh::ClientTh(Socket *peer): keep_talking(true), is_running(true),
    peer(peer), car(new BlueCar()) {
    sendWelcomeMsg();
    sendTrackData();
    sendCarData();
}

void ClientTh::sendWelcomeMsg() {
    std::string welcome_msg = "Bienvenido!\n";
    send(welcome_msg);
}

void ClientTh::sendTrackData() {

}

void ClientTh::sendCarData() {
    std::string car_msg = car->serialize();
    send(car_msg);
}

void ClientTh::sendTrackData(std::string track_serialized) {
    send(track_serialized);
}

void ClientTh::run() {
    while (keep_talking) {
        char action;
        receive(&action);
        actions.push(action);
    }

    is_running = false;
}

void ClientTh::processNextAction() {
    if (!actions.empty()) {
        char a = actions.front();
        actions.pop();
        car->updateState(a);
    }
}

void ClientTh::updateCar() {
    car->update();
}

void ClientTh::receive(char *action) {
    try {
        peer->Receive(action, 1);
    } catch(const SocketError &e) {
        keep_talking = false;
        std::cout << e.what() << "\n";
    }
}

void ClientTh::send(std::string &response) {
    try {
        const char *resp = response.c_str();
        peer->Send(resp, response.length());
    } catch(const SocketError &e) {
        keep_talking = false;
        std::cout << e.what() << "\n";
    }
}

void ClientTh::stop() {
    keep_talking = false;
    peer->Release();
}

bool ClientTh::isDead() {
    return !is_running;
}

ClientTh::~ClientTh() {
    delete car;
}
