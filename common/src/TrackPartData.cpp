//
// Created by fede on 1/11/19.
//

#include "TrackPartData.h"

// Loads type of element
void TrackPartData::loadType(trackElem elem) {
    type = elem;
}

// Loads into cardinal meters
void TrackPartData::loadPos(int row, int col) {
    posX = row*BLOCKSIZE;
    posY = (col - 1)*BLOCKSIZE;
}

// Returns track element type
trackElem TrackPartData::getType() const{
    return type;
}

// Gets left corner position of track element in meters
int TrackPartData::getPosX() {
    return posX;
}

// Gets bottom corner position of track element in meters
int TrackPartData::getPosY() {
    return posY;
}

void TrackPartData::setID(int ID) {
    trackElemID = ID;
}
