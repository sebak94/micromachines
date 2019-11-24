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

bool Car::collided() {
    return collision;
}

bool Car::exploded() {
    bool aux = explosion;
    explosion = false;
    return aux;
}

int Car::getExplosionX() {
    return explosionX;
}

int Car::getExplosionY() {
    return explosionY;
}

std::string Car::getMyColor() const {
    return this->color;
}

void Car::update(int x, int y, int degrees, int health, int laps) {
    this->degrees = degrees;
    this->health = health;
    this->laps = laps;
    collision = false;
    if (health != lastHealth) {
        if (health > lastHealth) {
            explosion = true;
            explosionX = this->x;
            explosionY = this->y;
        }
        lastHealth = health;
        collision = true;
    }
    this->x = x;
    this->y = y;
}
