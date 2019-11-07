#include <iostream>
#include "../include/ModelUpdater.h"

ModelUpdater::ModelUpdater(Socket &socket, ModelMonitor &modelMonitor, Thread* drawerThread) : socket(socket), modelMonitor(modelMonitor) {
    std::string welcome =  receive(); // Recibe mensaje de bienvenida
    std::cout << welcome;
    std::string carStr = receive(); // Recibe auto
    std::cout << carStr;
    std::string trackStr = receive(); // Recibe pista
    std::cout << trackStr;

    Track track = Track(trackStr);
    modelMonitor.setTrack(track.getTrackPartData());
    modelMonitor.setMyColor(carStr);
    modelMonitor.addCar(carStr);
    drawer = drawerThread;
}

ModelUpdater::~ModelUpdater() {

}

void ModelUpdater::run() {
    running = true;
    while (running) {
        try {
            std::string text = receive();
            printf("recibo: %s", text.c_str());
            modelMonitor.updateCar(text); //actualizar modelo
        } catch (std::exception &e) {
            printf("murio el socket en el updater\n");
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
