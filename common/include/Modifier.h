#ifndef __MODIFIER_H__
#define __MODIFIER_H__

#include <string>
#include "../../common/include/ModType.h"

class Modifier {
private:
    int x;
    int y;
    ModType type;

public:
    Modifier(int x, int y, std::string type);
    ~Modifier();
    void setType(std::string str);
    std::string getType() const;
    int getX() const;
    int getY() const;
    std::string serialize();
};

#endif
