#include "../include/client_th.h"
#include "../include/model/cars/blue_car.h"
#include "../../common/include/socket_error.h"
#include "../../common/include/lock.h"
#include <unistd.h>
#include "iostream"
#include "vector"
#include "string"

#define SPEEDREDUCTIONFACTOR 0.9

ClientTh::ClientTh(Socket *peer, TrackList &tracks)
        : keep_talking(true),
          is_running(true),
          peer(peer),
          tracks(tracks) {
    sendWelcomeMsg();
    //sendCarData();
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
        //No mando los autos que ya terminaron, mando solo los otros autos
        //if (players[i]->getState() != waitingEnd) {
            std::string s = players[i]->car->serialize();
            send(s);
        //}
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
    std::cout << "match" << matchSelection << std::endl;
    tracks.readTracks();
    size_t pos = 0;
    //Ejemplo de protocolo para crear: nameTrack,2\n
    //Ejemplo de protocolo para unirse: nameMatch,0\n
    //Cuando me estoy uniendo a una partida mando 0 como cantidad de jugadores
    if (state == creating) {
        std::string track = parse(matchSelection, pos, ','); //nombre de la pista
        std::string numberPlayers = parse(matchSelection, pos, '\n'); //cantidad de jugadores
        sendTrackData(tracks.getTrack(track).serialize());
    } else if (state == joining) {
        std::string match = parse(matchSelection, pos, ','); //numero de la partida
        gameNumber = stoi(match);
        sendTrackData(tracks.getTrack("").serialize());
        //aca en realidad hay que poner el nombre de la pista de la partida a la que quiere unirse
    }
}

void ClientTh::setPlayerMode() {
    std::string modeSelection;
    char action;
    while (action != '\n') {
        receive(&action);
        modeSelection += action;
    }
    if (modeSelection[0] == 'C')
        state = creating;
    else if (modeSelection[0] == 'J')
        state = joining;

}

void ClientTh::setCar(Car * matchCar) {
    car = matchCar;
}

void ClientTh::sendLapsData(std::string laps_serialized) {
    send(laps_serialized);
}

void ClientTh::sendGameState(GameState & previousSt, GameState & st) {
    if (previousSt != st) {
        previousSt = st;
        switch (st) {
            case mainMenu:
                send(std::string(MSG_ST_MAINMENU));
                break;
            case creating:
                send(std::string(MSG_ST_CREATING));
                break;
            case joining:
                send(std::string(MSG_ST_JOINING));
                break;
            case waitingPlayers:
                send(std::string(MSG_ST_WAITINGPLAYERS));
                break;
            case startCountdown:
                send(std::string(MSG_ST_COUNTDOWN));
                break;
            case playing:
                send(std::string(MSG_ST_PLAYING));
                break;
            case waitingEnd:
                send(std::string(MSG_ST_WAITINGEND));
                break;
            case gameEnded:
                send(std::string(MSG_ST_GAMEENDED));
                break;
        }
    }
}

void ClientTh::run() {
    std::string strState = MSG_ST_MAINMENU;
    GameState lastState = gameEnded;
    while (is_running){
        switch (state) {
            case mainMenu:
                //EUNI.updateGameState(lastState, state);
                sendGameState(lastState, state);
                break;
            case creating:
                sendGameState(lastState, state);
                break;
            case joining:
                sendGameState(lastState, state);
                break;
            case waitingPlayers:
                sendGameState(lastState, state);
                break;
            case startCountdown:
                sendGameState(lastState, state);
                break;
            case playing:
                sendGameState(lastState, state);
                while (keep_talking && state == playing) {
                    char action;
                    receive(&action);
                    Lock l(m);
                    actions.push(action);
                }
                break;
            case waitingEnd:
                sendGameState(lastState, state);
                break;
            case gameEnded:
                sendGameState(lastState, state);
                break;
        }
        usleep(500000);
    }
}

void ClientTh::processNextAction() {
    Lock l(m);
    while (!actions.empty()) {
        char a = actions.front();
        actions.pop();
        car->updateState(a);
    }
}

int ClientTh::getCarPosX() {
    Lock l(m);
    return car->getPosX();
}

int ClientTh::getCarPosY() {
    Lock l(m);
    return car->getPosY();
}

void ClientTh::reduceSpeed(float32 factor){
    Lock l(m);
    car->reduceSpeed(factor);
}

int ClientTh::getCarLastTrackID() {
    Lock l(m);
    return car->getTrackID();
}

void ClientTh::setState(GameState s) {
    this->state = s;
}

void ClientTh::updateCar() {
    if (state == playing) {
        car->update();
    }
}

void ClientTh::newCarPosition(Point point) {
    car->newPos(point);
}

void ClientTh::updateLaps() {
    car->updateLaps();
}

void ClientTh::updateLastTrackID(int ID) {
    car->setTrackID(ID);
}

void ClientTh::receive(char *action) {
    try {
        peer->Receive(action, 1);
    } catch (const SocketError &e) {
        keep_talking = false;
        is_running = false;
        std::cout << e.what() << "\n";
    }
}

void ClientTh::receiveActionPlugin(char *action) {
    Lock l(m);
    this->actions.push(action[0]);
}

void ClientTh::send(std::basic_string<char> response) {
    try {
        const char *resp = response.c_str();
        peer->Send(resp, response.length());
    } catch (const SocketError &e) {
        keep_talking = false;
        is_running = false;
        std::cout << e.what() << "\n";
    }
}

void ClientTh::setAvailableGames(std::string g) {
    Lock l(m);
    if (g != availableGames)
        availableGames = g;
}

void ClientTh::sendAvailableGames() {
    Lock l(m);
    send(availableGames);
}

void ClientTh::stop() {
    keep_talking = false;
    peer->Release();
}

bool ClientTh::isDead() {
    return !is_running;
}

GameState ClientTh::getState() {
    Lock l(m);
    return state;
}

int ClientTh::getGameNumber() {
    Lock l(m);
    return gameNumber;
}

int ClientTh::getLaps() {
    return car->getLaps();
}

std::string ClientTh::carColor() {
    return car->getColor();
}

void ClientTh::setWinners(std::vector<std::string> w) {
    winners = w;
}

void ClientTh::sendWinners() {
    std::string winStr = "W,";
    for (int i = 0; i < winners.size(); i++) {
        winStr += winners[i] + ',';
    }
    winStr.erase(winStr.length()-1); //borro la ultima coma
    winStr.append("\n");
    send(winStr);
}

ClientTh::~ClientTh() {
    //delete car;
    //Por ahora hago el delete de todos los autos en acceptor_th
}
