#include "../include/Camera.h"

Camera::Camera(SdlWindow &window, std::map<std::string, SdlSurface*> &pictures,
        std::map<trackPartType, SdlSurface*> &trackPictures) :
        window(window), pictures(pictures), trackPictures(trackPictures) {
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
            //trackPictures[empty]->render(sdlDestGrass);
            x += width;
        }
        x = 0;
        y += height;
    }
}

void Camera::showTrack(int xMyCar, int yMyCar, std::vector<TrackPartData> &track) {
    //Transformo las coordenadas para que mi auto quede en el medio de la pantalla
    //y se muestre la parte de la pista correspondiente
    double xBegin = - xMyCar * (blockWidth / 100) + (window.getWidth() / 2);
    double yBegin = - yMyCar * (blockHeight / 100) - (window.getHeight() / 2) + blockHeight;

    for (int i = 0; i < track.size(); i++) {
        double x = track[i].getPosX() * (blockWidth / 100);
        double y = track[i].getPosY() * (blockHeight / 100);
        SDL_Rect sdlDestRoad = {(int) (x + xBegin), (int) (-y - yBegin), (int) blockWidth, (int) blockHeight};
        //if (track[i].getType() == empty)
            trackPictures[empty]->render(sdlDestRoad);
        trackPictures[track[i].getType()]->render(sdlDestRoad);
    }
}

void Camera::showCars(int xMyCar, int yMyCar, std::map<std::string, Car*> &cars) {
    double widthCar = window.getWidth() / 15;
    double heightCar = widthCar * 2;
    double xBegin = - xMyCar * (blockWidth / 100) + (window.getWidth() / 2);
    double yBegin = - yMyCar * (blockHeight / 100) - (window.getHeight() / 2);

    for (std::map<std::string, Car*>::iterator it = cars.begin(); it != cars.end(); ++it) {
        Car* car = it->second;
        double x = car->getX() * (blockWidth / 100) - (widthCar / 2);
        double y = car->getY() * (blockHeight / 100) + (heightCar / 2);
        SDL_Rect sdlDestCar = {(int)(x + xBegin), (int)(- y - yBegin), (int)widthCar, (int)heightCar};
        pictures[car->getMyColor()]->renderRotate(sdlDestCar, car->getDegrees(), SDL_FLIP_NONE);
    }
}
