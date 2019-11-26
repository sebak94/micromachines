#include "../include/EventLoopSDL.h"

#define FAKE_KEYDOWN 1

EventLoopSDL::EventLoopSDL(BlockingQueue &queue, Drawer *drawerThread,
                           ModelMonitor &modelMonitor) :
        queue(queue), drawer(drawerThread), modelMonitor(modelMonitor),
        luaIA(queue, drawer, modelMonitor) {
}

EventLoopSDL::~EventLoopSDL() {}

void EventLoopSDL::enqueueKeyDownEvent(SDL_KeyboardEvent &keyEvent) {
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
        this->queue.push(drawer->getMatchWindow().serializeData());
        this->queue.push(drawer->getMatchWindow().getSelection());
    } else if (drawer->getMatchWindow().returnIsPressed()) {
        this->queue.push(MatchWindow::returnSerialized());
    }
}

void EventLoopSDL::run() {
    this->running = true;
    bool flushed = false;
    while (running) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (modelMonitor.getGameState()) {
            case mainMenu:
                flushed = false;
                quitAndResize(event);
                drawer->getMatchWindow().updateMatchButtons(&event);
                if (drawer->getMatchWindow().isModeSelected() &&
                    !selectionSent) {
                    this->queue.push(drawer->getMatchWindow().getSelection());
                    selectionSent = true;
                }
                break;
            case creating:
                selectionSent = false;
                quitAndResize(event);
                matchWindowInfo(event);
                break;
            case joining:
                selectionSent = false;
                quitAndResize(event);
                matchWindowInfo(event);
                break;
            case waitingPlayers:
                quitAndResize(event);
                break;
            case startCountdown:
                quitAndResize(event);
                if (!luaPlaying && drawer->getMatchWindow().isReady() &&
                    drawer->getMatchWindow().isLuaSelected()) {
                    luaPlaying = true;
                    this->luaIA.setTrack(this->modelMonitor.getTrack());
                    std::string color = modelMonitor.getMyColor();
                    this->luaIA.setColor(color);
                    this->luaIA.run();
                }
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
                if (!flushed) {
                    this->queue.push("L"); // to end "playing" receive
                    this->queue.push("I"); // to flush in "waitingEnd"
                }
                flushed = true;
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
