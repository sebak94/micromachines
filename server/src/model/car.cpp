#include "../../include/model/car.h"
#include "../../include/model/point.h"
#include <cstdint>

Car::Car(Point initial_position, uint8_t width, uint8_t height):
    width(width), height(height), max_velocity(100), acceleration(25),
    grip(2), maneuverability(1), current_velocity(0), health(100),
    position(initial_position) {}

void Car::update() {}

void Car::serialize() {}
