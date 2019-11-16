#include "../include/Car.h"

Car::Car(std::string color) {
    this->color = color;
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

int Car::getMyLap() const {
    return this->laps;
}

std::string Car::getMyColor() const {
    return this->color;
}

void Car::update(int x, int y, int degrees, int health, int laps) {
    this->x = x;
    this->y = y;
    this->degrees = degrees;
    this->health = health;
    this->laps = laps;
}
