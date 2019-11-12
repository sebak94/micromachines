#include "../../../include/model/box2d-entities/tdtire.h"
#include "../../../include/model/cars/car.h"
#include <Box2D/Box2D.h>

TDTire::TDTire(b2World* world, Point initial_position) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(initial_position.getX(), initial_position.getY());
    body = world->CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.3f, 0.8f);
    body->CreateFixture(&polygonShape, 1);//shape, density
    
    body->SetUserData(this);
}

void TDTire::setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed,
    float maxDriveForce, float maxLateralImpulse) {
    this->maxForwardSpeed = maxForwardSpeed;
    this->maxBackwardSpeed = maxBackwardSpeed;
    this->maxDriveForce = maxDriveForce;
    this->maxLateralImpulse = maxLateralImpulse;
}

b2Vec2 TDTire::getLateralVelocity() {
    b2Vec2 currentRightNormal = body->GetWorldVector(b2Vec2(1,0));
    return b2Dot(currentRightNormal, body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 TDTire::getForwardVelocity() {
    b2Vec2 currentForwardNormal = body->GetWorldVector( b2Vec2(0,1) );
    return b2Dot( currentForwardNormal, body->GetLinearVelocity() ) * currentForwardNormal;
}

void TDTire::updateFriction() {
    //lateral linear velocity
    b2Vec2 impulse = body->GetMass() * -getLateralVelocity();
    if (impulse.Length() > maxLateralImpulse) {
      impulse *= maxLateralImpulse / impulse.Length();
    }
    body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);

    //angular velocity
    body->ApplyAngularImpulse(0.1f * body->GetInertia() * -body->GetAngularVelocity(), true);
    
    //forward linear velocity
    b2Vec2 currentForwardNormal = getForwardVelocity();
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragForceMagnitude = -2 * currentForwardSpeed;
    body->ApplyForce(dragForceMagnitude * currentForwardNormal, body->GetWorldCenter(), true);
}
 
void TDTire::updateDrive(int control_state) {
    //find desired speed
    float desiredSpeed = 0;
    switch (control_state & (TDC_UP|TDC_DOWN)) {
        case TDC_UP: desiredSpeed = maxForwardSpeed; break;
        case TDC_DOWN: desiredSpeed = maxBackwardSpeed; break;
        default: return;
    }
    
    //find current speed in forward direction
    b2Vec2 currentForwardNormal = body->GetWorldVector(b2Vec2(0,1));
    float currentSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);
    
    //apply necessary force
    float force = 0;
    if (desiredSpeed > currentSpeed)
        force = maxDriveForce;
    else if (desiredSpeed < currentSpeed)
        force = -maxDriveForce;
    else
        return;
    body->ApplyForce(force * currentForwardNormal, body->GetWorldCenter(), true);
}

void TDTire::updateTurn(int control_state) {
    float desiredTorque = 0;
    switch (control_state & (TDC_LEFT|TDC_RIGHT)) {
        case TDC_LEFT: desiredTorque = 15; break;
        case TDC_RIGHT: desiredTorque = -15; break;
        default: ;
    }
    body->ApplyTorque(desiredTorque, true);
}

TDTire::~TDTire() {
    body->GetWorld()->DestroyBody(body);
}
