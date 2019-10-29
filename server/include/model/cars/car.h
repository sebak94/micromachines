#ifndef __CAR_H__
#define __CAR_H__

#include "../model.h"
#include "../point.h"
#include "colors.h"
#include "states/car_state.h"
#include "../../player_action.h"
#include <cstdint>

class Car: public Model {
    protected:
    const uint8_t width;
    const uint8_t height;
    const uint8_t max_velocity;
    const uint8_t acceleration;
    const uint8_t grip;
    const uint8_t maneuverability;
    uint8_t current_velocity;
    uint8_t health;
    Point position;
    Color color;
    CarState *state;

    public:
    Car(uint8_t width, uint8_t height, uint8_t max_velocity,
        uint8_t acceleration, uint8_t grip, uint8_t maneuverability,
        Point initial_position, Color color);
    void changeState(PlayerAction action);
    ~Car();
};

#endif
