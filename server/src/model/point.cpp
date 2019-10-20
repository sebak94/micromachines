#include "../../include/model/point.h"
#include <cstdint>

Point::Point(uint16_t x, uint16_t y): x(x), y(y) {}

uint16_t Point::getX() {
    return x;
}

uint16_t Point::getY() {
    return y;
}

Point Point::operator+(Point&& other) {
    return Point(x + other.getX(), y + other.getY());
}
