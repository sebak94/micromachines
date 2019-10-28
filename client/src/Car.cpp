#include "../include/Car.h"

Car::Car(int x, int y, int degrees, int health, PicType type) {
    this->x = x;
    this->y = y;
    this->degrees = degrees;
    this->health = health;
    this->type = type;
}

Car::~Car() {

}

int Car::getX() const {
    return this->x;
}

int Car::getY() const {
    return this->y;
}

int Car::getDegrees() const {
    return this->degrees;
}

int Car::getHealth() const {
    return this->health;
}

PicType Car::getType() const {
    return this->type;
}
