#include "../include/client_th.h"
#include "../include/model/cars/blue_car.h"
#include "../../common/include/socket_error.h"
#include "../../common/include/lock.h"
#include "iostream"
#include "vector"
#include "string"

ClientTh::ClientTh(Socket *peer, Car *car, TrackList& tracks) : keep_talking(true),
                            is_running(true), peer(peer), car(car), tracks(tracks) {
    sendWelcomeMsg();
    sendCarData();
}

void ClientTh::sendWelcomeMsg() {
    std::string welcome_msg = "Bienvenido!\n";
    send(welcome_msg);
}

void ClientTh::sendCarData() {
    std::string car_msg = car->serialize();
    send(car_msg);
}

void ClientTh::sendAllCarsToPlayer(std::vector<ClientTh *> players) {
    for (size_t i = 0; i < players.size(); i++) {
        std::string s = players[i]->car->serialize();
        send(s);
    }
}

void ClientTh::sendTrackData(std::string track_serialized) {
    send(track_serialized);
}

void ClientTh::sendAllTrackNames(std::string tracks) {
    send(tracks);
}

std::string ClientTh::parse(const std::string &str, size_t &pos, const char delim) {
    std::string substr;
    size_t nextPos = str.find(delim, pos);
    size_t len = nextPos - pos;
    substr = str.substr(pos, len);
    pos = nextPos + 1;
    return substr;
}

void ClientTh::setMatch() {
    std::string matchSelection;
    char action;
    while (action != '\n') {
        receive(&action);
        matchSelection += action;
    }
    //printf("recibo: %s\n", matchSelection.c_str());

    size_t pos = 0;
    std::string word = parse(matchSelection, pos, ','); //C o J (create o join match)
    std::string track = parse(matchSelection, pos, ','); //nombre de la pista o partida
    //cantidad de jugadores, en el caso de unirse a una partida mando un 0
    std::string numberPlayers = parse(matchSelection, pos, '\n');
    sendTrackData(tracks.getTrack(track).serialize());
}

void ClientTh::run() {
    std::string strState = "G\nmainMenu\n";
    while (is_running){
        switch (state) {
            case mainMenu:
                setMatch();
                setState(waitingPlayers);
                break;
            case selectingTrack:
                strState = "G\nselectingTrack\n";
                send(strState);
                while (state == selectingTrack) {

                }
                break;
            case selectingCar:
                strState = "G\nselectingCar\n";
                send(strState);
                while (state == selectingCar) {

                }
                break;
            case waitingPlayers:
                strState = "G\nwaitingPlayers\n";
                send(strState);
                while (state == waitingPlayers) {

                }
                break;
            case startCountdown:
                strState = "G\nstartCountdown\n";
                send(strState);
                while (state == startCountdown) {

                }
                break;
            case playing:
                strState = "G\nplaying\n";
                send(strState);
                while (keep_talking) {
                    char action;
                    receive(&action);
                    Lock l(m);
                    actions.push(action);
                }
                break;
            case waitingEnd:
                strState = "G\nwaitingEnd\n";
                send(strState);
                while (state == waitingEnd) {

                }
                break;
            case gameEnded:
                strState = "G\ngameEnded\n";
                send(strState);
                while (state == gameEnded) {

                }
                break;
        }
    }
    is_running = false;
}

void ClientTh::processNextAction() {
    Lock l(m);
    while (!actions.empty()) {
        char a = actions.front();
        actions.pop();
        car->updateState(a);
    }
}

void ClientTh::setState(GameState s) {
    this->state = s;
}

void ClientTh::updateCar() {
    car->update();
}

void ClientTh::receive(char *action) {
    try {
        peer->Receive(action, 1);
    } catch (const SocketError &e) {
        keep_talking = false;
        std::cout << e.what() << "\n";
    }
}

void ClientTh::receiveActionPlugin(char *action) {
    Lock l(m);
    this->actions.push(action[0]);
}

void ClientTh::send(std::string &response) {
    try {
        const char *resp = response.c_str();
        peer->Send(resp, response.length());
    } catch (const SocketError &e) {
        keep_talking = false;
        std::cout << e.what() << "\n";
        throw e;
    }
}

void ClientTh::stop() {
    keep_talking = false;
    peer->Release();
}

bool ClientTh::isDead() {
    return !is_running;
}

GameState ClientTh::getState() {
    return state;
}

ClientTh::~ClientTh() {
    //delete car;
    //Por ahora hago el delete de todos los autos en acceptor_th
}
