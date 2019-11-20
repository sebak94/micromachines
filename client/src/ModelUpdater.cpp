#include <iostream>
#include "../include/ModelUpdater.h"

ModelUpdater::ModelUpdater(Socket &socket, ModelMonitor &modelMonitor, Drawer* drawerThread) :
        socket(socket), modelMonitor(modelMonitor), drawer(drawerThread) {
    std::string welcome =  receive(); // Recibe mensaje de bienvenida
    std::cout << welcome;
}

ModelUpdater::~ModelUpdater() {

}

bool ModelUpdater::updateState(std::string &received) {
    std::cout << received << std::endl;
    if (received[0] == 'G') {
        modelMonitor.setGameState(received);
        received = receive();
        return true;
    } else {
        return false;
    }
}

void ModelUpdater::run() {
    running = true;
    while (running) {
        std::string received{};
        if (running && modelMonitor.getGameState() == mainMenu) {
            received = receive(); // cambio de estado o la pista serializada
            updateState(received);
        }
        if (running && modelMonitor.getGameState() == creating) {
            modelMonitor.setTrackNames(received);
            Track track = Track(receive());
            modelMonitor.setTrack(track.getTrackPartData());
            received = receive();
            modelMonitor.setMyColor(received);
            modelMonitor.updateCar(received);
            modelMonitor.setGameState(waitingPlayers);
        }
        if (running && modelMonitor.getGameState() == joining) {
            modelMonitor.setMatchNames(received);
            receive();
            Track track = Track(receive());
            modelMonitor.setTrack(track.getTrackPartData());
            received = receive();
            modelMonitor.setMyColor(received);
            modelMonitor.updateCar(received);
            modelMonitor.setGameState(waitingPlayers);
        }
        while (running && modelMonitor.getGameState() == waitingPlayers) {
            std::string received = receive();
            updateState(received);
        }
        while (running && modelMonitor.getGameState() == startCountdown) {
            std::string received = receive();
            updateState(received);
            modelMonitor.updateCar(received);
        }
        while (running && modelMonitor.getGameState() == playing) {
            try {
                std::string received = receive();
                updateState(received);
                modelMonitor.updateCar(received);
            } catch (std::exception &e) {
                running = false;
                drawer->stop();
            }
        }
    }

}

void ModelUpdater::stop() {

}

std::string ModelUpdater::receive() {
    std::vector<char> response;
    char c;
    this->socket.Receive(&c, 1);
    while (c != '\n') {
        response.push_back(c);
        this->socket.Receive(&c, 1);
    }
    std::string str_resp(response.begin(), response.end());
    std::cout << str_resp << std::endl;
    return str_resp + "\n";
}
