#include "../include/PictureLoader.h"

#define CAR_RED "red"
#define CAR_BLACK "black"
#define CAR_BLUE "blue"
#define CAR_GREEN "green"
#define CAR_ORANGE "orange"
#define CAR_WHITE "white"
#define CAR_YELLOW "yellow"

PictureLoader::PictureLoader(std::map<std::string, SdlSurface*> &pictures, std::map<trackPartType, SdlSurface*> &trackPictures) : pictures(pictures), trackPictures(trackPictures) {
    //Imagenes para la pista
    trackPictures[empty] = new SdlSurface("images/grass.png");
    trackPictures[downRight] = new SdlSurface("images/downRight.png");
    trackPictures[downLeft] = new SdlSurface("images/downLeft.png");
    trackPictures[upRight] = new SdlSurface("images/upRight.png");
    trackPictures[upLeft] = new SdlSurface("images/upLeft.png");
    trackPictures[horizontal] = new SdlSurface("images/horizontal.png");
    trackPictures[vertical] = new SdlSurface("images/vertical.png");

    //Imagenes de los autos
    pictures[CAR_RED] = new SdlSurface("images/car_red.png");
    pictures[CAR_BLACK] = new SdlSurface("images/car_black.png");
    pictures[CAR_BLUE] = new SdlSurface("images/car_blue.png");
    pictures[CAR_GREEN] = new SdlSurface("images/car_green.png");
    pictures[CAR_ORANGE] = new SdlSurface("images/car_orange.png");
    pictures[CAR_WHITE] = new SdlSurface("images/car_white.png");
    pictures[CAR_YELLOW] = new SdlSurface("images/car_yellow.png");

    //Agregar todas las imagenes faltantes
}

PictureLoader::~PictureLoader() {
    for (const auto pair : pictures) {
        delete pair.second;
    }
}
