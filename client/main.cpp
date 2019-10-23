#include <iostream>
#include <string>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>
#include <map>
#include "include/server.h"
#include "../common/include/socket_error.h"
#include "include/sdl/SdlWindow.h"
#include "include/sdl/SdlAnimation.h"
#include "include/sdl/SdlSurface.h"
#include "include/PictureLoader.h"

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

void showAnimation(SdlWindow &window) {
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

void showBackground(SdlWindow &window, int xPos, int yPos, std::map<std::string, SdlSurface*> &pictures) {
    //Pinto el backgroud de pasto
    int x = 0, y = 0;
    int width = window.getWidth() / 3;
    int height = window.getHeight() / 2;
    while (y < window.getHeight()) {
        while (x < window.getWidth()) {
            SDL_Rect sdlDestGrass = {x, y, width, height};
            pictures["GRASS"]->render(sdlDestGrass, window);
            x += width;
        }
        x = 0;
        y += height;
    }

    //Agrego arriba la ruta
    int routeWidth = 400;
    int routeHeight = 400;
    int halfWidth = (window.getWidth() - routeWidth) / 2;
    int beginHeight = - window.getHeight(); //la altura tiene el eje al reves de lo "comun"

    x = halfWidth;
    y = beginHeight;
    SDL_Rect sdlDestRoad;
    //Supongamos que la pista tiene 2 imagenes rectas
    for (int i = 0; i < 2; i++) {
        y += routeHeight;
        sdlDestRoad = {x + xPos, -y + yPos, routeWidth, routeHeight};
        pictures["LINEROAD"]->render(sdlDestRoad, window);
    }
    //Agrego una curva
    y += routeHeight;
    y -= (routeHeight - 100) / 10; //esto para la curva (saca el espacio que sobra)
    sdlDestRoad = {x + xPos, -y + yPos, routeWidth, routeHeight};
    pictures["CURVEROAD"]->render(sdlDestRoad, window);
    //Agrego una imagen mas despues de la curva
    x += routeWidth;
    x -= (routeWidth - 100) / 10; //esto despues de la curva (saca el espacio que sobra)
    sdlDestRoad = {x + xPos, -y + yPos, routeWidth, routeHeight};
    pictures["LINEROAD"]->renderRotate(sdlDestRoad, 90, SDL_FLIP_NONE, window);
}

int testSdl() {
    std::map<std::string, SdlSurface*> pictures;
    PictureLoader loader(pictures);
    try {
        SdlWindow window(900, 600);
        showAnimation(window);
        window.fill();
        bool running = true;
        int widthCar = 80, heightCar = 170;
        int x = 0, y = 0; //Son para mover la pista, el auto es fijo, se mueve la pista
        while (running) {
            window.fill();
            showBackground(window, x, y, pictures);
            //Ubico el auto siempre en el medio
            SDL_Rect sdlDestCar = {(window.getWidth() - widthCar) / 2, (window.getHeight() - heightCar) / 2, widthCar, heightCar};
            pictures["CAR"]->render(sdlDestCar, window);

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
