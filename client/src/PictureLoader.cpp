#include "../include/PictureLoader.h"
#include "../include/PicType.h"

PictureLoader::PictureLoader(std::map<PicType, SdlSurface*> &pictures) : pictures(pictures) {
    //Imagenes para la pista
    pictures[PicType::GRASS_BACK] = new SdlSurface("images/grass.png");
    pictures[PicType::ROAD_BACK] = new SdlSurface("images/back.png");

    pictures[PicType::LINE_VERTICAL_UP] = new SdlSurface("images/line_vertical_up.png");
    pictures[PicType::LINE_VERTICAL_DOWN] = new SdlSurface("images/line_vertical_down.png");
    pictures[PicType::LINE_HORIZONTAL_LEFT] = new SdlSurface("images/line_horizontal_left.png");
    pictures[PicType::LINE_HORIZONTAL_RIGHT] = new SdlSurface("images/line_horizontal_right.png");

    pictures[PicType::CURVE_UP_TO_RIGHT] = new SdlSurface("images/up_to_right.png");
    pictures[PicType::CURVE_DOWN_TO_LEFT] = new SdlSurface("images/down_to_left.png");
    pictures[PicType::CURVE_LEFT_TO_UP] = new SdlSurface("images/left_to_up.png");
    pictures[PicType::CURVE_RIGHT_TO_DOWN] = new SdlSurface("images/right_to_down.png");

    pictures[PicType::CURVE_DOWN_TO_RIGHT] = new SdlSurface("images/down_to_right.png");
    pictures[PicType::CURVE_UP_TO_LEFT] = new SdlSurface("images/up_to_left.png");
    pictures[PicType::CURVE_RIGHT_TO_UP] = new SdlSurface("images/right_to_up.png");
    pictures[PicType::CURVE_LEFT_TO_DOWN] = new SdlSurface("images/left_to_down.png");

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
