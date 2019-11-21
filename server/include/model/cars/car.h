#ifndef __CAR_H__
#define __CAR_H__

#include "../model.h"
#include "../point.h"
#include "../box2d-entities/tdcar.h"
#include "color.h"
#include <cstdint>
#include <string>
#include <Box2D/Box2D.h>

enum {
    TDC_LEFT   = 0x1,
    TDC_RIGHT  = 0x2,
    TDC_UP     = 0x4,
    TDC_DOWN   = 0x8
};

class Car: public Model {
    protected:
    const uint8_t width;
    const uint8_t height;
    const uint8_t max_velocity;
    const uint8_t acceleration;
    const uint8_t grip;
    const uint8_t maneuverability;
    uint8_t health;
    Color color;
    TDCar td_car;
    int control_state;
    int lastTrackID = -1;
    int laps = 0;

    public:
    Car(uint8_t width, uint8_t height, uint16_t max_velocity, uint8_t acceleration, uint8_t grip,
        uint8_t maneuverability, Point initial_position, ColorType color, uint16_t rotation, b2World *world,
        int startID);
    static Car* createBlackCar(b2World *world, const Point &startingPoint,
                            uint16_t rot, int startID);
    static Car* createBlueCar(b2World *world, const Point &startingPoint,
                            uint16_t rot, int startID);
    static Car* createRedCar(b2World *world, const Point &startingPoint,
                            uint16_t rot, int startID);
    static Car* createWhiteCar(b2World *world, const Point &startingPoint,
                            uint16_t rot, int startID);
    static Car* createYellowCar(b2World *world, const Point &startingPoint,
                            uint16_t rot, int startID);
    void updateState(char action);
    virtual void update() override;
    virtual std::string serialize() override;
    void newPos(Point point);
    int getPosX();
    int getPosY();
    void setTrackID(int ID);
    int getTrackID();
    ~Car();

    void updateLaps();

    void reduceSpeed(float32 factor);

    int getLaps();

    std::string getColor();
};

#endif
