#ifndef __CAR_H__
#define __CAR_H__

#include "model.h"
#include "point.h"
#include <cstdint>

class Car: public Model {
    private:
    const uint8_t width;
    const uint8_t height;
    const uint8_t max_velocity;
    const uint8_t acceleration;
    const uint8_t grip;
    const uint8_t maneuverability;
    uint8_t current_velocity;
    uint8_t health;
    Point position;

    public:
    Car(Point initial_position, uint8_t width, uint8_t height);
    virtual void update() override;
    virtual void serialize() override;
};

#endif
