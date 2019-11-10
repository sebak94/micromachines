#ifndef __DESTRUCTION_LISTENER_H__
#define __DESTRUCTION_LISTENER_H__

#include "fixture_user_data.h"
#include <Box2D/Box2D.h>

class DestructionListener : public b2DestructionListener {
    void SayGoodbye(b2Fixture* fixture) {
        if (FixtureUserData* fud = (FixtureUserData*)fixture->GetUserData())
            delete fud;
    }

    //(unused but must implement all pure virtual functions)
    void SayGoodbye(b2Joint* joint) {}
};

#endif
