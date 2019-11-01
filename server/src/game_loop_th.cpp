#include "../include/game_loop_th.h"
#include "../include/model/micromachines.h"
#include "../include/model/interpreter.h"
#include <chrono>

#define MS_PER_UPDATE 16 // 60 FPS

GameLoopTh::GameLoopTh(Micromachines micromachines): running(true),
    micromachines(micromachines) {}

void GameLoopTh::run() {
    auto previous = std::chrono::system_clock::now();
    double lag = 0.0;
    while (running) {
        auto current = std::chrono::system_clock::now();
        auto elapsed = current - previous;
        previous = current;
        lag += elapsed.count();

        std::vector<ClientTh*> clients = micromachines.clients;
        for (size_t i = 0; i < clients.size; i++) {
            ClientTh* client = clients[i];
            Interpreter interp;
            CarState* newCarState = interp.interpret(client->popAction());
            client->updateCarState(newCarState);
        }

        while (lag >= MS_PER_UPDATE) {
            micromachines.update();
            lag -= MS_PER_UPDATE;
        }

        // render();
    }
}

void GameLoopTh::stop() {
    running = false;
}

GameLoopTh::~GameLoopTh() {}
