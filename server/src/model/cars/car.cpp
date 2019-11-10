#include "../../../include/model/point.h"
#include "../../../include/model/cars/car.h"
#include "../../../include/model/cars/color.h"
#include <string>
#include <cstdint>
#include <math.h>

#define PI 3.14159265

Car::Car(uint8_t width, uint8_t height, uint8_t max_velocity, uint8_t acceleration, uint8_t grip,
         uint8_t maneuverability, Point initial_position, ColorType color, uint16_t rotation) :
        width(width), height(height), max_velocity(max_velocity),
        acceleration(acceleration), grip(grip),
        maneuverability(maneuverability), current_velocity(0), health(100),
        rotation(rotation), position(initial_position), color(color) {

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
        case 'Y':
            this->states["accelerate"] = false;
            break;
        case 'W':
            this->states["left"] = false;
            break;
        case 'X':
            this->states["right"] = false;
            break;
        case 'Z':
            this->states["break"] = false;
            break;
    }
}

void Car::update() {
    printf("VELOCIDAD: %i\n", this->current_velocity);
    printf("POS (X%i,Y%i): \n", position.getX(), position.getY());
    printf("ANGULO: %i\n", this->rotation);
    printf("\n");

    this->leftCar();
    this->rightCar();
    this->accelerateCar();
    this->breakCar();

    this->updateCarPosition();
}

void Car::updateCarPosition() {
    //el seno y coseno son en radianes, asi que convierto el angulo de rotacion a radianes
    int x = this->current_velocity * (sin(this->rotation * PI / 180)) / 50;
    int y = this->current_velocity * (cos(this->rotation * PI / 180)) / 50;
    this->addPositionX(x);
    this->addPositionY(y);
}

void Car::accelerateCar() {
    if (this->states["accelerate"]) {
        if ((this->current_velocity + this->acceleration) < this->max_velocity)
            this->current_velocity += this->acceleration;
        else
            this->current_velocity = this->max_velocity;
    } else if ((!this->states["break"]) && (this->current_velocity > 0)) {
        if ((this->current_velocity - this->acceleration) > 0)
            this->current_velocity -= this->acceleration;
        else
            this->current_velocity = 0;
    }
}

void Car::breakCar() {
    if (this->states["break"]) {
        if ((this->current_velocity - this->acceleration) >
            (this->max_velocity * -1))
            this->current_velocity -= this->acceleration;
        else
            this->current_velocity = (this->max_velocity * -1);
    } else if ((!this->states["accelerate"]) && (this->current_velocity < 0)) {
        if ((this->current_velocity + this->acceleration) < 0)
            this->current_velocity += this->acceleration;
        else
            this->current_velocity = 0;
    }
}

void Car::leftCar() {
    if (this->states["left"])
        this->addRotation(-2);
    else if (!this->states["right"])
        this->addRotation(1);
}

void Car::rightCar() {
    if (this->states["right"])
        this->addRotation(2);
    else if (!this->states["left"])
        this->addRotation(-1);
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
