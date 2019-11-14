#ifndef __TDGRANDSTAND_H__
#define __TDGRANDSTAND_H__

#include "../../../../common/include/Grandstand.h"
#include <Box2D/Box2D.h>

class TDGrandstand {
    private:
    b2Body* body;

    public:
    TDGrandstand(b2World *world, Grandstand &gs);
    ~TDGrandstand() = default;
};

#endif
