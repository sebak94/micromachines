#ifndef __CAR_H__
#define __CAR_H__

#include "./model.h"

class Car: public Model {
    public:
    virtual void update() override;
    virtual void serialize() override;
};

#endif
