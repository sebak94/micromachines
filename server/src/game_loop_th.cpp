#include "../include/game_loop_th.h"
#include "../include/model/micromachines.h"
#include <cstdint>
#include <unistd.h>

#define TICKS_PER_SECOND 60
#define SKIP_TICKS 1000 / TICKS_PER_SECOND
#define MAX_FRAMESKIP 10
#define MICROSECS_WAIT 16000 //seria que en un segundo se dibujen aprox 60 veces

GameLoopTh::GameLoopTh(Micromachines &micromachines) :
        running(true), micromachines(micromachines) {
    this->loader.load_dynamic_libraries();
}

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
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast
                <std::chrono::milliseconds>(end - begin);
            micromachines.world->Step(1.0/30.0, 5, 5);
            next_game_tick += SKIP_TICKS;
            loops++;
        }

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast
            <std::chrono::milliseconds>(end - begin);
        /*std::this_thread::sleep_for(
                std::chrono::seconds(1 / TICKS_PER_SECOND) - duration);*/
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        int microsecsPassed = std::chrono::duration_cast<std::chrono::microseconds>(
                duration).count();

        micromachines.sendNewStateToPlayers();
        usleep(MICROSECS_WAIT - microsecsPassed);

        this->executeLibraries();
    }
}

void GameLoopTh::executeLibraries() {
    if ((this->game_loops % 1000) == 0) {
        std::vector<char *> commands;
        this->loader.execute(commands);
        for (auto &i : commands)
            micromachines.changeCarState(i);
        this->game_loops = 1;
    } else {
        this->game_loops++;
    }
}

void GameLoopTh::stop() {
    running = false;
}

GameLoopTh::~GameLoopTh() {}
