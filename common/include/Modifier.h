#ifndef __MODIFIER_H__
#define __MODIFIER_H__

#include <string>
#include "../../common/include/ModType.h"

typedef enum {
    left,
    right,
    up,
    down
} direction;

class Modifier {
private:
    int x;
    int y;
    ModType type;
    direction dir = left;

public:
    Modifier(int x, int y, std::string &type, direction dir);
    ~Modifier();
    void setType(std::string &str);
    std::string getType() const;
    int getX() const;
    int getY() const;
    std::string serialize();

    void travel(int step);
};

#endif
