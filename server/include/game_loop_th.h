#ifndef __GAME_LOOP_TH_H__
#define __GAME_LOOP_TH_H__

#include "../../common/include/thread.h"
#include "model/micromachines.h"
#include "loader.h"
#include <cstdint>

class GameLoopTh: public Thread {
    private:
    bool running;
    Micromachines &micromachines;
    Loader loader;
    uint8_t game_loops = 1;

    uint64_t GetTickCountMs();
    void executeLibraries();

    public:
    GameLoopTh(Micromachines &micromachines);
    virtual void run() override;
    virtual void stop() override;
    ~GameLoopTh();

    void waitForPlayers();
};

#endif
