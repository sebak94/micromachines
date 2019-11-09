#include "../include/PictureLoader.h"

#define CAR_RED "red"
#define CAR_BLACK "black"
#define CAR_BLUE "blue"
#define CAR_GREEN "green"
#define CAR_ORANGE "orange"
#define CAR_WHITE "white"
#define CAR_YELLOW "yellow"

PictureLoader::PictureLoader(SdlWindow &window, std::map<std::string, SdlSurface*> &pictures,
        std::map<trackPartType, SdlSurface*> &trackPictures) :
        window(window), pictures(pictures), trackPictures(trackPictures) {
    //Imagenes para la pista
    trackPictures[empty] = new SdlSurface("../common/images/grass.png", window);
    trackPictures[downRight] = new SdlSurface("../common/images/downRight.png", window);
    trackPictures[downLeft] = new SdlSurface("../common/images/downLeft.png", window);
    trackPictures[upRight] = new SdlSurface("../common/images/upRight.png", window);
    trackPictures[upLeft] = new SdlSurface("../common/images/upLeft.png", window);
    trackPictures[horizontal] = new SdlSurface("../common/images/horizontal.png", window);
    trackPictures[vertical] = new SdlSurface("../common/images/vertical.png", window);
    trackPictures[finishH] = new SdlSurface("../common/images/finishHorizontal.png", window);
    trackPictures[finishV] = new SdlSurface("../common/images/finishVertical.png", window);

    //Imagenes de los autos
    pictures[CAR_RED] = new SdlSurface("images/car_red.png", window);
    pictures[CAR_BLACK] = new SdlSurface("images/car_black.png", window);
    pictures[CAR_BLUE] = new SdlSurface("images/car_blue.png", window);
    pictures[CAR_GREEN] = new SdlSurface("images/car_green.png", window);
    pictures[CAR_ORANGE] = new SdlSurface("images/car_orange.png", window);
    pictures[CAR_WHITE] = new SdlSurface("images/car_white.png", window);
    pictures[CAR_YELLOW] = new SdlSurface("images/car_yellow.png", window);

    //Agregar todas las imagenes faltantes
}

PictureLoader::~PictureLoader() {
    for (const auto pair : pictures) {
        delete pair.second;
    }
}
