#ifndef __CAR_H__
#define __CAR_H__

#include "../model.h"
#include "../point.h"
#include "color.h"
#include <cstdint>
#include <string>
#include <unordered_map>

class Car: public Model {
    protected:
    const uint8_t width;
    const uint8_t height;
    const uint8_t max_velocity;
    const uint8_t acceleration;
    const uint8_t grip;
    const uint8_t maneuverability;
    int current_velocity;
    uint8_t health;
    int16_t rotation;
    Point position;
    Color color;
    std::unordered_map<std::string, bool> states;

    public:
    Car(uint8_t width, uint8_t height, uint8_t max_velocity,
        uint8_t acceleration, uint8_t grip, uint8_t maneuverability,
        Point initial_position, ColorType color);
    void updateState(char action);
    void addPositionX(uint16_t x);
    void addPositionY(uint16_t y);
    void addRotation(int16_t rotation);
    virtual void update() override;
    virtual std::string serialize() override;
    void accelerateCar();
    void leftCar();
    void rightCar();
    void breakCar();
    void updateCarPosition();
    ~Car();

};

#endif
