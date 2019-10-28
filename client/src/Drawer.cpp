#include <SDL2/SDL_events.h>
#include <iostream>
#include "../include/Drawer.h"
#include "../include/sdl/SdlAnimation.h"
#include "../include/Block.h"
#include "../include/PicType.h"
#include "../include/Model.h"
#include "../include/Camera.h"

#define ROUTEWIDTH 400
#define ROUTEHEIGHT 400

Drawer::Drawer(Socket &socket) : socket(socket), window(900, 600), loader(pictures) {
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

void Drawer::showBackground(int xPos, int yPos, std::map<PicType, SdlSurface*> &pictures, std::vector<Block> &blocks, Camera &camera) {
    //Pinto el backgroud de pasto
    int x = 0, y = 0;
    int width = window.getWidth() / 3;
    int height = window.getHeight() / 2;
    while (y < window.getHeight()) {
        while (x < window.getWidth()) {
            SDL_Rect sdlDestGrass = {x, y, width, height};
            pictures[PicType::GRASS_BACK]->render(sdlDestGrass, window);
            x += width;
        }
        x = 0;
        y += height;
    }

    //Agrego la pista
    int xBegin = 0;
    int yBegin = ROUTEHEIGHT;
    for (int i = 0; i < blocks.size(); i++) {
        Block block = blocks[i];
        PicType type = block.getType();
        x = blocks[i].getX() * (ROUTEWIDTH / 100);
        y = blocks[i].getY() * (ROUTEHEIGHT / 100);
        SDL_Rect sdlDestRoad = {x + xBegin + xPos, -y - yBegin + yPos, ROUTEWIDTH, ROUTEHEIGHT};
        pictures[PicType::ROAD_BACK]->render(sdlDestRoad, window);
        pictures[type]->render(sdlDestRoad, window);
    }
}

void Drawer::showCars(int xPos, int yPos, std::vector<Car> &cars, Camera &camera) {
    int widthCar = 80, heightCar = 170;
    for (int i = 0; i < cars.size(); i++) {
        //cars[i].x viene del modelo, lo multiplico porque en realidad lo represento mas grande (acorde a la ventana)
        int x = cars[i].getX() * (ROUTEWIDTH / 100) - (widthCar / 2);
        int y = cars[i].getY() * (ROUTEHEIGHT / 100) + (heightCar / 2);
        SDL_Rect sdlDestCar = {x + xPos, - y + yPos, widthCar, heightCar};
        pictures[cars[i].getType()]->renderRotate(sdlDestCar, cars[i].getDegrees(), SDL_FLIP_NONE, window);
    }
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
        Camera camera(model, window.getWidth(), window.getHeight());
        window.fill();
        showBackground(x, y, pictures, model.getBlocks(), camera);
        showCars(x, y, model.getCars(), camera);
        window.render();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
