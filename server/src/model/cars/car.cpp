#include "../../../include/model/cars/car.h"
#include "../../../include/model/point.h"
#include "../../../include/model/cars/colors.h"

Car::Car(uint8_t width, uint8_t height, uint8_t max_velocity,
    uint8_t acceleration, uint8_t grip, uint8_t maneuverability,
    Point initial_position, Color color): width(width), height(height),
    max_velocity(max_velocity), acceleration(acceleration), grip(grip),
    maneuverability(maneuverability), position(initial_position),
    color(color) {}
