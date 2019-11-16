#include <iostream>
#include "../include/ModelUpdater.h"

ModelUpdater::ModelUpdater(Socket &socket, ModelMonitor &modelMonitor, Drawer* drawerThread) :
        socket(socket), modelMonitor(modelMonitor), drawer(drawerThread) {
    std::string welcome =  receive(); // Recibe mensaje de bienvenida
    std::cout << welcome;
    std::string carStr = receive(); // Recibo mi auto
    std::cout << "Auto: " << carStr;
    std::string trackNames = receive(); //Recibo todos los nombres de las pistas
    std::cout << "Pistas: " << trackNames;

    modelMonitor.setTrackNames(trackNames);
    modelMonitor.setMyColor(carStr);
    modelMonitor.updateCar(carStr);
}

ModelUpdater::~ModelUpdater() {

}

void ModelUpdater::run() {
    running = true;
    while (running) {
        if (running && modelMonitor.getGameState() == mainMenu) {
            std::string text = receive(); //Recibo cambio de estado o la pista serializada
            if (text[0] == 'G') {
                std::string text = receive();
                printf("text1: %s\n", text.c_str());
                modelMonitor.setGameState(text);
            } else {
                Track track = Track(text); //Creo la pista
                modelMonitor.setTrack(track.getTrackPartData());
            }
        }
        while (running && modelMonitor.getGameState() == waitingPlayers) {
            std::string text = receive();
            if (text[0] == 'G') {
                std::string text = receive();
                printf("text2: %s\n", text.c_str());
                modelMonitor.setGameState(text);
            } else {
                modelMonitor.updateCar(text);
            }
        }
        while (running && modelMonitor.getGameState() == startCountdown) {
            std::string text = receive();
            if (text[0] == 'G') {
                std::string text = receive();
                printf("text3: %s\n", text.c_str());
                modelMonitor.setGameState(text);
            } else {
                //printf("recibo: %s", text.c_str());
                modelMonitor.updateCar(text);
            }
        }
        while (running && modelMonitor.getGameState() == playing) {
            try {
                std::string text = receive();
                //printf("text4: %s\n", text.c_str());
                if (text[0] == 'G') {
                    std::string text = receive();
                    modelMonitor.setGameState(text);
                }
                //printf("recibo: %s", text.c_str());
                modelMonitor.updateCar(text);
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
    return str_resp + "\n";
}
