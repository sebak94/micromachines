#ifndef __EVENTLOOPSDL_H__
#define __EVENTLOOPSDL_H__

#include "ThreadSafeQueue.h"
#include "../../common/include/thread.h"
#include "Drawer.h"
#include <SDL2/SDL_events.h>

class EventLoopSDL : public Thread {
private:
    bool running;
    ThreadSafeQueue &queue;
    Drawer* drawer;

public:
    EventLoopSDL(ThreadSafeQueue &queue, Drawer* drawerThread);
    ~EventLoopSDL();
    virtual void run() override;
    virtual void stop() override;

private:
    void enqueueKeyDownEvent(SDL_KeyboardEvent& keyEvent);
    void enqueueKeyUpEvent(SDL_KeyboardEvent& keyEvent);
};

#endif