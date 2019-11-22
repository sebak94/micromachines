#include "../../../include/model/point.h"
#include "../../../include/model/cars/car.h"
#include "../../../include/model/cars/color.h"
#include <string>
#include <cstdint>
#include <math.h>
#include <Box2D/Box2D.h>
#include <iostream>

#define PI 3.14159265
#define RADTODEG 57.295779513082320876f
#define VEL_CONFIG "max velocity [%]"
#define ACCEL_CONFIG "acceleration [%]"
#define GRIP_CONFIG "grip [%]"
#define MANEU_CONFIG "maneuverability [%]"

Car::Car(uint8_t width, uint8_t height, uint16_t max_velocity,
         float acceleration, float grip, float maneuverability,
         Point initial_position, ColorType color, uint16_t rotation,
         b2World *world, int startID) :
        width(width), height(height), max_velocity(max_velocity),
        acceleration(acceleration), grip(grip),
        maneuverability(maneuverability), health(100), color(color),
        td_car(world, max_velocity, acceleration, grip, maneuverability,
        rotation, initial_position), control_state(0), lastTrackID(startID){
}

Car * Car::createBlackCar(b2World *world, const Point &startingPoint,
                          float rot,
                          int startID, Config & config) {
    return std::move(new Car(100, 40, config.getAsUint16(VEL_CONFIG),
            config.getAsFloat(ACCEL_CONFIG), config.getAsFloat(GRIP_CONFIG),
            config.getAsFloat(MANEU_CONFIG), startingPoint, black, rot, world,
            startID));
}

Car* Car::createBlueCar(b2World *world, const Point &startingPoint,
                        float rot, int startID, Config & config) {
    return std::move(new Car(100, 40, config.getAsUint16(VEL_CONFIG),
            config.getAsFloat(ACCEL_CONFIG), config.getAsFloat(GRIP_CONFIG),
            config.getAsFloat(MANEU_CONFIG), startingPoint, blue, rot, world,
            startID));
}

Car* Car::createRedCar(b2World *world, const Point &startingPoint,
                       float rot, int startID, Config & config) {
    return std::move(new Car(100, 40, config.getAsUint16(VEL_CONFIG),
            config.getAsFloat(ACCEL_CONFIG), config.getAsFloat(GRIP_CONFIG),
            config.getAsFloat(MANEU_CONFIG), startingPoint, red, rot, world,
            startID));
}

Car* Car::createWhiteCar(b2World *world, const Point &startingPoint,
                         float rot, int startID, Config & config) {
    return std::move(new Car(100, 40, config.getAsUint16(VEL_CONFIG),
            config.getAsFloat(ACCEL_CONFIG), config.getAsFloat(GRIP_CONFIG),
            config.getAsFloat(MANEU_CONFIG), startingPoint, white, rot, world,
            startID));
}

Car* Car::createYellowCar(b2World *world, const Point &startingPoint,
                          float rot, int startID, Config & config) {
    return std::move(new Car(100, 40, config.getAsUint16(VEL_CONFIG),
            config.getAsFloat(ACCEL_CONFIG), config.getAsFloat(GRIP_CONFIG),
            config.getAsFloat(MANEU_CONFIG), startingPoint, yellow, rot, world,
            startID));
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

void Car::newPos(Point point) {
    td_car.newPosition(point);
}

int Car::getPosX() {
    return (int)td_car.body->GetPosition().x;
}

int Car::getPosY() {
    return (int)td_car.body->GetPosition().y;
}

void Car::modifySpeedByFactor(float32 factor){
    td_car.modifySpeedByFactor(factor);
}

void Car::setTrackID(int ID) {
    lastTrackID = ID;
}

int Car::getTrackID() {
    return lastTrackID;
}

void Car::updateLaps() {
    laps++;
    std::cout << std::endl << std::endl << laps << std::endl;
}

std::string Car::serialize() {
    // La serializacion es:
    // current_velocity,health,rotation,x,y,laps,color\n
    return std::to_string(0) + ","
           + std::to_string(health) + ","
           + std::to_string((int)(td_car.body->GetAngle()*RADTODEG*-1)) + ","
           + std::to_string((int)td_car.body->GetPosition().x) + ","
           + std::to_string((int)td_car.body->GetPosition().y) + ","
           + std::to_string(laps) + ","
           + color.name() + "\n";
}

int Car::getLaps() {
    return laps;
}

std::string Car::getColor() {
    return color.name();
}

Car::~Car() {}
