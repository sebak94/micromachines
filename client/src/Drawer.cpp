#include <SDL2/SDL_events.h>
#include <iostream>
#include "../include/Drawer.h"
#include "../include/sdl/SdlAnimation.h"
#include "../include/PicType.h"
#include "../include/Model.h"
#include "../include/Camera.h"

#define WIDTH 900
#define HEIGHT 600

Drawer::Drawer(Socket &socket) : socket(socket), window(WIDTH, HEIGHT), loader(pictures) {
    //para dibujar la primera vez, en realidad dibujo lo que venga del servidor
    testSdl(pictures, "asd");
}

Drawer::~Drawer() {}

void Drawer::run() {
    this->running = true;
    while (running) {
        try {
            std::string text = receive();
            printf("recibo: %s\n\n", text.c_str());
            testSdl(pictures, text); //dibujar
        } catch (std::exception &e) {
            printf("murio el socket en el drawer\n");
            running = false;
        }
    }
}

//Por ahora recibo 1 solo byte
std::string Drawer::receive() {
    std::vector<char> response;
    char c;
    this->socket.Receive(&c, 1);
    response.push_back(c);
    std::string str_resp(response.begin(), response.end());
    return str_resp;
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

int Drawer::testSdl(std::map<PicType, SdlSurface*> &pictures, std::string text) {
    try {
        if (text == "A") {
            y -= 20;
        } else if (text == "B") {
            y += 20;
        } else if (text == "L") {
            x -= 20;
        } else if (text == "R") {
            x += 20;
        }
        Model model(8); //Modelo hardcodeado
        Camera camera(window, model, pictures);
        window.fill();
        camera.showBackground();
        camera.showTrack(50, 150, x, y);
        camera.showCars(50, 150, x, y);
        window.render();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
