#include "../include/game_loop_th.h"
#include "../include/micromachines.h"
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

        // processInput();

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
