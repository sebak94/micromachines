#include "../../../include/model/point.h"
#include "../../../include/model/cars/car.h"
#include "../../../include/model/cars/color.h"
#include <string>
#include <cstdint>
#include <math.h>

Car::Car(uint8_t width, uint8_t height, uint8_t max_velocity,
         uint8_t acceleration, uint8_t grip, uint8_t maneuverability,
         Point initial_position, ColorType color) :
        width(width), height(height), max_velocity(max_velocity),
        acceleration(acceleration), grip(grip),
        maneuverability(maneuverability), current_velocity(0), health(100),
        rotation(0), position(initial_position), color(color) {

    this->states["accelerate"] = false;
    this->states["left"] = false;
    this->states["right"] = false;
    this->states["break"] = false;
}

void Car::updateState(char action) {
    switch (action) {
        case 'A':
            this->states["accelerate"] = true;
            break;
        case 'L':
            this->states["left"] = true;
            break;
        case 'R':
            this->states["right"] = true;
            break;
        case 'B':
            this->states["break"] = true;
            break;
    }
}

void Car::update() {
    if (this->states["left"]) this->leftCar();
    if (this->states["right"]) this->rightCar();
    if (this->states["accelerate"]) this->accelerateCar();
    if (this->states["break"]) this->breakCar();
}

void Car::updateCarPosition() {
    int x = (this->current_velocity) * ((int) pow(sin(this->rotation), 2));
    int y = (this->current_velocity) * ((int) pow(cos(this->rotation), 2));
    this->addPositionX(x);
    this->addPositionY(y);
}

void Car::accelerateCar() {
    if (this->current_velocity < this->max_velocity)
        this->current_velocity += this->acceleration;

    this->updateCarPosition();
}

void Car::breakCar() {
    if ((this->current_velocity * -1) < this->max_velocity)
        this->current_velocity -= this->acceleration;

    this->updateCarPosition();
}

void Car::leftCar() {
    this->addRotation(-1);
}

void Car::rightCar() {
    this->addRotation(1);
}

void Car::addPositionX(uint16_t x) {
    position = Point(position.getX() + x, position.getY());
}

void Car::addPositionY(uint16_t y) {
    position = Point(position.getX(), position.getY() + y);
}

void Car::addRotation(int16_t rotation) {
    this->rotation += rotation;
}

std::string Car::serialize() {
    // La serializacion es:
    // current_velocity, health, rotation, x, y, color\n
    return std::to_string(current_velocity) + ","
           + std::to_string(health) + ","
           + std::to_string(rotation) + ","
           + std::to_string(position.getX()) + ","
           + std::to_string(position.getY()) + ","
           + color.name() + "\n";
}

Car::~Car() {}
