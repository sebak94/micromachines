#include "../include/PictureLoader.h"

PictureLoader::PictureLoader(std::map<std::string, SdlSurface*> &pictures) : pictures(pictures) {
    pictures["CAR"] = new SdlSurface("images/pitstop_car_1.png");
    pictures["GRASS"] = new SdlSurface("images/Grass_Tile.png");
    pictures["LINEROAD"] = new SdlSurface("images/Road_01_Tile_03.png");
    pictures["CURVEROAD"] = new SdlSurface("images/Road_01_Tile_02.png");
    //agregar todas las imagenes
}

PictureLoader::~PictureLoader() {
    for (const auto pair : pictures) {
        delete pair.second;
    }
}
