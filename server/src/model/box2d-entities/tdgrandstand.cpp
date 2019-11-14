#include "../../../include/model/box2d-entities/tdgrandstand.h"
#include "../../../../common/include/Grandstand.h"
#include <Box2D/Box2D.h>

TDGrandstand::TDGrandstand(b2World *world, Grandstand &gs) {
    b2BodyDef gsBodyDef;
    gsBodyDef.type = b2_staticBody;
    gsBodyDef.position.Set(gs.getPosX(), gs.getPosY());

    body = world->CreateBody(&gsBodyDef);
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox(gs.getMetreWidth() / 2, gs.getMetreHeight() / 2);

    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    body->CreateFixture(&boxFixtureDef);
}
