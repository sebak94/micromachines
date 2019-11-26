#include "../include/game_loop_th.h"
#include "../include/model/micromachines_th.h"
#include <cstdint>
#include <unistd.h>

#define TICKS_PER_SECOND 60
#define SKIP_TICKS (1000/TICKS_PER_SECOND)
#define MAX_FRAMESKIP 10
#define MICROSECS_WAIT 16000 //seria que en un segundo se dibujen aprox 60 veces
#define MODIFIER_TIME 100000
#define RACETIME "race time [mins]"
#define REFRESH_FREQ "world refresh frequency [Hz]"

GameLoopTh::GameLoopTh(MicroMachinesTh &micromachines) :
        running(true), micromachines(micromachines),
        config(micromachines.getConfig()) {
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
    while (micromachines.getPlayersNumber() < micromachines.getTotalNumberPlayers() || micromachines.somePlayersInMainMenu()) {
        if (running) {
            auto begin = std::chrono::steady_clock::now();
            loops = 0;
            micromachines.updatePlayersState();
            updateWorld(next_game_tick, 0, loops, 1.0 / config.getAsFloat(REFRESH_FREQ), 1 , 5);
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
        countdownTime = (SECSTOSTART) * (SECTOMICROSEC);  // us
        while (countdownTime > 0) {
            auto begin = std::chrono::steady_clock::now();
            uint64_t loops = 0;
            micromachines.updatePlayersState();
            updateWorld(next_game_tick, SKIP_TICKS, loops, 1.0 / config.getAsFloat(REFRESH_FREQ), 5, 5);
            micromachines.sendNewStateToPlayers();
            timeWait(MICROSECS_WAIT, begin);
            countdownTime -= MICROSECS_WAIT;
        }
        micromachines.setAllPlayersGameStates(playing);
    }
}

void GameLoopTh::play() {
    bool ending = false;
    uint64_t next_game_tick = GetTickCountMs();
    countdownTime = (MINTOMICROSEC) * config.getAsInt(RACETIME);  // us
    while (running && countdownTime > 0 && !micromachines.allPlayersGameEnded()) {
        auto begin = std::chrono::steady_clock::now();
        uint64_t loops = 0;
        micromachines.updatePlayersState();
        micromachines.monitorTrack();
        micromachines.updateModifiersPosition();
        if (timeElapsed(modifTimer) > MODIFIER_TIME) {
            micromachines.throwModifier();
            modifTimer = std::chrono::steady_clock::now();
        }
        updateWorld(next_game_tick, SKIP_TICKS, loops, 1.0 / config.getAsFloat(REFRESH_FREQ), 5, 5);
        micromachines.sendNewStateToPlayers();
        micromachines.sendModifiersToPlayers();
        micromachines.updateWinners();
        micromachines.sendWinners();
        timeWait(MICROSECS_WAIT, begin);

        // this->executeLibraries();
        countdownTime -= MICROSECS_WAIT;
        if (micromachines.allPlayersWaitingEnd() && !ending) {
            countdownTime = (PODIUMVIEWTIME)*0.9;  // cuando llegan todos en 4,5 segs corta animacion
            ending = true;
        }
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
    showMainMenu();
    waitForPlayers();
    countdownWait();
    play();
    stop();
}

bool GameLoopTh::isRunning() {
    return running;
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
