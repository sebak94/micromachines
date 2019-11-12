#include <iostream>
#include "../include/ModelUpdater.h"

ModelUpdater::ModelUpdater(Socket &socket, ModelMonitor &modelMonitor, Drawer* drawerThread) :
        socket(socket), modelMonitor(modelMonitor), drawer(drawerThread) {
    std::string welcome =  receive(); // Recibe mensaje de bienvenida
    std::cout << welcome;
    std::string carStr = receive(); // Recibo mi auto
    std::cout << carStr;
    std::string trackStr = receive(); // Recibe pista
    std::cout << trackStr;

    Track track = Track(trackStr);
    modelMonitor.setTrack(track.getTrackPartData());
    modelMonitor.setMyColor(carStr);
    modelMonitor.updateCar(carStr);
}

ModelUpdater::~ModelUpdater() {

}

void ModelUpdater::run() {
    while (modelMonitor.getGameState() == mainMenu) {
        std::cout << "main" << std::endl;
        std::string text = receive();
        if (text[0] == 'G') {
            std::string text = receive();
            modelMonitor.setGameState(text);
        } else {
            modelMonitor.updateCar(text);
        }
    }
    while (modelMonitor.getGameState() == waitingPlayers) {
        std::cout << "waitingplayers" << std::endl;
        std::string text = receive();
        if (text[0] == 'G') {
            std::string text = receive();
            modelMonitor.setGameState(text);
        } else {
            modelMonitor.updateCar(text);
        }
    }
    while (modelMonitor.getGameState() == startCountdown) {
        std::cout << "countdown" << std::endl;
        std::string text = receive();
        if (text[0] == 'G') {
            std::string text = receive();
            modelMonitor.setGameState(text);
        } else {
            printf("recibo: %s", text.c_str());
            modelMonitor.updateCar(text);
        }
    }
    running = true;
    while (running) {
        std::cout << "running" << std::endl;
        try {
            std::string text = receive();
            if (text[0] == 'G') {
                std::string text = receive();
                modelMonitor.setGameState(text);
            }
            printf("recibo: %s", text.c_str());
            modelMonitor.updateCar(text);
        } catch (std::exception &e) {
            running = false;
            drawer->stop();
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
    return str_resp + "\n";
}
