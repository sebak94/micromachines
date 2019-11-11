#include "../include/EventLoopSDL.h"

#define FAKE_KEYDOWN 1

EventLoopSDL::EventLoopSDL(ThreadSafeQueue &queue, Drawer* drawerThread) : queue(queue), drawer(drawerThread) {}

EventLoopSDL::~EventLoopSDL() {}

void EventLoopSDL::enqueueKeyDownEvent(SDL_KeyboardEvent& keyEvent) {
    if (keyEvent.repeat == FAKE_KEYDOWN)
        return;

    switch (keyEvent.keysym.sym) {
        case SDLK_LEFT:
            this->queue.push("L"); //left
            break;
        case SDLK_RIGHT:
            this->queue.push("R"); //right
            break;
        case SDLK_UP:
            this->queue.push("A"); //accelerate
            break;
        case SDLK_DOWN:
            this->queue.push("B"); //break
            break;
    }
}

void EventLoopSDL::enqueueKeyUpEvent(SDL_KeyboardEvent &keyEvent) {
    switch (keyEvent.keysym.sym) {
        case SDLK_LEFT:
            this->queue.push("W"); //left
            break;
        case SDLK_RIGHT:
            this->queue.push("X"); //right
            break;
        case SDLK_UP:
            this->queue.push("Y"); //accelerate
            break;
        case SDLK_DOWN:
            this->queue.push("Z"); //break
            break;
    }
}

void EventLoopSDL::run() {
    this->running = true;
    while (running) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        drawer->updateFullScreenButton(&event);
        drawer->updateRecButton(&event);
        switch (event.type) {
            case SDL_KEYDOWN:
                enqueueKeyDownEvent((SDL_KeyboardEvent &) event);
                break;
            case SDL_KEYUP:
                enqueueKeyUpEvent((SDL_KeyboardEvent &) event);
                break;
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
}

void EventLoopSDL::stop() {

}
