//
// Created by fperezboco on 9/11/19.
//

#ifndef MICROMACHINES_GRANDSTAND_H
#define MICROMACHINES_GRANDSTAND_H

#define LAYOUT_PUB1LEFT "pub1Left"
#define LAYOUT_PUB1RIGHT "pub1Right"
#define LAYOUT_PUB1UP "pub1Up"
#define LAYOUT_PUB1DOWN "pub1Down"

#include "../../editor/include/TrackBlock.h"

class Track;

class Grandstand {
private:
    int posX;
    int posY;
    int row;
    int col;
    bool powerUpThrown = false;
    trackPartType type;

public:
    Grandstand(trackPartType type, int row, int col, int blockSize, int height);
    static bool isPublic(trackPartType t);
    trackPartType getType();
    int getRow();
    int getCol();
    int getPosX();
    int getPosY();

    std::string getTypeAsString();

    static trackPartType identifyElem(const std::string &layoutElem);
};


#endif //MICROMACHINES_GRANDSTAND_H
