#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "../include/Camera.h"
#include "../../common/include/TextTexture.h"
#include "../include/sdl/SdlTexture.h"
#include "../include/sdl/SdlSoundFX.h"

#define EXPLOS_SIZE 300  // pixels
#define EXPLOSION_LEN 150  // ms
#define EXPLOSION_SPRITE_W 960
#define EXPLOSION_SPRITE_H 384
#define EXPLOSION_SPRITES_X 5
#define EXPLOSION_SPRITES_Y 2
#define HEART "heart"
#define PODIUM "podium"
#define FONTNAME "../common/fonts/OpenSans-Bold.ttf"
#define EXPL_PATH "../common/images/explosion.png"
#define VIBRATE_MAG 0.1
#define VIBRATE_RATE 2
#define BLOCKSIZE 100.0
#define MODIFIER_SIZE_FACTOR 4
#define CAR_HEIGHT 3  // compression from blockheight
#define CAR_WIDTH 6  // compression from blockwidth

Camera::Camera(SdlWindow &window,
               std::map<std::string, SdlSurface *> &pictures,
               std::map<trackPartType, SdlSurface *> &trackPictures,
               double drawDistance) :
        window(window), pictures(pictures), trackPictures(trackPictures),
        lapBox(SdlSurface(LAPBOXPATH, window)),
        explosionTex(SdlTexture(EXPL_PATH, window)),
        drawDistance(drawDistance) {
    TTF_Init();
    validateDrawDistance();
}

Camera::~Camera() {}

/* Transformo las coordenadas para que mi auto quede en el medio de la
* pantalla y se muestre la parte de la pista correspondiente */
void Camera::showTrack(int xMyCar,
        int yMyCar,
        std::vector<TrackPartData> track) {
    double xBegin, yBegin;
    calcBeginPos(xBegin, yBegin, xMyCar, yMyCar);

    for (auto & i : track) {
        double x = i.getPosX() * (blockWidth / (BLOCKSIZE));
        double y = i.getPosY() * (blockHeight / (BLOCKSIZE));
        SDL_Rect sdlDestRoad = {(int) (x + xBegin), (int) (-y - yBegin),
                                (int) blockWidth, (int) blockHeight};
        if (isOnWindow(sdlDestRoad)) {
            trackPictures[empty]->render(sdlDestRoad);
            trackPictures[i.getType()]->render(sdlDestRoad);
        }
    }
}

/* Determina si el bloque a renderizar caerá dentro de la ventana */
bool Camera::isOnWindow(const SDL_Rect &sdlDestRoad) {
    return sdlDestRoad.x + sdlDestRoad.w > 0 &&
          sdlDestRoad.x  < window.getWidth() &&
          sdlDestRoad.y + sdlDestRoad.h > 0 &&
          sdlDestRoad.y < window.getHeight();
}

/* Asumo que del lado del server cada modificador mide 25 x 25
* (como los bloques miden 100 x 100) */
void Camera::showModifiers(int xMyCar,
        int yMyCar, std::vector<Modifier> modifiers) {
    double modifWidth = blockWidth / MODIFIER_SIZE_FACTOR;
    double modifHeight = blockHeight / MODIFIER_SIZE_FACTOR;
    double xBegin, yBegin;
    calcBeginPos(xBegin, yBegin, xMyCar, yMyCar);

    for (auto & modifier : modifiers) {
        double x = modifier.getX() * (blockWidth / (BLOCKSIZE));
        double y = modifier.getY() * (blockHeight / (BLOCKSIZE)) + modifHeight;
        int realX = x + xBegin;
        int realY = - y - yBegin;
        SDL_Rect area = {realX, realY, (int)modifWidth, (int)modifHeight};
        pictures[modifier.getType()]->render(area);
    }
}

void Camera::calcBeginPos(double & xBegin, double & yBegin, int & x, int & y) {
    xBegin = - x * (blockWidth / (BLOCKSIZE)) + (window.getWidth() / 2.0);
    yBegin = - y * (blockHeight / (BLOCKSIZE)) - (window.getHeight() / 2.0);
}

void Camera::setCarSize(double & widthCar, double & heightCar) {
    widthCar = blockWidth / CAR_WIDTH;
    heightCar = blockHeight / CAR_HEIGHT;
}

