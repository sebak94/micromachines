#include "../include/game_loop_th.h"
#include "../include/model/micromachines.h"
#include <cstdint>

#define TICKS_PER_SECOND 60
#define SKIP_TICKS 1000 / TICKS_PER_SECOND
#define MAX_FRAMESKIP 10

GameLoopTh::GameLoopTh(Micromachines &micromachines) :
        running(true), micromachines(micromachines) {}

uint64_t GameLoopTh::GetTickCountMs() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_nsec / 1000000) + ((uint64_t) ts.tv_sec * 1000ull);
}

void GameLoopTh::run() {
    uint64_t next_game_tick = GetTickCountMs();
    uint64_t loops;

    while (running) {
        auto begin = std::chrono::steady_clock::now();

        loops = 0;
        micromachines.updatePlayersState();

        while (GetTickCountMs() > next_game_tick && loops < MAX_FRAMESKIP) {
            micromachines.update();
            next_game_tick += SKIP_TICKS;
            loops++;
        }

        micromachines.sendNewStateToPlayers();

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                end - begin);
        /*std::this_thread::sleep_for(
                std::chrono::seconds(1 / TICKS_PER_SECOND) - duration);*/
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void GameLoopTh::stop() {
    running = false;
}

GameLoopTh::~GameLoopTh() {}
