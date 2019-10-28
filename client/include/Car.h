#ifndef __CAR_H__
#define __CAR_H__

#include "PicType.h"

class Car {
private:
    int x;
    int y;
    int degrees;
    int health;
    PicType type;

public:
    Car(int x, int y, int degrees, int health, PicType type);
    ~Car();
    int getX() const;
    int getY() const;
    int getDegrees() const;
    int getHealth() const;
    PicType getType() const;
};

#endif
