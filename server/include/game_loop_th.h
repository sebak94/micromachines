#ifndef __GAME_LOOP_TH_H__
#define __GAME_LOOP_TH_H__

#include "../../common/include/thread.h"
#include "model/micromachines_th.h"
#include "loader.h"
#include <cstdint>

#define SECSTOSTART 5  // secs
#define MINTOMICROSEC 60000000  // secs
#define SECTOMICROSEC 1000000
#define PODIUMVIEWTIME 5000000  // us

class GameLoopTh: public Thread {
    private:
    bool running;
    MicroMachinesTh &micromachines;
    Loader loader;
    uint8_t game_loops = 1;
    double countdownTime = 0;
    Config config;
    modifierChrono modifTimer = std::chrono::steady_clock::now();

    uint64_t GetTickCountMs();
    void executeLibraries();

    public:
    explicit GameLoopTh(MicroMachinesTh &micromachines);
    virtual void run() override;
    virtual void stop() override;
    ~GameLoopTh();
    void waitForPlayers();
    void countdownWait();
    static int timeElapsed(std::chrono::time_point<std::chrono::steady_clock> &start);
    static bool timeWait(int timeToWait, std::chrono::time_point<std::chrono::steady_clock> &start);
    void updateWorld(uint64_t &next_game_tick, uint64_t skip_ticks, uint64_t &loops, float32 timeStep,
                     int32 velocityIterations, int32 positionIterations);
    void play();
    void showMainMenu();

    bool isRunning();
};

#endif
