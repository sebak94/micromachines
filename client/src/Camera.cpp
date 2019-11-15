#include <SDL2/SDL_ttf.h>
#include "../include/Camera.h"
#include "../../common/include/TextTexture.h"
#include "../include/sdl/SdlTexture.h"

Camera::Camera(SdlWindow &window, std::map<std::string, SdlSurface*> &pictures,
        std::map<trackPartType, SdlSurface*> &trackPictures) :
        window(window), pictures(pictures), trackPictures(trackPictures),
        lapBox(SdlSurface(LAPBOXPATH, window)) {
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
// 39.5% -> 31%
void Camera::showTrack(int xMyCar, int yMyCar, std::vector<TrackPartData> &track) {
    //Transformo las coordenadas para que mi auto quede en el medio de la pantalla
    //y se muestre la parte de la pista correspondiente
    double xBegin = - xMyCar * (blockWidth / 100) + (window.getWidth() / 2);
    double yBegin = - yMyCar * (blockHeight / 100) - (window.getHeight() / 2) + blockHeight;

    for (int i = 0; i < track.size(); i++) {
        double x = track[i].getPosX() * (blockWidth / 100);
        double y = track[i].getPosY() * (blockHeight / 100);
        SDL_Rect sdlDestRoad = {(int) (x + xBegin), (int) (-y - yBegin), (int) blockWidth, (int) blockHeight};
        if (sdlDestRoad.x + sdlDestRoad.w > 0 &&
            sdlDestRoad.x  < window.getWidth() &&
            sdlDestRoad.y + sdlDestRoad.h > 0 &&
            sdlDestRoad.y < window.getHeight()) {
                trackPictures[empty]->render(sdlDestRoad);
                trackPictures[track[i].getType()]->render(sdlDestRoad);
        }
    }
}

void Camera::showCars(int xMyCar, int yMyCar, std::map<std::string, Car*> &cars) {
    double widthCar = blockWidth / 6;
    double heightCar = blockHeight / 3;
    double xBegin = - xMyCar * (blockWidth / 100) + (window.getWidth() / 2.0);
    double yBegin = - yMyCar * (blockHeight / 100) - (window.getHeight() / 2.0);

    for (auto & it : cars) {
        Car* car = it.second;
        double x = car->getX() * (blockWidth / 100) - (widthCar / 2);
        double y = car->getY() * (blockHeight / 100) + (heightCar / 2);
        SDL_Rect sdlDestCar = {(int)(x + xBegin), (int)(- y - yBegin), (int)widthCar, (int)heightCar};
        pictures[car->getMyColor()]->renderRotate(sdlDestCar, car->getDegrees(), SDL_FLIP_NONE);
    }
}

void Camera::updateBlockSize() {
    //Las imagenes son cuadradas, asi que le pongo el mismo ancho que largo
    blockWidth = (window.getWidth() + window.getHeight()) / 4.0;
    blockHeight = (window.getWidth() + window.getHeight()) / 4.0;
}

void Camera::showLaps(int lap, int totalLaps) {
    SDL_Color textColor = {255, 255, 255, 0};
    SDL_Renderer * r = window.getRenderer();
    int w = window.getWidth(); int h = window.getHeight();
    SDL_Rect rect = {w - 130, h - 110, 200, 110};
    lapBox.render(rect);
    lapNumber.textToTexture(r, std::to_string(lap), textColor, LAPFONT, 50);
    lapNumber.render(r, w - 100, h - 75);
    std::string totStr = "/" + std::to_string(totalLaps);
    lapNumber.textToTexture(r, totStr, textColor, LAPFONT, 25);
    lapNumber.render(r, w - 45, h - 50);
}
