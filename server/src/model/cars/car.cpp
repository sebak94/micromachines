#include "../../../include/model/point.h"
#include "../../../include/model/cars/car.h"
#include "../../../include/model/cars/color.h"
#include "../../../include/model/cars/states/car_state.h"
#include "../../../include/model/cars/states/accelerate.h"
#include "../../../include/model/cars/states/left.h"
#include "../../../include/model/cars/states/right.h"
#include "../../../include/model/cars/states/break.h"
#include "../../../include/model/cars/states/nothing.h"
#include <string>
#include <cstdint>

Car::Car(uint8_t width, uint8_t height, uint8_t max_velocity,
    uint8_t acceleration, uint8_t grip, uint8_t maneuverability,
    Point initial_position, ColorType color): width(width), height(height),
    max_velocity(max_velocity), acceleration(acceleration), grip(grip),
    maneuverability(maneuverability), current_velocity(0), health(100),
    rotation(0), position(initial_position), color(color),
    state(new Nothing()) {}

void Car::updateState(char action) {
    switch (action) {
        case 'A': state = new Accelerate(); break;
        case 'L': state = new Left(); break;
        case 'R': state = new Right(); break;
        case 'B': state = new Break(); break;
        default: state = new Nothing(); break;
    }
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

void Car::update() {
    state->apply(this);
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

Car::~Car() {
    delete state;
}
