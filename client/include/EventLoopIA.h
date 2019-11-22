#ifndef __EventLoopIA_H__
#define __EventLoopIA_H__

#include "ThreadSafeQueue.h"
#include "../../common/include/thread.h"
#include "Drawer.h"
#include <SDL2/SDL_events.h>
#include "./lua/ai.h"

class EventLoopIA : public Thread {
private:
    bool running;
    ThreadSafeQueue &queue;
    Drawer *drawer;
    ModelMonitor &modelMonitor;
    Ai lua_ai;

public:
    EventLoopIA(ThreadSafeQueue &queue, Drawer *drawerThread,
                ModelMonitor &modelMonitor);

    ~EventLoopIA();

    void setTrack(std::vector <TrackPartData> &track);

    void setColor(std::string &color);

    virtual void run() override;

    virtual void stop() override;
};

#endif
