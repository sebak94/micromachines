//
// Created by fperezboco on 27/10/19.
//

#include <jsoncpp/json/json.h>
#include <cmath>
#include "Track.h"
#include "TrackList.h"

// Loads data of 1 track in position <trackNumber> of the json file.
void Track::loadTrack(const Json::Value &fileTracks, int trackNumber) {
    // loads list of elements as string temporarily
    for (int j = 0; j < fileTracks[TRACKS_ID][trackNumber][LAYOUT_ID].size(); j++)
        trackLayout.emplace_back(fileTracks[TRACKS_ID][trackNumber][LAYOUT_ID][j].asString());
    width = fileTracks[TRACKS_ID][trackNumber][WIDTH_ID].asInt();  // track width in blocks
    height = fileTracks[TRACKS_ID][trackNumber][HEIGHT_ID].asInt();  // track width in blocks
    name = fileTracks[TRACKS_ID][trackNumber][NAME_ID].asString();
    startRow = fileTracks[TRACKS_ID][trackNumber][START_ID][0].asInt();  // row
    startCol = fileTracks[TRACKS_ID][trackNumber][START_ID][1].asInt();  // col
    initLayout();  // fills all blocks as empty (grass)
    configure();  // sets not empty blocks in matricial order
    trackLayout.clear();  // releases temporal strings
}

// Gets track name
std::string Track::getName() {
    return name;
}

// Print tracks sketch to terminal
void Track::print() {
    int column = 0;
    for (int i=0; i < trackPartData.size(); i++) {
        printElem(trackPartData[i]);
        column++;
        if (column == width) {
            std::cout << std::endl;
            column = 0;
        }
    }
}

// Prints individual parts of the track
void Track::printElem(const TrackPartData & part) {
    if (part.getType()==empty)
        std::cout << "░";
    if (part.getType()==downRight)
        std::cout << "╔";
    if (part.getType()==downLeft)
        std::cout << "╗";
    if (part.getType()==upRight)
        std::cout << "╚";
    if (part.getType()==upLeft)
        std::cout << "╝";
    if (part.getType()==horizontal)
        std::cout << "═";
    if (part.getType()==vertical)
        std::cout << "║";
}

// Counts rows and cols from 0. Loads position of element in meters.
void Track::loadElem(int row, int col, trackElem elem) {
    TrackPartData part;
    part.loadType(elem);
    part.loadPos(height - row, col);
    part.setID(partCounter);
    partCounter++;
    trackPartData[row*width + col] = part;
}

// Gets type (curve, straight line, etc)
trackElem Track::getElemType(int row, int col) {
    return trackPartData[row*width + col].getType();
}

// Checks if on Track based on car position [meters]
bool Track::isOnTrack(int posX, int posY) {
    TrackPartData part = getTrackPart(posX, posY);
    trackElem type = part.getType();
    int x = posX - part.getPosX(); // to relative coords
    int y = posY - part.getPosY(); // to relative coords
    if (type == horizontal || type == vertical) {
        return true;
    } else if (type == empty) {
        return false;
    } else if (type == downLeft) {
        return inCurveRange(false, false, x, y);
    } else if (type == downRight){
        return inCurveRange(true, false, x, y);
    } else if (type == upRight){
        return inCurveRange(true, true, x, y);
    } else if (type == upLeft){
        return inCurveRange(false, true, x, y);
    } else {
        return false;
    }
}

// Checks if car is out of radius of curve using polar coordinates.
bool Track::inCurveRange(bool invertedX, bool invertedY, int x, int y) {
    if ( invertedX )
        x = BLOCKSIZE - x;
    if ( invertedY )
        y = BLOCKSIZE - y;
    return sqrt(x*x+y*y) < BLOCKSIZE;
}

/* Transforms sequential order of the track (json)
 * to a matricial layout */
void Track::configure() {
    int row = startRow;
    int col = startCol;
    loadElem(row, col, identifyElem(trackLayout[0]));
    trackElem prevElem = getElemType(row, col);
    trackElem actualElem;
    col++;  // assumes top-left corner is a down-right curve
    for (int i=1; i < trackLayout.size(); i++){
        actualElem = identifyElem(trackLayout[i]);
        loadElem(row, col, actualElem);
        setNextCoord(row, col, actualElem, prevElem);
        if (isCurve(actualElem))
            prevElem = actualElem;
    }
}

bool Track::isCurve(const trackElem & elem) {
    return !(elem == horizontal || elem == vertical);
}

// Transforms json strings to elements of track
trackElem Track::identifyElem(const std::string & layoutElem){
    if (layoutElem == LAYOUT_DL)
        return downLeft;
    else if (layoutElem == LAYOUT_DR)
        return downRight;
    else if (layoutElem == LAYOUT_H)
        return horizontal;
    else if (layoutElem == LAYOUT_UL)
        return upLeft;
    else if (layoutElem == LAYOUT_UR)
        return upRight;
    else if (layoutElem == LAYOUT_V)
        return vertical;
}

/* Finds next position to emplace the
 * track piece based on the last curve */
void Track::setNextCoord(int & row,
                         int & col,
                         trackElem elem,
                         trackElem prev){
    if (elem == horizontal && (prev == upRight || prev == downRight))
        col++;
    else if (elem == horizontal && (prev == upLeft || prev == downLeft))
        col--;
    else if (elem == vertical && (prev == upRight || prev == upLeft))
        row--;
    else if(elem == vertical && (prev == downRight || prev == downLeft))
        row++;
    else if (elem == downRight && prev == downLeft)
        row++;
    else if (elem == upRight && prev == upLeft)
        row--;
    else if (elem == upRight && prev == downLeft)
        col++;
    else if (elem == downLeft && prev == downRight)
        row++;
    else if (elem == upLeft && prev == upRight)
        row--;
    else if (elem == upLeft && prev == downLeft)
        col--;
    else if (elem == downRight && prev == upRight)
        row--;
    else if (elem == downRight && prev == upLeft)
        col++;
}

// fills whole track as empty (grass)
void Track::initLayout() {
    TrackPartData emptyPart;
    trackPartData.clear();
    trackPartData.reserve(width * height);
    for (int i = 0; i < width * height; i++)
        trackPartData.emplace_back(emptyPart);
}

/* Transforms positions in meters to indexes of matrix.
 *-1 to transform to display coordinates. See loadPos().*/
TrackPartData & Track::getTrackPart(int posX, int posY) {
    int row = posToIndex((height-1)*BLOCKSIZE - findNearestPos(posY));
    int col = posToIndex(findNearestPos(posX));
    return trackPartData[row*width + col];
}

// Finds nearest bottom-left corner in BLOCKSIZE multiples
int Track::findNearestPos(int pos) {
    for(int nearest = 0; nearest < MAXTRACKSIZE; nearest+=BLOCKSIZE ) {
        if(pos >= nearest && pos < nearest + BLOCKSIZE)
            return nearest;
    }
    return -1;  // not found
}

// Transforms from meters to matricial display indexes
int Track::posToIndex(int pos) {
    return pos/BLOCKSIZE;
}