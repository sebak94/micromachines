#include "../include/Camera.h"

Camera::Camera(Model &model, int camWidth, int camHeigth) : model(model) {
    this->camWidth = camWidth;
    this->camHeight = camHeigth;
}

Camera::~Camera() {

}
