#ifndef __TDTIRE_H__
#define __TDTIRE_H__

#include <Box2D/Box2D.h>
#include "../point.h"

class TDTire {
    private:
    float maxForwardSpeed;
    float maxBackwardSpeed;
    float maxDriveForce;
    float maxLateralImpulse;

    public:
    b2Body* body;
    void setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed,
        float maxDriveForce, float maxLateralImpulse);
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();
    void updateFriction();
    void updateDrive(int control_state);
    void updateTurn(int control_state);

    TDTire(b2World* world, Point initial_position);
    ~TDTire();
};

#endif
