#include <SDL2/SDL_events.h>
#include <iostream>
#include "../include/Drawer.h"
#include "../include/sdl/SdlAnimation.h"
#include <unistd.h>

#define WIDTH 900
#define HEIGHT 600
#define MICROSECS_WAIT 500000 //es medio segundo
//ahora se esta dibujando una vez cada medio segundo
//16000 seria que en un segundo se dibujen aprox 60 veces

Drawer::Drawer(ModelMonitor &modelMonitor) :
    window(WIDTH, HEIGHT),
    loader(window, pictures, trackPictures),
    camera(window, pictures, trackPictures),
    modelMonitor(modelMonitor) {}

Drawer::~Drawer() {}

void Drawer::run() {
    running = true;
    while (running) {
        auto start = std::chrono::system_clock::now();
        try {
            draw();
            printf("dibujo\n");
        } catch (std::exception &e) {
            running = false;
        }
        auto end = std::chrono::system_clock::now();
        int microsecsPassed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        usleep(MICROSECS_WAIT - microsecsPassed);
    }
}

void Drawer::stop() {
    running = false;
}

void Drawer::draw() {
    window.fill();
    camera.showBackground();
    int x = modelMonitor.getCars()[modelMonitor.getMyColor()]->getX();
    int y = modelMonitor.getCars()[modelMonitor.getMyColor()]->getY();
    camera.showTrack(x, y, modelMonitor.getTrack());
    camera.showCars(x, y, modelMonitor.getCars());
    window.render();
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
