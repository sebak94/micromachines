#include <SDL2/SDL_events.h>
#include <iostream>
#include "../include/Drawer.h"
#include "../include/sdl/SdlAnimation.h"
#include "../../record/include/Record.h"
#include "../../common/include/Error.h"
#include <unistd.h>

#define MICROSECS_WAIT 16000 //seria que en un segundo se dibujen aprox 60 veces
#define MUSICPATH "../common/sounds/beat.wav"
#define FULLSCREENBUTTON "../common/images/fullscreen.png"
#define RECBUTTON "../common/images/buttons/recButton.png"
#define VIDEOPATH "./recorded.mp4"
#define VIDEOFPS 30

Drawer::Drawer(ModelMonitor &modelMonitor) :
    window(WIDTH, HEIGHT),
    loader(window, pictures, trackPictures),
    camera(window, pictures, trackPictures),
    modelMonitor(modelMonitor), music(MUSICPATH),
    video(std::string(VIDEOPATH), VIDEOFPS, WIDTH, HEIGHT){
    createFullScreenButton();
    createRecButton();
    lastFrame.reserve(3*WIDTH*HEIGHT);
    videoTexture = video.getSDLRecordTexture(window.getRenderer());
}

Drawer::~Drawer() {}

void Drawer::run() {
    music.play();
    running = true;
    int lastMicrosecs = 0;  // aux to count recording + drawing time
    std::thread recorder = std::thread(&Drawer::recorderTh, this);
    while (running) {
        auto start = std::chrono::system_clock::now();
        try {
            draw();
        } catch (std::exception &e) {
            running = false;
        }
        auto end = std::chrono::system_clock::now();
        int microsecsPassed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        if (lastMicrosecs < microsecsPassed) {
            std::cout << "maxDrawingTime = " << microsecsPassed/1000 << "ms" << std::endl;
            lastMicrosecs = microsecsPassed;
        }
        if ( MICROSECS_WAIT > microsecsPassed )
            usleep(MICROSECS_WAIT - microsecsPassed);
    }
    recorder.join();
    music.stop();
}

void Drawer::stop() {
    running = false;
}

void Drawer::resize(int width, int height) {
    window.resize(width, height);
}

void Drawer::createFullScreenButton() {
    int size = (window.getWidth() + window.getHeight()) / 37;
    SDL_Rect area = {0, 0, size, size};
    fullScreenButton = Button(window.getRenderer(), area, FULLSCREENBUTTON);
}

void Drawer::createRecButton() {
    SDL_Rect area = {WIDTH - 80, 10, 100, 35};
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
    recButton.draw(window.getRenderer());
}

void Drawer::draw() {
    window.fill();
    camera.updateBlockSize();
    camera.showBackground();
    int x = modelMonitor.getCars()[modelMonitor.getMyColor()]->getX();
    int y = modelMonitor.getCars()[modelMonitor.getMyColor()]->getY();
    camera.showTrack(x, y, modelMonitor.getTrack());
    camera.showCars(x, y, modelMonitor.getCars());
    showFullScreenButton();
    showRecButton();
    window.render();
    saveLastFrame();
}

void Drawer::saveLastFrame() {
    if (video.isRecording()) {
        int res = SDL_RenderReadPixels(window.getRenderer(),
                                       nullptr,
                                       SDL_PIXELFORMAT_RGB24,
                                       lastFrame.data(),
                                       3 * WIDTH);
        std::lock_guard<std::mutex> lock(recordMutex);
        if (res) {
            std::cout << "Error reading pixels" << SDL_GetError() << std::endl;
            throw Error("Error Writing %s", SDL_GetError());
        }
    }
}

void Drawer::recorderTh() {
    int lastMicrosecs = 0;  // aux to count recording time
    while (running) {
        auto frameStart = std::chrono::system_clock::now();
        // si estoy grabando, me copio el lastFrame de Draw y proceso el frame
        if (video.isRecording()) {
            std::lock_guard<std::mutex> lock(recordMutex);
            lastRecordState = true;
            video.setLastFrame(lastFrame);
            video.writeFrame();
        // si no, finalizo y escribo el archivo
        } else if (lastRecordState && !video.isRecording()) {
            std::lock_guard<std::mutex> lock(recordMutex);
            lastRecordState = false;
            std::cout << "no grabo mas" << std::endl;
            video.close();
        }
        //control para FPS
        auto end = std::chrono::system_clock::now();
        int microsecsPassed = std::chrono::duration_cast<std::chrono::microseconds>(end - frameStart).count();
        if (lastMicrosecs < microsecsPassed) {
            std::cout << "maxRecordingTime = " << microsecsPassed/1000 << "ms" << std::endl;
            lastMicrosecs = microsecsPassed;
        }
        if ( 1000000 * 1 / VIDEOFPS > microsecsPassed )
            usleep(1000000 * 1 / VIDEOFPS - microsecsPassed);
    }
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
    SdlAnimation anim(texture, framesInX, framesInY, widthFrame, heightFrame);
    SDL_Rect sdlDest = {(window.getWidth() - widthFrame) / 2, (window.getHeight() - heightFrame) / 2, widthFrame, heightFrame};
    anim.render(sdlDest, window);
}
