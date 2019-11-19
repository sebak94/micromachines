#include <iostream>
#include "../include/ModelUpdater.h"

ModelUpdater::ModelUpdater(Socket &socket, ModelMonitor &modelMonitor, Drawer* drawerThread) :
        socket(socket), modelMonitor(modelMonitor), drawer(drawerThread) {
    std::string welcome =  receive(); // Recibe mensaje de bienvenida
    std::cout << welcome;
    /*std::string carStr = receive(); // Recibo mi auto
    std::cout << "Auto: " << carStr;
    std::string trackNames = receive(); //Recibo todos los nombres de las pistas
    std::cout << "Pistas: " << trackNames;
    std::string lapsStr = receive(); // Recibe cantidad de vueltas
    std::cout << lapsStr;*/

    /*modelMonitor.setTrackNames(trackNames);
    modelMonitor.setTotalLaps(lapsStr);
    modelMonitor.setMyColor(carStr);
    modelMonitor.updateCar(carStr);*/
}

ModelUpdater::~ModelUpdater() {

}

bool ModelUpdater::updateState(std::string &received) {
    std::cout << received << std::endl;
    if (received[0] == 'G') {
        received = receive();
        std::cout << received << std::endl;
        modelMonitor.setGameState(received);
        received = receive();
        std::cout << received << std::endl;
        return true;
    } else {
        return false;
    }
}

void ModelUpdater::run() {
    running = true;
    /*modelMonitor.setTrackNames(trackNames);
    modelMonitor.setTotalLaps(lapsStr);
    modelMonitor.setMyColor(carStr);
    modelMonitor.updateCar(carStr);*/
    while (running) {
        std::string received{};
        if (running && modelMonitor.getGameState() == mainMenu) {
            received = receive(); //Recibo cambio de estado o la pista serializada
            updateState(received);
            //Track track = Track(received); //Creo la pista
            //modelMonitor.setTrack(track.getTrackPartData());
        }
        if (running && modelMonitor.getGameState() == creating) {
            //std::string received = receive(); //Recibo cambio de estado o la pista serializada
            updateState(received);
            modelMonitor.setTrackNames(received);
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
            modelMonitor.updateCar(received);
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
    return str_resp + "\n";
}
