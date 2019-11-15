#include "../../../include/model/point.h"
#include "../../../include/model/cars/car.h"
#include "../../../include/model/cars/color.h"
#include <string>
#include <cstdint>
#include <math.h>
#include <Box2D/Box2D.h>

#define PI 3.14159265
#define RADTODEG 57.295779513082320876f

Car::Car(uint8_t width, uint8_t height, uint16_t max_velocity, uint8_t acceleration, uint8_t grip,
         uint8_t maneuverability, Point initial_position, ColorType color, uint16_t rotation, b2World *world,
         int startID) :
        width(width), height(height), max_velocity(max_velocity),
        acceleration(acceleration), grip(grip),
        maneuverability(maneuverability), health(100), color(color),
        td_car(world, max_velocity, acceleration, grip, maneuverability,
        rotation, initial_position), control_state(0), lastTrackID(startID) {
}

void Car::updateState(char action) {
    switch (action) {
        case 'L' : control_state |= TDC_LEFT;  break;
        case 'R' : control_state |= TDC_RIGHT; break;
        case 'A' : control_state |= TDC_UP;    break;
        case 'B' : control_state |= TDC_DOWN;  break;
        case 'W' : control_state &= ~TDC_LEFT;  break;
        case 'X' : control_state &= ~TDC_RIGHT; break;
        case 'Y' : control_state &= ~TDC_UP;    break;
        case 'Z' : control_state &= ~TDC_DOWN;  break;
        default: ;
    }
}

void Car::update() {
    td_car.update(control_state);
}

void Car::updatePos(Point point) {
    td_car.updatePoisition(point);
}

int Car::getPosX() {
    return (int)td_car.body->GetPosition().x;
}

int Car::getPosY() {
    return (int)td_car.body->GetPosition().y;
}

void Car::setTrackID(int ID) {
    lastTrackID = ID;
}

int Car::getTrackID() {
    return lastTrackID;
}

std::string Car::serialize() {
    // La serializacion es:
    // current_velocity,health,rotation,x,y,color\n
    return std::to_string(0) + ","
           + std::to_string(health) + ","
           + std::to_string((int)(td_car.body->GetAngle()*RADTODEG*-1)) + ","
           + std::to_string((int)td_car.body->GetPosition().x) + ","
           + std::to_string((int)td_car.body->GetPosition().y) + ","
           + color.name() + "\n";
}

Car::~Car() {}
