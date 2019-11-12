#include <SDL2/SDL_ttf.h>
#include "../include/Camera.h"
#include "../../common/include/TextTexture.h"

#define HEART "heart"
#define FONTNAME "../common/fonts/OpenSans-Bold.ttf"

Camera::Camera(SdlWindow &window, std::map<std::string, SdlSurface*> &pictures,
        std::map<trackPartType, SdlSurface*> &trackPictures) :
        window(window), pictures(pictures), trackPictures(trackPictures) {
    TTF_Init();
}

Camera::~Camera() {}

void Camera::showBackground() {
    //Pinto el backgroud de pasto
    double x = 0, y = 0;
    double width = window.getWidth() / 3;
    double height = window.getHeight() / 2;
    while (y < window.getHeight()) {
        while (x < window.getWidth()) {
            SDL_Rect sdlDestGrass = {(int)x, (int)y, (int)width, (int)height};
            //trackPictures[empty]->render(sdlDestGrass);
            x += width;
        }
        x = 0;
        y += height;
    }
}

void Camera::showTrack(int xMyCar, int yMyCar, std::vector<TrackPartData> &track) {
    //Transformo las coordenadas para que mi auto quede en el medio de la pantalla
    //y se muestre la parte de la pista correspondiente
    double xBegin = - xMyCar * (blockWidth / 100) + (window.getWidth() / 2);
    double yBegin = - yMyCar * (blockHeight / 100) - (window.getHeight() / 2) + blockHeight;

    for (int i = 0; i < track.size(); i++) {
        double x = track[i].getPosX() * (blockWidth / 100);
        double y = track[i].getPosY() * (blockHeight / 100);
        SDL_Rect sdlDestRoad = {(int) (x + xBegin), (int) (-y - yBegin), (int) blockWidth, (int) blockHeight};
        trackPictures[empty]->render(sdlDestRoad);
        trackPictures[track[i].getType()]->render(sdlDestRoad);
    }
}

void Camera::showCars(int xMyCar, int yMyCar, std::map<std::string, Car*> &cars) {
    double widthCar = blockWidth / 6;
    double heightCar = blockHeight / 3;
    double xBegin = - xMyCar * (blockWidth / 100) + (window.getWidth() / 2);
    double yBegin = - yMyCar * (blockHeight / 100) - (window.getHeight() / 2);
    SDL_Color color = {0, 0, 0, 0};
    TextTexture text;

    for (std::map<std::string, Car*>::iterator it = cars.begin(); it != cars.end(); ++it) {
        Car* car = it->second;
        double x = car->getX() * (blockWidth / 100) - (widthCar / 2);
        double y = car->getY() * (blockHeight / 100) + (heightCar / 2);
        int realX = x + xBegin;
        int realY = - y - yBegin;
        SDL_Rect sdlDestCar = {realX, realY, (int)widthCar, (int)heightCar};
        pictures[car->getMyColor()]->renderRotate(sdlDestCar, car->getDegrees(), SDL_FLIP_NONE);

        SDL_Rect sdlDestHeart = {realX, (int)(realY+heightCar), (int)blockWidth / 20, (int)blockHeight / 20};
        pictures[HEART]->render(sdlDestHeart);
        text.textToTexture(window.getRenderer(), std::to_string(car->getHealth()) + "%", color, FONTNAME, blockWidth / 26);
        text.render(window.getRenderer(), (int)(realX+sdlDestHeart.w), (int)(realY+heightCar));
    }
}

void Camera::updateBlockSize() {
    //Las imagenes son cuadradas, asi que le pongo el mismo ancho que largo
    blockWidth = (window.getWidth() + window.getHeight()) / 4;
    blockHeight = (window.getWidth() + window.getHeight()) / 4;
}
