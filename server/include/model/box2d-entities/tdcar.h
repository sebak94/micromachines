#ifndef __TDCAR_H__
#define __TDCAR_H__

#include "tdtire.h"
#include "../point.h"
#include <Box2D/Box2D.h>
#include <vector>

class ContactListener : public b2ContactListener {
private:
public:
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
};

class TDCar {
    private:
    std::vector<TDTire*> tires;
    b2RevoluteJoint *flJoint, *frJoint;
    ContactListener contactListener;
    b2Fixture * fix;
    bool contacting = false;

    public:
    b2Body* body;
    TDCar(b2World *world, uint16_t max_velocity, float acceleration,
          float grip, float maneuverability, float rotation, Point initial_position);
    void update(int control_state);
    ~TDCar();
    void newPosition(Point point);
    void modifySpeedByFactor(float32 factor);
    static void
    checkConfigLimits(uint16_t &max_velocity, float &acceleration, float &grip,
                      float &maneuverability);

    bool isContacting();
};

#endif
