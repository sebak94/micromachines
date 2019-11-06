#ifndef __EVENTLOOPSDL_H__
#define __EVENTLOOPSDL_H__

#include "ThreadSafeQueue.h"
#include "../../common/include/thread.h"
#include <SDL2/SDL_events.h>

class EventLoopSDL : public Thread {
private:
    bool running;
    ThreadSafeQueue &queue;

public:
    EventLoopSDL(ThreadSafeQueue &queue);
    ~EventLoopSDL();
    virtual void run() override;
    virtual void stop() override;

private:
    void enqueueKeyboardEvent(SDL_KeyboardEvent& keyEvent);
};

#endif
