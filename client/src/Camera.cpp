#include "../include/Camera.h"

Camera::Camera(SdlWindow &window, Model &model, std::map<PicType, SdlSurface*> &pictures, std::map<trackPartType, SdlSurface*> &trackPictures) : window(window), model(model), pictures(pictures), trackPictures(trackPictures) {
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
            trackPictures[empty]->render(sdlDestGrass, window);
            x += width;
        }
        x = 0;
        y += height;
    }
}

void Camera::showTrack(int xMyCar, int yMyCar) {
    //Transformo las coordenadas para que mi auto quede en el medio de la pantalla
    //y se muestre la parte de la pista correspondiente
    double xBegin = - xMyCar * (blockWidth / 100) + (window.getWidth() / 2);
    double yBegin = - yMyCar * (blockHeight / 100) - (window.getHeight() / 2) + blockHeight;

    std::vector<TrackPartData> trackPartData = model.getTrackPartData();
    for (int i = 0; i < trackPartData.size(); i++) {
        double x = trackPartData[i].getPosX() * (blockWidth / 100);
        double y = trackPartData[i].getPosY() * (blockHeight / 100);
        SDL_Rect sdlDestRoad = {(int) (x + xBegin), (int) (-y - yBegin), (int) blockWidth, (int) blockHeight};
        trackPictures[trackPartData[i].getType()]->render(sdlDestRoad, window);
        //printf("x: %d, y: %d, tipo: %d\n", trackPartData[i].getPosX(), trackPartData[i].getPosY(), trackPartData[i].getType());
        //printf("x: %f, y: %f, tipo: %d\n\n", x, y, trackPartData[i].getType());
    }
}

void Camera::showCars(int xMyCar, int yMyCar) {
    double widthCar = window.getWidth() / 15;
    double heightCar = widthCar * 2;
    double xBegin = - xMyCar * (blockWidth / 100) + (window.getWidth() / 2);
    double yBegin = - yMyCar * (blockHeight / 100) - (window.getHeight() / 2);

    for (int i = 0; i < model.getCars().size(); i++) {
        double x = model.getCars()[i].getX() * (blockWidth / 100) - (widthCar / 2);
        double y = model.getCars()[i].getY() * (blockHeight / 100) + (heightCar / 2);
        SDL_Rect sdlDestCar = {(int)(x + xBegin), (int)(- y - yBegin), (int)widthCar, (int)heightCar};
        pictures[model.getCars()[i].getType()]->renderRotate(sdlDestCar, model.getCars()[i].getDegrees(), SDL_FLIP_NONE, window);
        //printf("auto x: %d, y: %d\n", model.getCars()[i].getX(), model.getCars()[i].getY());
    }
}
