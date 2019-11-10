#ifndef __TDCAR_H__
#define __TDCAR_H__

#include "tdtire.h"
#include <Box2D/Box2D.h>
#include <vector>

class TDCar {
    private:
    std::vector<TDTire*> tires;
    b2RevoluteJoint *flJoint, *frJoint;

    public:
    b2Body* body;
    TDCar(b2World *world, uint16_t max_velocity, uint8_t acceleration,
        uint8_t grip, uint8_t maneuverability);
    void update(int control_state);
    ~TDCar();
};

#endif
