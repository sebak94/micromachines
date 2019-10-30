#include "../include/Camera.h"

Camera::Camera(SdlWindow &window, Model &model, std::map<PicType, SdlSurface*> &pictures) : window(window), model(model), pictures(pictures) {
    //Las imagenes son cuadradas, asi que le pongo el mismo ancho que largo
    blockWidth = (window.getWidth() + window.getHeight()) / 4;
    blockHeight = (window.getWidth() + window.getHeight()) / 4;
}

Camera::~Camera() {

}

void Camera::showBackground() {
    //Pinto el backgroud de pasto
    double x = 0, y = 0;
    double width = window.getWidth() / 3;
    double height = window.getHeight() / 2;
    while (y < window.getHeight()) {
        while (x < window.getWidth()) {
            SDL_Rect sdlDestGrass = {(int)x, (int)y, (int)width, (int)height};
            pictures[PicType::GRASS_BACK]->render(sdlDestGrass, window);
            x += width;
        }
        x = 0;
        y += height;
    }
}

void Camera::showTrack(int xMyCar, int yMyCar, int xPos, int yPos) {
    //Transformo las coordenadas para que mi auto quede en el medio de la pantalla
    //y se muestre la parte de la pista correspondiente
    double xBegin = - xMyCar * (blockWidth / 100) + (window.getWidth() / 2);
    double yBegin = - yMyCar * (blockHeight / 100) - (window.getHeight() / 2) + blockHeight;

    for (int i = 0; i < model.getBlocks().size(); i++) {
        Block block = model.getBlocks()[i];
        double x = model.getBlocks()[i].getX() * (blockWidth / 100);
        double y = model.getBlocks()[i].getY() * (blockHeight / 100);
        SDL_Rect sdlDestRoad = {(int) (x + xBegin + xPos), (int) (-y - yBegin + yPos), (int) blockWidth, (int) blockHeight};
        pictures[block.getType()]->render(sdlDestRoad, window);
    }
}

void Camera::showCars(int xMyCar, int yMyCar, int xPos, int yPos) {
    double widthCar = window.getWidth() / 15;
    double heightCar = widthCar * 2;
    double xBegin = - xMyCar * (blockWidth / 100) + (window.getWidth() / 2);
    double yBegin = - yMyCar * (blockHeight / 100) - (window.getHeight() / 2);

    for (int i = 0; i < model.getCars().size(); i++) {
        double x = model.getCars()[i].getX() * (blockWidth / 100) - (widthCar / 2);
        double y = model.getCars()[i].getY() * (blockHeight / 100) + (heightCar / 2);
        SDL_Rect sdlDestCar = {(int)(x + xBegin + xPos), (int)(- y - yBegin + yPos), (int)widthCar, (int)heightCar};
        pictures[model.getCars()[i].getType()]->renderRotate(sdlDestCar, model.getCars()[i].getDegrees(), SDL_FLIP_NONE, window);
    }
}
