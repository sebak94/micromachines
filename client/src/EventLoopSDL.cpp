#include "../include/EventLoopSDL.h"

EventLoopSDL::EventLoopSDL(ThreadSafeQueue &queue) : queue(queue) {}

EventLoopSDL::~EventLoopSDL() {}

void EventLoopSDL::enqueueKeyboardEvent(SDL_KeyboardEvent& keyEvent) {
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

void EventLoopSDL::run() {
    this->running = true;
    while (running) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_KEYDOWN:
                enqueueKeyboardEvent((SDL_KeyboardEvent&)event);
                break;
            case SDL_QUIT:
                this->queue.push("Q"); //encolo una Q para finalizar
                this->running = false;
                break;
        }
    }
}
