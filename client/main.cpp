#include <iostream>
#include <string>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>
#include "include/server.h"
#include "../common/include/socket_error.h"
#include "include/sdl/SdlWindow.h"
#include "include/sdl/SdlAnimation.h"
#include "include/sdl/SdlSurface.h"

#define SUCCESS 0

void run(const char *hostname, const char *service) {
    try {
        bool running = true;
        std::string str;
        Server server(hostname, service);

        while (running) {
            std::cout << server.receive();
            running = false;
        }
    } catch(const SocketError &e) {}
}

void showAnimations(SdlWindow &window) {
    //Animacion explosion
    int framesInX = 5;
    int framesInY = 2;
    //Divido el ancho de la imagen por la cantidad de frames a lo ancho
    int widthFrame = 960 / framesInX;
    //Divido el largo de la imagen por la cantidad de frames a lo largo
    int heightFrame = 384 / framesInY;
    SdlAnimation anim4(framesInX, framesInY, "images/explosion.png", window, widthFrame, heightFrame);
    SDL_Rect sdlDest3 = {(window.getWidth() - widthFrame) / 2, (window.getHeight() - heightFrame) / 2, widthFrame, heightFrame};
    anim4.render(sdlDest3);
}

void showBackground(SdlWindow &window, int xPos, int yPos) {
    SdlSurface imageGrass("images/Grass_Tile.png", window);
    //Pinto todo el backgroud de pasto
    int x = 0, y = 0;
    int width = window.getWidth() / 3;
    int height = window.getHeight() / 2;
    while (y < window.getHeight()) {
        while (x < window.getWidth()) {
            SDL_Rect sdlDestGrass = {x, y, width, height};
            imageGrass.render(sdlDestGrass);
            x += width;
        }
        x = 0;
        y += height;
    }

    //Agrego arriba la ruta
    SdlSurface imageRoad("images/Road_01_Tile_03.png", window);
    SdlSurface imageRoadCurve("images/Road_01_Tile_08.png", window);
    int routeWidth = 500; //el ancho de la ruta es siempre el mismo y siempre en el medio de la pantalla
    int routeHeight = window.getHeight() / 2; //muestro de a 2 imagenes de ruta
    int halfWidth = (window.getWidth() - routeWidth) / 2;
    int beginHeight = - window.getHeight(); //la altura tiene el eje al reves de lo "comun"

    //Supongamos que la pista tiene 3 imagenes rectas
    for (int i = 0; i < 3; i++) {
        SDL_Rect sdlDestRoad1 = {halfWidth + xPos, -beginHeight - routeHeight + yPos, routeWidth, routeHeight};
        imageRoad.render(sdlDestRoad1);
        beginHeight += routeHeight;
    }
    SDL_Rect sdlDestRoad2 = {halfWidth + xPos, -beginHeight - routeHeight + yPos, routeWidth, routeHeight};
    imageRoadCurve.render(sdlDestRoad2);
}

int testSdl() {
    try {
        SdlWindow window(900, 600);
        showAnimations(window);

        window.fill();
        SdlSurface image("images/pitstop_car_1.png", window);
        bool running = true;
        int widthCar = 100, heightCar = 200;
        int x = 0, y = 0; //Son para mover la pista, el auto es fijo, se mueve la pista
        while (running) {
            window.fill();
            showBackground(window, x, y);
            //Ubico el auto siempre en el medio
            SDL_Rect sdlDestCar = {(window.getWidth() - widthCar) / 2, (window.getHeight() - heightCar) / 2, widthCar, heightCar};
            image.render(sdlDestCar);

            SDL_Event event;
            SDL_WaitEvent(&event);
            switch (event.type) {
                case SDL_KEYDOWN: {
                    SDL_KeyboardEvent& keyEvent = (SDL_KeyboardEvent&) event;
                    switch (keyEvent.keysym.sym) {
                        case SDLK_LEFT:
                            x += 20;
                            break;
                        case SDLK_RIGHT:
                            x -= 20;
                            break;
                        case SDLK_UP:
                            y += 20;
                            break;
                        case SDLK_DOWN:
                            y -= 20;
                            break;
                    }
                } // Fin KEY_DOWN
                    break;
                case SDL_QUIT:
                    running = false;
                    break;
            }
            window.render();
        }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    SDL_Quit();
    return 0;
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        std::cout << "./client <hostname> <service>";
        return SUCCESS;
    }
    run(argv[1], argv[2]);
    testSdl();
    return SUCCESS;
}
