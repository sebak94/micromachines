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
    std::string lapsStr = receive(); // Recibe cantidad de vueltas
    std::cout << lapsStr;

    modelMonitor.setTrackNames(trackNames);
    modelMonitor.setTotalLaps(lapsStr);
    modelMonitor.setMyColor(carStr);
    modelMonitor.updateCar(carStr);
}

ModelUpdater::~ModelUpdater() {

}

void ModelUpdater::run() {
    running = true;
    while (running) {
        try {
            std::string text = receive(); //Recibo cambio de estado u otra cosa
            if (text[0] == 'G') {
                //Si recibi cambio de estado lo actualizo
                std::string text = receive();
                printf("text: %s\n", text.c_str());
                modelMonitor.setGameState(text);
            } else {
                //Si recibi otra cosa, depende del estado actual lo que voy a hacer
                if (modelMonitor.getGameState() == mainMenu) {
                    Track track = Track(text);
                    modelMonitor.setTrack(track.getTrackPartData());
                } else if (modelMonitor.getGameState() == waitingPlayers
                        || modelMonitor.getGameState() == startCountdown) {
                    modelMonitor.updateCar(text);
                } else if (modelMonitor.getGameState() == playing
                        ||modelMonitor.getGameState() == waitingEnd) {
                    if (text[0] == 'W') {
                        modelMonitor.updateMatchResults(text.substr(2, text.length()));
                    } else {
                        modelMonitor.updateCar(text);
                    }
                }
            }
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
