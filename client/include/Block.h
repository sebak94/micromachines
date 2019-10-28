#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <utility>
#include "PicType.h"

class Block {
private:
    int xFrom;
    int yFrom;
    PicType type;

public:
    Block(int xFrom, int yFrom, PicType type);
    ~Block();
    int getX() const;
    int getY() const;
    PicType getType() const;
};

#endif
