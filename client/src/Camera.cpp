#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "../include/Camera.h"
#include "../../common/include/TextTexture.h"
#include "../include/sdl/SdlTexture.h"
#include "../include/sdl/SdlSoundFX.h"

#define HEART "heart"
#define FONTNAME "../common/fonts/OpenSans-Bold.ttf"

Camera::Camera(SdlWindow &window,
               std::map<std::string, SdlSurface *> &pictures,
               std::map<trackPartType, SdlSurface *> &trackPictures,
               double drawDistance) :
        window(window), pictures(pictures), trackPictures(trackPictures),
        lapBox(SdlSurface(LAPBOXPATH, window)), drawDistance(drawDistance) {
    TTF_Init();
    validateDrawDistance();
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

void Camera::showTrack(int xMyCar, int yMyCar, std::vector<TrackPartData> track) {
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

void Camera::showModifiers(int xMyCar, int yMyCar, std::vector<Modifier> modifiers) {
    //asumo que del lado del server cada modificador mide 25 x 25 (como los bloques miden 100 x 100)
    double width = blockWidth / 4.0;
    double height = blockHeight / 4.0;
    double xBegin = - xMyCar * (blockWidth / 100.0) + (window.getWidth() / 2.0);
    double yBegin = - yMyCar * (blockHeight / 100.0) - (window.getHeight() / 2.0);

    for (int i = 0; i < modifiers.size(); i++) {
        double x = modifiers[i].getX() * (blockWidth / 100);
        double y = modifiers[i].getY() * (blockHeight / 100) + height;
        int realX = x + xBegin;
        int realY = - y - yBegin;
        SDL_Rect area = {realX, realY, (int)width, (int)height};
        pictures[modifiers[i].getType()]->render(area);
    }
}

void Camera::showCars(int xMyCar, int yMyCar, std::map<std::string, Car *> cars,
                 const std::string& string) {
    double widthCar = blockWidth / 6.0;
    double heightCar = blockHeight / 3.0;
    double xBegin = - xMyCar * (blockWidth / 100.0) + (window.getWidth() / 2.0);
    double yBegin = - yMyCar * (blockHeight / 100.0) - (window.getHeight() / 2.0);
    SDL_Color color = {0, 0, 0, 0};
    TextTexture text;
    for (auto & it : cars) {
        Car* car = it.second;
        double x = car->getX() * (blockWidth / 100.0) - (widthCar / 2.0);
        double y = car->getY() * (blockHeight / 100.0) + (heightCar / 2.0);
        int realX = x + xBegin;
        int realY = - y - yBegin;
        if (it.first == string) {
            realX = window.getWidth()/2 - (widthCar / 2);
            realY = window.getHeight()/2 - (heightCar / 2);
        }
        SDL_Rect sdlDestCar = {realX, realY, (int)widthCar, (int)heightCar};
        pictures[car->getMyColor()]->renderRotate(sdlDestCar, car->getDegrees(), SDL_FLIP_NONE);

        SDL_Rect sdlDestHeart = {realX, (int)(realY+heightCar), (int)blockWidth / 20, (int)blockHeight / 20};
        pictures[HEART]->render(sdlDestHeart);
        text.textToTexture(window.getRenderer(), std::to_string(car->getHealth()) + "%", color, FONTNAME, blockWidth / 26);
        text.render(window.getRenderer(), (int)(realX+sdlDestHeart.w), (int)(realY+heightCar));
    }
}

void Camera::showCountdown() {
    SDL_Color color = {255, 255, 255, 0};
    TextTexture text;
    int fontsize = 100;
    int s = fontsize*96/128;  //offset from center of number
    if (countDownStarted) {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        countDown -= duration.count();
    }
    start = std::chrono::steady_clock::now();
    if (countDown <= 0) {
        countDown = SECOND;
        countDownNumber--;
    }
    text.textToTexture(window.getRenderer(), std::to_string((int)countDownNumber), color, FONTNAME, 100);
    text.render(window.getRenderer(), window.getWidth()/2, window.getHeight()/2 - s);
    countDownStarted = true;
}

void Camera::updateBlockSize() {
    //Las imagenes son cuadradas, asi que le pongo el mismo ancho que largo
    blockWidth = (window.getWidth() + window.getHeight()) / drawDistance;
    blockHeight = (window.getWidth() + window.getHeight()) / drawDistance;
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

void Camera::drawPodium(std::vector<std::string> matchResults) {
    int x = window.getWidth()/9;
    int y = window.getHeight()/12;
    int w = window.getWidth()/1.8;
    int h = window.getHeight()/6;
    for (int i = 0; i < matchResults.size(); i++) {
        SDL_Rect area = {x, y, w, h};
        SDL_Rect numberArea = {window.getWidth()/5, y+(h/3), w/15, h/2};
        SDL_Rect carArea = {(int)(window.getWidth()/2.5), y, w/10, h};
        pictures["podium"]->render(area);
        pictures[std::to_string(i+1)]->render(numberArea);
        pictures[matchResults[i]]->renderRotate(carArea, 90, SDL_FLIP_NONE);
        y += window.getHeight()/6;
    }
}

void Camera::validateDrawDistance() {
    if (drawDistance < 4 || drawDistance > 8)
        drawDistance = 4;
}

void Camera::reset() {
    countDown = SECOND;
    countDownNumber = SECSTOSTART;
    countDownStarted = false;
}
