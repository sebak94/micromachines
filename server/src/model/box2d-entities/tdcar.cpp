#include "../../../include/model/box2d-entities/tdcar.h"
#include "../../../include/model/box2d-entities/tdtire.h"
#include "../../../include/model/cars/car.h"
#include "../../../include/model/point.h"
#include <Box2D/Box2D.h>
#include <iostream>

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

TDCar::TDCar(b2World *world, uint16_t max_velocity, uint8_t acceleration,
             uint8_t grip, uint8_t maneuverability, uint16_t rotation, Point initial_position) {
    //create car body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(initial_position.getX(), initial_position.getY());
    bodyDef.angle = DEGTORAD*(rotation * -1);
    body = world->CreateBody(&bodyDef);
    body->SetAngularDamping(1);
    body->SetLinearDamping(1);

    b2Vec2 vertices[8];
    vertices[0].Set(5, 0);
    vertices[1].Set(3,2.5);
    vertices[2].Set(2.8, 5.5);
    vertices[3].Set(1, 10);
    vertices[4].Set(-1, 10);
    vertices[5].Set(-2.8, 5.5);
    vertices[6].Set(-3, 2.5);
    vertices[7].Set(-1.5, 0);
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices, 8);
    b2Fixture* fixture = body->CreateFixture(&polygonShape, 0.1f);//shape, density

    //prepare common joint parameters
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = body;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorB.SetZero();//center of tire

    float maxForwardSpeed = max_velocity;
    float maxBackwardSpeed = -40;
    float backTireMaxDriveForce = 400;
    float frontTireMaxDriveForce = 500;
    float backTireMaxLateralImpulse = 8.5;
    float frontTireMaxLateralImpulse = 7.5;

    //back left tire
    TDTire* tire = new TDTire(world, initial_position);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
    jointDef.bodyB = tire->body;
    jointDef.localAnchorA.Set( -3, 0.75f );
    world->CreateJoint( &jointDef );
    tires.push_back(tire);

    //back right tire
    tire = new TDTire(world, initial_position);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
    jointDef.bodyB = tire->body;
    jointDef.localAnchorA.Set( 3, 0.75f );
    world->CreateJoint( &jointDef );
    tires.push_back(tire);

    //front left tire
    tire = new TDTire(world, initial_position);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
    jointDef.bodyB = tire->body;
    jointDef.localAnchorA.Set( -3, 8.5f );
    flJoint = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    tires.push_back(tire);

    //front right tire
    tire = new TDTire(world, initial_position);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
    jointDef.bodyB = tire->body;
    jointDef.localAnchorA.Set( 3, 8.5f );
    frJoint = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    tires.push_back(tire);
}

TDCar::~TDCar() {
    for (int i = 0; i < tires.size(); i++)
        delete tires[i];
}

void TDCar::update(int control_state) {
    for (int i = 0; i < tires.size(); i++)
        tires[i]->updateFriction();
    for (int i = 0; i < tires.size(); i++)
        tires[i]->updateDrive(control_state);

    //control steering
    float lockAngle = 35 * DEGTORAD;
    float turnSpeedPerSec = 160 * DEGTORAD;//from lock to lock in 0.5 sec
    float turnPerTimeStep = turnSpeedPerSec / 60.0f;
    float desiredAngle = 0;
    switch (control_state & (TDC_LEFT|TDC_RIGHT)) {
        case TDC_LEFT:  desiredAngle = lockAngle;  break;
        case TDC_RIGHT: desiredAngle = -lockAngle; break;
        default: ;
    }
    float angleNow = flJoint->GetJointAngle();
    float angleToTurn = desiredAngle - angleNow;
    angleToTurn = b2Clamp( angleToTurn, -turnPerTimeStep, turnPerTimeStep );
    float newAngle = angleNow + angleToTurn;
    flJoint->SetLimits( newAngle, newAngle );
    frJoint->SetLimits( newAngle, newAngle );
}

void TDCar::updatePoisition(Point point) {
    body->SetTransform(b2Vec2(point.getX(),point.getY()),body->GetAngle());
}