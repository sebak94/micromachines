#include "../../../include/model/cars/car.h"
#include "../../../include/model/point.h"
#include "../../../include/model/cars/color.h"
#include "../../../include/model/cars/states/car_state.h"
#include "../../../include/model/cars/states/accelerate.h"
#include <utility>
#include <string>

Car::Car(uint8_t width, uint8_t height, uint8_t max_velocity,
    uint8_t acceleration, uint8_t grip, uint8_t maneuverability,
    Point initial_position, ColorType color): width(width), height(height),
    max_velocity(max_velocity), acceleration(acceleration), grip(grip),
    maneuverability(maneuverability), current_velocity(0), health(100),
    rotation(0), position(initial_position), color(color),
    state(new Accelerate()) {}

void Car::setState(CarState *state_received) {
    Accelerate ac;
    CarState* new_state = state_received->newState(&ac); // deberiamos pasarle (this)state
    delete state;
    state = new_state;
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
