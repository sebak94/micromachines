#ifndef __EVENTLOOPSDL_H__
#define __EVENTLOOPSDL_H__

#include "BlockingQueue.h"
#include "../../common/include/thread.h"
#include "Drawer.h"
#include <SDL2/SDL_events.h>

class EventLoopSDL : public Thread {
private:
    bool running;
    BlockingQueue &queue;
    Drawer* drawer;
    ModelMonitor &modelMonitor;
    bool luaPlaying = false;
    bool selectionSent = false;

public:
    EventLoopSDL(BlockingQueue &queue, Drawer *drawerThread, ModelMonitor &modelMonitor);
    ~EventLoopSDL();
    virtual void run() override;
    virtual void stop() override;

private:
    void enqueueKeyDownEvent(SDL_KeyboardEvent& keyEvent);
    void enqueueKeyUpEvent(SDL_KeyboardEvent& keyEvent);
    void quitAndResize(SDL_Event &event);
};

#endif
