#ifndef __POINT_H__
#define __POINT_H__

#include <cstdint>

class Point {
    private:
    uint16_t x;
    uint16_t y;

    public:
    Point(uint16_t x, uint16_t y);
    uint16_t getX();
    uint16_t getY();
    Point operator+(Point&& other);
};

#endif
