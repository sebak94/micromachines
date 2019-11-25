#include "../include/EventLoopSDL.h"

#define FAKE_KEYDOWN 1

EventLoopSDL::EventLoopSDL(BlockingQueue &queue, Drawer *drawerThread, ModelMonitor &modelMonitor)
        : queue(queue), drawer(drawerThread), modelMonitor(modelMonitor) {}

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
            modelMonitor.setBrake();
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
            modelMonitor.unsetBrake();
            break;
    }
}

void EventLoopSDL::quitAndResize(SDL_Event &event) {
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

void EventLoopSDL::matchWindowInfo(SDL_Event &event) {
    drawer->getMatchWindow().updateMatchButtons(&event);
    if (drawer->getMatchWindow().isReady()) {
        if (drawer->getMatchWindow().isLuaSelected()) {
            printf("AI Playing\n");
            luaPlaying = true;
            //acá lanzar el nuevo hilo de los eventos de lua (pero dejar este hilo corriendo)
        }
        this->queue.push(drawer->getMatchWindow().serializeData());
        this->queue.push(drawer->getMatchWindow().getSelection());
    }
}

void EventLoopSDL::run() {
    this->running = true;
    while (running) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (modelMonitor.getGameState()) {
            case mainMenu:
                quitAndResize(event);
                drawer->getMatchWindow().updateMatchButtons(&event);
                if (drawer->getMatchWindow().isModeSelected() && !selectionSent) {
                    this->queue.push(drawer->getMatchWindow().getSelection());
                    selectionSent = true;
                }
                break;
            case creating:
                quitAndResize(event);
                matchWindowInfo(event);
                break;
            case joining:
                quitAndResize(event);
                matchWindowInfo(event);
                break;
            case waitingPlayers:
                quitAndResize(event);
                break;
            case startCountdown:
                quitAndResize(event);
                break;
            case playing:
                selectionSent = false;
                quitAndResize(event);
                if (!luaPlaying) {
                    switch (event.type) {
                        case SDL_KEYDOWN:
                            enqueueKeyDownEvent((SDL_KeyboardEvent &) event);
                            break;
                        case SDL_KEYUP:
                            enqueueKeyUpEvent((SDL_KeyboardEvent &) event);
                            break;
                    }
                }
                break;
            case waitingEnd:
                quitAndResize(event);
                break;
            case gameEnded:
                quitAndResize(event);
                break;
        }
    }
}

void EventLoopSDL::stop() {

}