void Camera::showCars(int xMyCar, int yMyCar, std::map<std::string, Car *> cars,
                 const std::string& myCar) {
    double widthCar, heightCar, xBegin, yBegin;
    setCarSize(widthCar, heightCar);
    calcBeginPos(xBegin, yBegin, xMyCar, yMyCar);
    TextTexture text;
    for (auto & it : cars) {
        Car* car = it.second;
        double x = car->getX() * (blockWidth / (BLOCKSIZE)) - (widthCar / 2.0);
        double y = car->getY() * (blockHeight / (BLOCKSIZE)) + (heightCar / 2.0);
        int realX = x + xBegin;
        int realY = - y - yBegin;
        if (it.first == myCar) {
            realX = window.getWidth()/2 - (widthCar / 2);
            realY = window.getHeight()/2 - (heightCar / 2);
        }
        showLife(car, realX, realY, heightCar);
        vibrateCar(car, realX, realY);
        SDL_Rect sdlDestCar = {realX, realY, (int)widthCar, (int)heightCar};
        pictures[car->getMyColor()]->renderRotate(sdlDestCar,
                                                  car->getDegrees(),
                                                  SDL_FLIP_NONE);
        explodeCar(car, realX + widthCar/2, realY + heightCar/2, EXPLOS_SIZE);
    }
}


void Camera::vibrateCar(Car *car, int &x, int &y) {
    auto it = carVibration.find(car);
    if (it != carVibration.end()) {
        if (it->second >= 0) x += VIBRATE_MAG;
        else if (it->second < 0) y -= VIBRATE_MAG;
        it->second++;
        if (it->second == VIBRATE_RATE) it->second = -VIBRATE_RATE;
    } else {
        carVibration.emplace(car, 0);
    }
}

void Camera::explodeCar(Car *car, int x, int y, int s) {
    if (car->exploded()) triggerExplosion(car);
    auto exploding = explosions.find(car);
    if (exploding != explosions.end() && exploding->second.isTriggered()) {
        SDL_Rect sdlExp = {x - s / 2, y - s / 2, s, s};
        exploding->second.render(sdlExp, window);
    }
}

void Camera::triggerExplosion(Car * car) {
    auto it = explosions.find(car);
    if (it != explosions.end()) {
        it->second.trigger();
    } else {
        int framesInX = EXPLOSION_SPRITES_X;
        int framesInY = EXPLOSION_SPRITES_Y;
        // Divido el ancho de la imagen por la cantidad de frames a lo ancho
        int widthFrame = EXPLOSION_SPRITE_W / framesInX;
        // Divido el largo de la imagen por la cantidad de frames a lo largo
        int heightFrame = EXPLOSION_SPRITE_H / framesInY;
        explosions.emplace(car, SdlAnimation(explosionTex, framesInX,
                                             framesInY, widthFrame, heightFrame,
                                             EXPLOSION_LEN));
        explosions.at(car).trigger();
    }
}

void Camera::showLife(Car * car, int & realX, int & realY, double &heightCar) {
    TextTexture text;
    SDL_Color color = {0, 0, 0, 0};
    SDL_Rect sdlDestHeart = {realX,
                             (int)(realY+heightCar),
                             (int)blockWidth / 20,
                             (int)blockHeight / 20};
    pictures[HEART]->render(sdlDestHeart);
    text.textToTexture(window.getRenderer(),
            std::to_string(car->getHealth()) + "%",
            color,
            FONTNAME,
            blockWidth / 26);
    text.render(window.getRenderer(),
            (int)(realX+sdlDestHeart.w),
            (int)(realY+heightCar));
}

void Camera::showCountdown() {
    SDL_Color color = {255, 255, 255, 0};
    TextTexture text;
    int fontsize = 300;
    int s = fontsize*128/96;  // offset from center of number
    if (countDownStarted) {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast< ms >(end - start);
        countDown -= duration.count();
    }
    start = std::chrono::steady_clock::now();
    if (countDown <= 0) {
        countDown = SECOND;
        countDownNumber--;
    }
    text.textToTexture(window.getRenderer(),
            std::to_string((int)countDownNumber),
            color,
            FONTNAME,
            fontsize);
    text.render(window.getRenderer(),
                (window.getWidth()/2 - s/5),
                (window.getHeight() - s)/2);
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
    if (lap > totalLaps) lap = totalLaps;
    lapNumber.textToTexture(r, std::to_string(lap), textColor, LAPFONT, 50);
    lapNumber.render(r, w - (BLOCKSIZE), h - 75);
    std::string totStr = "/" + std::to_string(totalLaps);
    lapNumber.textToTexture(r, totStr, textColor, LAPFONT, 25);
    lapNumber.render(r, w - 45, h - 50);
}

void Camera::drawPodium(std::vector<std::string> matchResults) {
    int x = window.getWidth()/9;
    int y = window.getHeight()/12;
    int w = window.getWidth()/1.8;
    int h = window.getHeight()/6;
    for (size_t i = 0; i < matchResults.size(); i++) {
        SDL_Rect area = {x, y, w, h};
        SDL_Rect numberArea = {window.getWidth()/5, y+(h/3), w/15, h/2};
        SDL_Rect carArea = {(int)(window.getWidth()/2.5), y, w/10, h};
        pictures[PODIUM]->render(area);
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
