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

void GameLoopTh::showMainMenu() {
    micromachines.setAllPlayersGameStates(waitingPlayers);
}

void GameLoopTh::waitForPlayers() {
    uint64_t next_game_tick = GetTickCountMs();
    uint64_t loops;
    while (micromachines.getPlayersNumber() < 2) {
        if (running) {
            auto begin = std::chrono::steady_clock::now();
            loops = 0;
            micromachines.updatePlayersState();
            updateWorld(next_game_tick, 0, loops, 1.0, 0, 5);
            micromachines.sendNewStateToPlayers();
            timeWait(MICROSECS_WAIT, begin);
        } else {
            break;
        }
    }
    micromachines.setAllPlayersGameStates(startCountdown);
}

void GameLoopTh::countdownWait() {
    if (running) {
        uint64_t next_game_tick = GetTickCountMs();
        uint64_t loops;
        countdownTime = SECSTOSTART * SECTOMICROSEC;  // us
        while (countdownTime > 0) {
            auto begin = std::chrono::steady_clock::now();
            loops = 0;
            micromachines.updatePlayersState();
            updateWorld(next_game_tick, SKIP_TICKS, loops, 1.0 / 60, 5, 5);
            micromachines.sendNewStateToPlayers();
            timeWait(MICROSECS_WAIT, begin);
            countdownTime -= MICROSECS_WAIT;
        }
        micromachines.setAllPlayersGameStates(playing);
    }
}

void GameLoopTh::play() {
    if (running) {
        uint64_t next_game_tick = GetTickCountMs();
        uint64_t loops;
        countdownTime = MAXRACETIME * SECTOMICROSEC;  // us
        while (countdownTime > 0) {
            auto begin = std::chrono::steady_clock::now();
            loops = 0;
            micromachines.updatePlayersState();
            updateWorld(next_game_tick, SKIP_TICKS, loops, 1.0 / 60, 5, 5);
            micromachines.sendNewStateToPlayers();
            timeWait(MICROSECS_WAIT/100, begin);

            // this->executeLibraries();
            countdownTime -= MICROSECS_WAIT;
        }
        micromachines.setAllPlayersGameStates(gameEnded);
        micromachines.setAllPlayersGameStates(mainMenu);
    }
}

void GameLoopTh::updateWorld(uint64_t &next_game_tick,
                             uint64_t skip_ticks,
                             uint64_t &loops,
                             float32 timeStep,
                             int32 velocityIterations,
                             int32 positionIterations) {
    while (GetTickCountMs() > next_game_tick && loops < MAX_FRAMESKIP) {
        micromachines.update();
        micromachines.world->Step(timeStep, velocityIterations, positionIterations);
        next_game_tick += skip_ticks;
        loops++;
    }
}

int GameLoopTh::timeElapsed(std::chrono::time_point<std::chrono::steady_clock> &start) {
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();  // us
}


bool GameLoopTh::timeWait(int timeToWait, std::chrono::time_point<std::chrono::steady_clock> &start) {
    int timeDiff = timeToWait - timeElapsed(start);
    if (timeDiff > 0) {
        usleep(timeDiff);
        return true;  // waited
    } else {
        return false;
    }
}


void GameLoopTh::run() {
    while (running) {
        showMainMenu();
        waitForPlayers();
        countdownWait();
        play();
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
    countdownTime = 0;
}

GameLoopTh::~GameLoopTh() {}
