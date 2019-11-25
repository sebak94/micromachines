//
// Created by fperezboco on 9/11/19.
//

#include "../include/Grandstand.h"

Grandstand::Grandstand(trackPartType type,
                       int row,
                       int col,
                       int blockSize,
                       int height) {
    this->type = type;
    this->row = row;
    this->col = col;
    this->posX = blockSize * col;
    this->posY = blockSize * (height - row - 1);
}

bool Grandstand::isPublic(trackPartType t) {
    return t == public1Left ||
           t == public1Down ||
           t == public1Right ||
           t == public1Up;
}

trackPartType Grandstand::getType() {
    return type;
}

std::string Grandstand::getTypeAsString() {
    if (type == public1Left) return LAYOUT_PUB1LEFT;
    if (type == public1Right) return LAYOUT_PUB1RIGHT;
    if (type == public1Up) return LAYOUT_PUB1UP;
    if (type == public1Down) return LAYOUT_PUB1DOWN;
    return LAYOUT_PUB1LEFT;
}

int Grandstand::getRow() {
    return row;
}

int Grandstand::getCol() {
    return col;
}

int Grandstand::getPosX() {
    return posX;
}

int Grandstand::getPosY() {
    return posY;
}

// Transforms json strings to elements of track
trackPartType Grandstand::identifyElem(const std::string & layoutElem){
    if (layoutElem == LAYOUT_PUB1LEFT) return public1Left;
    if (layoutElem == LAYOUT_PUB1DOWN) return public1Down;
    if (layoutElem == LAYOUT_PUB1RIGHT) return public1Right;
    if (layoutElem == LAYOUT_PUB1UP) return public1Up;
    return empty;
}