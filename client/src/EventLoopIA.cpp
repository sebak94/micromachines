#include "../include/EventLoopIA.h"

#define FAKE_KEYDOWN 1

EventLoopIA::EventLoopIA(ThreadSafeQueue &queue, Drawer *drawerThread) :
        queue(queue), drawer(drawerThread) {}

void EventLoopIA::setTrack(std::vector <TrackPartData> &track) {
    this->lua_ai.setTrack(track);
}

EventLoopIA::~EventLoopIA() {}

void EventLoopIA::run() {
    /*
    this->running = true;
    while (running) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        drawer->updateFullScreenButton(&event);
        drawer->updateRecButton(&event);
        switch (event.type) {
            case SDL_QUIT:
                this->queue.push("Q"); //encolo una Q para finalizar
                this->running = false;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        drawer->resize(event.window.data1, event.window.data2);
                        break;
                }
                break;
        }
    }
     */
}

void EventLoopIA::stop() {

}
