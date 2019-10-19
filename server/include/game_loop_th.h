#ifndef __GAME_LOOP_TH_H__
#define __GAME_LOOP_TH_H__

#include "../../common/include/thread.h"
#include "./micromachines.h"

class GameLoopTh: public Thread {
    private:
    bool running;
    Micromachines micromachines;

    public:
    GameLoopTh(Micromachines micromachines);
    virtual void run() override;
    virtual void stop() override;
    ~GameLoopTh();
};

#endif
