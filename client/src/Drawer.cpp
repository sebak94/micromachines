#include <SDL2/SDL_events.h>
#include <iostream>
#include "../include/Drawer.h"
#include "../include/sdl/SdlAnimation.h"
#include "../../record/include/Record.h"
#include "../../common/include/Error.h"
#include "../include/SoundTh.h"
#include <unistd.h>

#define FPS "fps limit"
#define FULLSCREENBUTTON "../common/images/fullscreen.png"
#define RECBUTTON "../common/images/buttons/recButton.png"
#define VIDEOPATH "./recorded.mp4"
#define VIDEOFPS "rec fps limit"
#define FULLSCN "fullscreen"
#define DRAW_DISTANCE "draw distance [4 - 8]"
#define SECTOMICROSEC 1000000.0

Drawer::Drawer(ModelMonitor &modelMonitor) :
        window(WIDTH, HEIGHT),
        loader(window, pictures, trackPictures),
        modelMonitor(modelMonitor),
        camera(window, pictures, trackPictures, config.getAsDouble(DRAW_DISTANCE)),
        video(std::string(VIDEOPATH), config.getAsDouble(VIDEOFPS), WIDTH, HEIGHT),
        matchWindow(window), sound(modelMonitor, window, config) {
    createFullScreenButton();
    createRecButton();
    lastFrame.reserve(3*WIDTH*HEIGHT),
    drawWait = SECTOMICROSEC / config.getAsDouble(FPS),
    recWait = SECTOMICROSEC / config.getAsDouble(VIDEOFPS); // us
}

Drawer::~Drawer() {}

void Drawer::run() {
    if (config.isSet(FULLSCN)) window.changeFullScreen();
    running = true;
    sound.start();
    std::thread recorder = std::thread(&Drawer::recorderTh, this);
    while (running) {
        auto start = std::chrono::system_clock::now();
        try {
            draw();
        } catch (std::exception &e) {
            printf("Drawer::run() exception catched: %s\n", e.what());
            running = false;
        }
        auto end = std::chrono::system_clock::now();
        int microsecsPassed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        if (drawWait > microsecsPassed)
            usleep(drawWait - microsecsPassed);
    }
    recorder.join();
    sound.stop();
    sound.join();
}

void Drawer::stop() {
    running = false;
}

void Drawer::resize(int width, int height) {
    window.resize(width, height);
}

void Drawer::createFullScreenButton() {
    int size = (window.getWidth() + window.getHeight()) / 37;
    SDL_Rect area = {10, 10, size, size};
    fullScreenButton = Button(window.getRenderer(), area, FULLSCREENBUTTON);
}

void Drawer::createRecButton() {
    SDL_Rect area = {WIDTH - 80, 10, 100, 30};
    recButton = Button(window.getRenderer(), area, RECBUTTON);
}

void Drawer::updateFullScreenButton(const SDL_Event *event) {
    fullScreenButton.updateEvent(event);
    if (fullScreenButton.isClicked()) {
        window.changeFullScreen();
    }
}

void Drawer::updateRecButton(const SDL_Event *event) {
    recButton.updateEvent(event);
    if (recButton.isClicked()) {
        video.changeState();
    }
}

void Drawer::showFullScreenButton() {
    int size = (window.getWidth() + window.getHeight()) / 37;
    fullScreenButton.updateSize(size, size);
    fullScreenButton.draw(window.getRenderer());
}

void Drawer::showRecButton() {
    recButton.updatePos(window.getWidth() - 80, 10);
    recButton.draw(window.getRenderer());
}

void Drawer::draw() {
    window.fill();
    if (modelMonitor.getGameState() == mainMenu) {
        matchWindow.render();
        matchWindow.setTrackNames(modelMonitor.getTrackNames());
        matchWindow.setMatchNames(modelMonitor.getMatchNames());
    } else if (modelMonitor.getGameState() == creating || modelMonitor.getGameState() == joining) {
        matchWindow.render();
    } else if (modelMonitor.getGameState() == startCountdown) {
        drawWorld();
        drawHUD();
        camera.showCountdown();
    } else if (modelMonitor.getGameState() == waitingEnd || modelMonitor.getGameState() == gameEnded) {
        drawWorld();
        drawHUD();
        camera.drawPodium(modelMonitor.getMatchResults());
        matchWindow.reset();
        camera.reset();
    } else {
        drawWorld();
        drawHUD();
    }
    showFullScreenButton();
    showRecButton();
    window.render();
    saveLastFrame();
}

void Drawer::drawWorld() {
    camera.updateBlockSize();
    int x = modelMonitor.getCars()[modelMonitor.getMyColor()]->getX();
    int y = modelMonitor.getCars()[modelMonitor.getMyColor()]->getY();
    camera.showTrack(x, modelMonitor.getTrack(), y);
    camera.showModifiers(x, y, modelMonitor.getModifiers());
    camera.showCars(x, y, modelMonitor.getCars(), modelMonitor.getMyColor());
}

void Drawer::drawHUD() {
    int laps = modelMonitor.getCars()[modelMonitor.getMyColor()]->getMyLap();
    int totalLaps = modelMonitor.getTotalLaps();
    camera.showLaps(laps, totalLaps);
}

void Drawer::saveLastFrame() {
    if (video.isRecording()) {
        std::lock_guard<std::mutex> lock(recordMutex);
        int res = SDL_RenderReadPixels(window.getRenderer(),
                                       nullptr,
                                       SDL_PIXELFORMAT_RGB24,
                                       lastFrame.data(),
                                       3 * WIDTH);
        if (res) {
            throw Error("Error Writing %s", SDL_GetError());
        }
    }
}

void Drawer::recorderTh() {
    while (running) {
        auto frameStart = std::chrono::system_clock::now();
        if (video.isRecording()) {
            std::lock_guard<std::mutex> lock(recordMutex);
            lastRecordState = true;
            video.setLastFrame(&lastFrame);
            video.writeFrame();
        } else if (lastRecordState && !video.isRecording()) {
            std::lock_guard<std::mutex> lock(recordMutex);
            lastRecordState = false;
        } else {
            sleep (1);
        }
        auto end = std::chrono::system_clock::now();
        int microsecsPassed = std::chrono::duration_cast<std::chrono::microseconds>(end - frameStart).count();
        if (recWait > microsecsPassed)
            usleep(recWait - microsecsPassed);
    }
    video.close();
}

void Drawer::showAnimation(SdlWindow &window) {
    //Muestro una animacion de prueba
    int framesInX = 5;
    int framesInY = 2;
    //Divido el ancho de la imagen por la cantidad de frames a lo ancho
    int widthFrame = 960 / framesInX;
    //Divido el largo de la imagen por la cantidad de frames a lo largo
    int heightFrame = 384 / framesInY;

    SdlTexture texture("images/explosion.png", window);
    SdlAnimation anim(texture, framesInX, framesInY, widthFrame, heightFrame,
                      0);
    SDL_Rect sdlDest = {(window.getWidth() - widthFrame) / 2, (window.getHeight() - heightFrame) / 2, widthFrame, heightFrame};
    anim.renderLooped(sdlDest, window);
}

MatchWindow& Drawer::getMatchWindow() {
    return this->matchWindow;
}
