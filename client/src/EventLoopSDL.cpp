#include "../include/EventLoopSDL.h"

#define FAKE_KEYDOWN 1

EventLoopSDL::EventLoopSDL(ThreadSafeQueue &queue, Drawer *drawerThread,
                           ModelMonitor &modelMonitor)
        : queue(queue), drawer(drawerThread), modelMonitor(modelMonitor),
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

void EventLoopSDL::run() {
    this->running = true;
    while (running) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (modelMonitor.getGameState()) {
            case mainMenu:
                quitAndResize(event);
                drawer->getMatchWindow().updateMatchButtons(&event);
                if (drawer->getMatchWindow().isReady()) {
                    this->queue.push(drawer->getMatchWindow().serializeData());
                }
                break;
            case selectingTrack:
                quitAndResize(event);
                break;
            case selectingCar:
                quitAndResize(event);
                break;
            case waitingPlayers:
                quitAndResize(event);
                break;
            case startCountdown:
                quitAndResize(event);
                if (!luaPlaying && drawer->getMatchWindow().isReady() &&
                    drawer->getMatchWindow().isLuaSelected()) {
                    luaPlaying = true;
                    printf("AI Playing\n");

                    std::string text = "T,9,10,16,16,20,classic,0,1500,0,100,1500,0,200,1500,0,300,1500,0,400,1500,0,500,1500,0,600,1500,0,700,1500,0,800,1500,0,900,1500,0,1000,1500,0,1100,1500,0,1200,1500,0,1300,1500,0,1400,1500,0,1500,1500,0,0,1400,0,100,1400,0,200,1400,0,300,1400,0,400,1400,0,500,1400,0,600,1400,0,700,1400,0,800,1400,0,900,1400,0,1000,1400,0,1100,1400,0,1200,1400,0,1300,1400,0,1400,1400,0,1500,1400,0,0,1300,0,100,1300,0,200,1300,0,300,1300,0,400,1300,0,500,1300,0,600,1300,0,700,1300,0,800,1300,0,900,1300,0,1000,1300,0,1100,1300,0,1200,1300,0,1300,1300,0,1400,1300,0,1500,1300,0,0,1200,0,100,1200,0,200,1200,0,300,1200,0,400,1200,0,500,1200,0,600,1200,0,700,1200,0,800,1200,0,900,1200,0,1000,1200,0,1100,1200,0,1200,1200,0,1300,1200,0,1400,1200,0,1500,1200,0,0,1100,0,100,1100,0,200,1100,0,300,1100,0,400,1100,0,500,1100,0,600,1100,0,700,1100,0,800,1100,0,900,1100,0,1000,1100,0,1100,1100,0,1200,1100,0,1300,1100,0,1400,1100,0,1500,1100,0,0,1000,0,100,1000,0,200,1000,0,300,1000,0,400,1000,0,500,1000,9,600,1000,9,700,1000,9,800,1000,9,900,1000,9,1000,1000,0,1100,1000,0,1200,1000,0,1300,1000,0,1400,1000,0,1500,1000,0,0,900,0,100,900,0,200,900,0,300,900,0,400,900,1,500,900,5,600,900,5,700,900,5,800,900,5,900,900,5,1000,900,2,1100,900,0,1200,900,0,1300,900,0,1400,900,0,1500,900,0,0,800,0,100,800,0,200,800,0,300,800,11,400,800,6,500,800,10,600,800,10,700,800,10,800,800,10,900,800,10,1000,800,6,1100,800,12,1200,800,0,1300,800,0,1400,800,0,1500,800,0,0,700,0,100,700,0,200,700,0,300,700,11,400,700,6,500,700,0,600,700,0,700,700,0,800,700,0,900,700,0,1000,700,6,1100,700,12,1200,700,0,1300,700,0,1400,700,0,1500,700,0,0,600,0,100,600,0,200,600,0,300,600,11,400,600,6,500,600,9,600,600,9,700,600,9,800,600,9,900,600,9,1000,600,6,1100,600,12,1200,600,0,1300,600,0,1400,600,0,1500,600,0,0,500,0,100,500,0,200,500,0,300,500,0,400,500,3,500,500,5,600,500,5,700,500,5,800,500,5,900,500,7,1000,500,4,1100,500,0,1200,500,0,1300,500,0,1400,500,0,1500,500,0,0,400,0,100,400,0,200,400,0,300,400,0,400,400,0,500,400,10,600,400,10,700,400,10,800,400,10,900,400,10,1000,400,0,1100,400,0,1200,400,0,1300,400,0,1400,400,0,1500,400,0,0,300,0,100,300,0,200,300,0,300,300,0,400,300,0,500,300,0,600,300,0,700,300,0,800,300,0,900,300,0,1000,300,0,1100,300,0,1200,300,0,1300,300,0,1400,300,0,1500,300,0,0,200,0,100,200,0,200,200,0,300,200,0,400,200,0,500,200,0,600,200,0,700,200,0,800,200,0,900,200,0,1000,200,0,1100,200,0,1200,200,0,1300,200,0,1400,200,0,1500,200,0,0,100,0,100,100,0,200,100,0,300,100,0,400,100,0,500,100,0,600,100,0,700,100,0,800,100,0,900,100,0,1000,100,0,1100,100,0,1200,100,0,1300,100,0,1400,100,0,1500,100,0,0,0,0,100,0,0,200,0,0,300,0,0,400,0,0,500,0,0,600,0,0,700,0,0,800,0,0,900,0,0,1000,0,0,1100,0,0,1200,0,0,1300,0,0,1400,0,0,1500,0,0";
                    Track track = Track(text.substr(2, text.length()));
                    std::vector<TrackPartData> trackData = track.getTrackPartData();

                    std::string color = modelMonitor.getMyColor();
                    printf("color:%s\n", color);
                    this->luaIA.setTrack(trackData);
                    this->luaIA.setColor(color);

                    this->luaIA.run();
                }
                break;
            case playing:
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
