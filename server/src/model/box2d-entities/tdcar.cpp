#include "../../../include/model/box2d-entities/tdcar.h"
#include "../../../include/model/box2d-entities/tdtire.h"
#include "../../../include/model/cars/car.h"
#include "../../../include/model/point.h"
#include <Box2D/Box2D.h>
#include <iostream>

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define ANGLECORRECTION 0.05f

TDCar::TDCar(b2World *world, uint16_t max_velocity, float acceleration,
             float grip, float maneuverability, float rotation, Point initial_position) {

    checkConfigLimits(max_velocity, acceleration, grip, maneuverability);
    world->SetContactListener(&contactListener);

    //create car body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(initial_position.getX(), initial_position.getY());
    bodyDef.angle = (DEGTORAD)*(rotation * -1.0f) + (ANGLECORRECTION);
    body = world->CreateBody(&bodyDef);
    body->SetAngularDamping(1);
    body->SetLinearDamping(1);
    body->SetUserData(&contacting);

    b2Vec2 vertices[8];
    vertices[0].Set(1.5, 0);
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

    float maxForwardSpeed = 50.0f + max_velocity;
    float maxBackwardSpeed = -40;
    float backTireMaxDriveForce = 200.0f + 2.0 * acceleration;
    float frontTireMaxDriveForce = 300.0f + 2.0 * acceleration;
    float frontTireMaxLateralImpulse = 7.5f + (maneuverability - 50) / 10.0f;
    float backTireMaxLateralImpulse = 8.5f + (maneuverability + grip - 100) / 10.f;

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

void TDCar::checkConfigLimits(uint16_t & max_velocity, float & acceleration,
                        float & grip, float & maneuverability) {
    if (max_velocity > 100) max_velocity = 100;
    if (acceleration > 100) acceleration = 100;
    if (grip > 100) grip = 100;
    if (maneuverability > 100) maneuverability = 100;
    if (max_velocity < 0) max_velocity = 0;
    if (acceleration < 0) acceleration = 0;
    if (grip < 0) grip = 0;
    if (maneuverability < 0) maneuverability = 0;
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
    float lockAngle = 35 * (DEGTORAD);
    float turnSpeedPerSec = 160 * (DEGTORAD);//from lock to lock in 0.5 sec
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

void TDCar::newPosition(Point point) {
    body->SetTransform(b2Vec2(point.getX(),point.getY()),body->GetAngle());
    for (int i = 0; i < tires.size(); i++)
        tires[i]->newPosition(point.getX(), point.getY());
    body->SetLinearVelocity(b2Vec2(0,0));  // avoids weird effects of teleporting
    body->SetAngularVelocity(0);  // avoids weird effects of teleporting
}

void TDCar::modifySpeedByFactor(float32 factor) {
    b2Vec2 vel = body->GetLinearVelocity();
    body->SetLinearVelocity(b2Vec2(vel.x*factor, vel.y*factor));
}

bool TDCar::isContacting() {
    bool aux = contacting;
    contacting = false;
    return aux;
}

void ContactListener::BeginContact(b2Contact* contact) {
    void * data = contact->GetFixtureA()->GetBody()->GetUserData();
    if (data) {
        bool * cont = static_cast<bool*>(data);
        *cont = true;
    }
    data = contact->GetFixtureB()->GetBody()->GetUserData();
    if (data) {
        bool * cont = static_cast<bool*>(data);
        *cont = true;
    }
}

void ContactListener::EndContact(b2Contact* contact) {}
