#include "../include/Block.h"

Block::Block(int xFrom, int yFrom, PicType type) {
    this->xFrom = xFrom;
    this->yFrom = yFrom;
    this->type = type;
}

Block::~Block() {

}

int Block::getX() const {
    return this->xFrom;
}

int Block::getY() const {
    return this->yFrom;
}

PicType Block::getType() const {
    return this->type;
}
