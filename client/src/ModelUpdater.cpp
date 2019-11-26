#include <iostream>
#include "../include/ModelUpdater.h"

ModelUpdater::ModelUpdater(Socket &socket, ModelMonitor &modelMonitor, Drawer* drawerThread) :
        socket(socket), modelMonitor(modelMonitor), drawer(drawerThread) {
    std::string welcome =  receive(); // Recibe mensaje de bienvenida
    std::cout << welcome;
}

ModelUpdater::~ModelUpdater() {}

bool ModelUpdater::updateState(std::string &received) {
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
        //try {
            std::string text = receive(); //Recibo cambio de estado u otra cosa
            if (text[0] == 'G') {
                //Si recibi cambio de estado lo actualizo
                printf("text: %s\n", text.c_str());
                modelMonitor.setGameState(text);
            } else {
                //Si recibi otra cosa, depende del estado actual lo que voy a hacer
                if (modelMonitor.getGameState() == mainMenu) {
                    modelMonitor.setTrackNames(text);
                    text = receive();
                    modelMonitor.setMatchNames(text.substr(2, text.length()));
                } else if (modelMonitor.getGameState() == creating) {
                    modelMonitor.reset();
                    Track track = Track(text);
                    modelMonitor.setTrack(track.getTrackPartData());
                    text = receive();
                    modelMonitor.createModifiers(text);
                    text = receive();
                    modelMonitor.setMyColor(text);
                    modelMonitor.updateCar(text);
                    text = receive();
                    modelMonitor.setTotalLaps(text);
                    modelMonitor.setGameState(waitingPlayers);
                } else if (modelMonitor.getGameState() == joining) {
                    modelMonitor.reset();
                    Track track = Track(text);
                    modelMonitor.setTrack(track.getTrackPartData());
                    text = receive();
                    modelMonitor.createModifiers(text);
                    text = receive();
                    modelMonitor.setMyColor(text);
                    modelMonitor.updateCar(text);
                    text = receive();
                    modelMonitor.setTotalLaps(text);
                    modelMonitor.setGameState(waitingPlayers);
                } else if (modelMonitor.getGameState() == waitingPlayers
                        || modelMonitor.getGameState() == startCountdown) {
                    if (text[0] == 'M') {
                        modelMonitor.updateModifiers(text);
                    } else {
                        modelMonitor.updateCar(text);
                    }
                } else if (modelMonitor.getGameState() == playing
                        || modelMonitor.getGameState() == waitingEnd
                        || modelMonitor.getGameState() == gameEnded) {
                    if (text[0] == 'W') {
                        modelMonitor.updateMatchResults(text.substr(2, text.length()));
                    } else if (text[0] == 'M') {
                        modelMonitor.updateModifiers(text);
                    } else {
                        modelMonitor.updateCar(text);
                    }
                }
            }
        /*} catch (std::exception &e) {
            printf("ModelUpdater::run() exception catched: %s\n", e.what());
            running = false;
            drawer->stop();
        }*/
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
