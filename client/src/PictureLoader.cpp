#include "../include/PictureLoader.h"
#include "../include/PicType.h"

PictureLoader::PictureLoader(std::map<PicType, SdlSurface*> &pictures, std::map<trackPartType, SdlSurface*> &trackPictures) : pictures(pictures), trackPictures(trackPictures) {
    //Imagenes para la pista
    trackPictures[empty] = new SdlSurface("images/grass.png");
    trackPictures[downRight] = new SdlSurface("images/downRight.png");
    trackPictures[downLeft] = new SdlSurface("images/downLeft.png");
    trackPictures[upRight] = new SdlSurface("images/upRight.png");
    trackPictures[upLeft] = new SdlSurface("images/upLeft.png");
    trackPictures[horizontal] = new SdlSurface("images/horizontal.png");
    trackPictures[vertical] = new SdlSurface("images/vertical.png");

    //Imagenes de los autos
    pictures[PicType::CAR_RED] = new SdlSurface("images/car_red.png");
    pictures[PicType::CAR_BLACK] = new SdlSurface("images/car_black.png");
    pictures[PicType::CAR_BLUE] = new SdlSurface("images/car_blue.png");
    pictures[PicType::CAR_GREEN] = new SdlSurface("images/car_green.png");
    pictures[PicType::CAR_ORANGE] = new SdlSurface("images/car_orange.png");
    pictures[PicType::CAR_WHITE] = new SdlSurface("images/car_white.png");
    pictures[PicType::CAR_YELLOW] = new SdlSurface("images/car_yellow.png");

    //Agregar todas las imagenes faltantes
}

PictureLoader::~PictureLoader() {
    for (const auto pair : pictures) {
        delete pair.second;
    }
}
