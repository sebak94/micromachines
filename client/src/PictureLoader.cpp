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
    trackPictures[empty] = new SdlSurface("images/grass.png", window);
    trackPictures[downRight] = new SdlSurface("images/downRight.png", window);
    trackPictures[downLeft] = new SdlSurface("images/downLeft.png", window);
    trackPictures[upRight] = new SdlSurface("images/upRight.png", window);
    trackPictures[upLeft] = new SdlSurface("images/upLeft.png", window);
    trackPictures[horizontal] = new SdlSurface("images/horizontal.png", window);
    trackPictures[vertical] = new SdlSurface("images/vertical.png", window);

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
    for (const auto pair : trackPictures) {
        delete pair.second;
    }
    for (const auto pair : pictures) {
        delete pair.second;
    }
}
