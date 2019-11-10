//
// Created by fperezboco on 9/11/19.
//

#ifndef MICROMACHINES_GRANDSTAND_H
#define MICROMACHINES_GRANDSTAND_H

#include "../include/Track.h"

#include "../../editor/include/TrackBlock.h"

class Track;

class Grandstand : public Track {
private:
    int posX;
    int posY;
    int row;
    int col;
    bool powerUpThrown = false;
    trackPartType type;

public:
    Grandstand(trackPartType type, int row, int col);
};


#endif //MICROMACHINES_GRANDSTAND_H
