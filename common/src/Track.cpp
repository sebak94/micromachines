//
// Created by fperezboco on 27/10/19.
//

#include <jsoncpp/json/json.h>
#include <cmath>
#include "../include/Track.h"

/* Constructor: Creates track for track editor */
Track::Track(int width, int height, const std::string &name) {
    TrackPartData emptyPart;
    this->name = name;
    this->width = width;
    this->height = height;
    partCounter = 0;
    trackPartData.clear();
    trackPartData.reserve(width * height);
    for (int i = 0; i < width * height; i++)
        trackPartData.emplace_back(emptyPart);
}

// Loads data of 1 track in position <trackNumber> of the json file.
void Track::loadTrack(const Json::Value &fileTracks, int trackNumber) {
    width = fileTracks[TRACKS_ID][trackNumber][WIDTH_ID].asInt();  // track width in blocks
    height = fileTracks[TRACKS_ID][trackNumber][HEIGHT_ID].asInt();  // track width in blocks
    name = fileTracks[TRACKS_ID][trackNumber][NAME_ID].asString();
    startRow = fileTracks[TRACKS_ID][trackNumber][START_ID][0].asInt();  // finish row
    startCol = fileTracks[TRACKS_ID][trackNumber][START_ID][1].asInt();  // finish col
    nextToStartRow = fileTracks[TRACKS_ID][trackNumber][NEXT_TO_START_ID][0].asInt();  // To set way
    nextToStartCol = fileTracks[TRACKS_ID][trackNumber][NEXT_TO_START_ID][1].asInt();  // To set way
    std::vector<std::string> fileLayout{}; // loads list of elements as temporal strings
    initLayout();  // fills all blocks as empty (grass)
    for (const auto & j : fileTracks[TRACKS_ID][trackNumber][LAYOUT_ID]) {
        fileLayout.emplace_back(j.asString());
    }
    loadGrandstands(fileTracks, trackNumber);
    initTrackParts(fileLayout);  // sets not empty blocks in matrix order
}

// Loads grandstands in grandstands vector and as trackPartData
void Track::loadGrandstands(const Json::Value &fileTracks, int trackNumber) {
    for (const auto & j : fileTracks[TRACKS_ID][trackNumber][GRANDSTANDS_ID]) {
        trackPartType type =  Grandstand::identifyElem(j[0].asString());
        grandstands.emplace_back(Grandstand(type,
                                            j[1].asInt(),
                                            j[2].asInt(),
                                            BLOCKSIZE,
                                            height));
        setTrackPartType(j[1].asInt(), j[2].asInt(), type);
    }
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
    else if (part.getType()==downRight)
        std::cout << "╔";
    else if (part.getType()==downLeft)
        std::cout << "╗";
    else if (part.getType()==upRight)
        std::cout << "╚";
    else if (part.getType()==upLeft)
        std::cout << "╝";
    else if (part.getType()==horizontal || part.getType()==finishH)
        std::cout << "═";
    else if (part.getType()==vertical || part.getType()==finishV)
        std::cout << "║";
    else std::cout << "X";
}

// Counts rows and cols from 0. Loads position of element in meters.
void Track::loadPart(int row, int col, trackPartType elem) {
    TrackPartData part;
    part.loadType(elem);
    part.loadPos(height - row, col);
    part.setID(partCounter);
    trackPartData[row*width + col] = part;
    if (isTrackPart(elem)) partCounter++;
}

// Checks if is a track part
bool Track::isTrackPart(trackPartType type) {
    return (type == horizontal ||
            type == vertical ||
            type == finishH ||
            type == finishV ||
            type == downRight ||
            type == downLeft ||
            type == upLeft ||
            type == upRight);
}

// Checks if is a track part
bool Track::isTrackPart(int row, int col) {
    trackPartType type = getPartType(row, col);
    return (type == horizontal ||
            type == vertical ||
            type == finishH ||
            type == finishV ||
            type == downRight ||
            type == downLeft ||
            type == upLeft ||
            type == upRight);
}

// Checks if is a straight track part
bool Track::isTrackLinePart(trackPartType type) {
    return (type == horizontal ||
            type == vertical ||
            type == finishH ||
            type == finishV);
}

// Gets type (curve, straight line, etc)
trackPartType Track::getPartType(int row, int col) {
    return trackPartData[row*width + col].getType();
}

// Checks if on Track based on car position [meters]
bool Track::isOnTrack(int posX, int posY) {
    TrackPartData part = getTrackPart(posX, posY);
    trackPartType type = part.getType();
    int x = posX - part.getPosX(); // to relative coords
    int y = posY - part.getPosY(); // to relative coords
    if (isTrackPart(type)) {
        return true;
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
 * to a matrix layout */
void Track::initTrackParts(const std::vector<std::string> & trackLayout) {
    trackSequence.clear();
    int row = startRow;
    int lastRow = row;
    int col = startCol;
    loadPart(row, col, identifyElem(trackLayout[0]));
    saveTrackSequence(row, col);
    trackPartType prevElem = setStartingPreviousTrackPart(row, col);
    trackPartType actualElem;
    row = nextToStartRow, col = nextToStartCol;
    for (int i=1; i < trackLayout.size(); i++){
        actualElem = identifyElem(trackLayout[i]);
        loadPart(row, col, actualElem);
        saveTrackSequence(row, col);
        lastRow = setNextCoord(row, col, actualElem, prevElem, lastRow);
        if (isCurve(actualElem)) {
            prevElem = actualElem;
        }
    }
}

// saves positions and sequence of Track
void Track::saveTrackSequence(int row, int col) {
    uint16_t y = (uint16_t)trackPartData[row*width + col].getPosY();
    uint16_t x = (uint16_t)trackPartData[row*width + col].getPosX();
    trackSequence.emplace(partCounter-1, Point(x,y));
}

/* Auxiliar function to start traversing the track */
trackPartType Track::setStartingPreviousTrackPart(int row, int col) {
    int t = getPartType(row, col);
    if (t == finishV && startRow < nextToStartRow) return downLeft;
    if (t == finishV && startRow > nextToStartRow) return upLeft;
    if (t == finishH && startCol < nextToStartCol) return upRight;
    if (t == finishH && startCol > nextToStartRow) return upLeft;
}

// Checks if track parts are properly connected. Assumes first is downRight
bool Track::validateTrack() {
    int row = startRow;
    int lastRow = row;
    int col = startCol;
    int i = 0;
    trackPartType actual;
    trackPartType previous = setStartingPreviousTrackPart(row, col);
    row = nextToStartRow, col = nextToStartCol;
    for (int j = 1; j <= partCounter + 1; j++){
        actual = getPartType(row, col);
        if (!validateConnection(previous, actual))
            return false;
        lastRow = setNextCoord(row, col, actual, previous, lastRow);
        if (isCurve(actual))
            previous = actual;
    }
    return true;
}

// Checks if track part is a curve
bool Track::isCurve(const trackPartType & elem) {
    return !(elem == horizontal ||
             elem == vertical ||
             elem == finishH ||
             elem == finishV);
}

// Transforms json strings to elements of track
trackPartType Track::identifyElem(const std::string & layoutElem){
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
    else if (layoutElem == LAYOUT_FH)
        return finishH;
    else if (layoutElem == LAYOUT_FV)
        return finishV;
}

/* Returns Json string track-type format based on part type*/
std::string Track::typeToFileType(int row, int col) {
    trackPartType t = getPartType(row, col);
    switch (t){
        case horizontal:
            return LAYOUT_H;
        case vertical:
            return LAYOUT_V;
        case downLeft:
            return LAYOUT_DL;
        case downRight:
            return LAYOUT_DR;
        case upLeft:
            return LAYOUT_UL;
        case upRight:
            return LAYOUT_UR;
        case finishH:
            return LAYOUT_FH;
        case finishV:
            return LAYOUT_FV;
        default:
            return LAYOUT_E;
    }
}

// Checks if "actual" Part is properly connected based on last curve "prev"
bool Track::validateConnection(trackPartType prev, trackPartType actual) {
    if ((actual == horizontal || actual == finishH)  && (prev == upRight || prev == downRight))
        return true;
    else if ((actual == horizontal || actual == finishH) && (prev == upLeft || prev == downLeft))
        return true;
    else if ((actual == vertical || actual == finishV) && (prev == upRight || prev == upLeft))
        return true;
    else if((actual == vertical || actual == finishV) && (prev == downRight || prev == downLeft))
        return true;
    else if (actual == downRight && prev == downLeft)
        return true;
    else if (actual == upRight && prev == upLeft)
        return true;
    else if (actual == upRight && prev == downLeft)
        return true;
    else if (actual == upRight && prev == downRight)
        return true;
    else if (actual == downLeft && prev == downRight)
        return true;
    else if (actual == downLeft && prev == upLeft)
        return true;
    else if (actual == downLeft && prev == upRight)
        return true;
    else if (actual == upLeft && prev == upRight)
        return true;
    else if (actual == upLeft && prev == downRight)
        return true;
    else if (actual == upLeft && prev == downLeft)
        return true;
    else if (actual == downRight && prev == upRight)
        return true;
    else if (actual == downRight && prev == upLeft)
        return true;
    else
        return false;
}

/* Finds next position to emplace the
 * track piece based on the last curve */
int Track::setNextCoord(int &row,
                         int &col,
                         trackPartType elem,
                         trackPartType prev,
                         int lastRow) {
    bool sameRow = (row == lastRow);  // elem and prev on same row
    lastRow = row;
    if ((elem == horizontal || elem == finishH) && (prev == upRight || prev == downRight))
        col++;
    else if ((elem == horizontal || elem == finishH) && (prev == upLeft || prev == downLeft))
        col--;
    else if ((elem == vertical || elem == finishV) && (prev == upRight || prev == upLeft))
        row--;
    else if((elem == vertical || elem == finishV) && (prev == downRight || prev == downLeft))
        row++;
    else if (elem == downRight && prev == downLeft)
        row++;
    else if (elem == upRight && prev == upLeft)
        row--;
    else if (sameRow && elem == upRight && prev == downLeft)
        row--;
    else if (!sameRow && elem == upRight && prev == downLeft)
        col++;
    else if (elem == upRight && prev == downRight)
        col++;
    else if (elem == downLeft && prev == downRight)
        row++;
    else if (elem == downLeft && prev == upLeft)
        col--;
    else if (!sameRow && elem == downLeft && prev == upRight)
        col--;
    else if (sameRow && elem == downLeft && prev == upRight)
        row++;
    else if (elem == upLeft && prev == upRight)
        row--;
    else if (elem == upLeft && prev == downLeft)
        col--;
    else if (!sameRow && elem == upLeft && prev == downRight)
        col--;
    else if (sameRow && elem == upLeft && prev == downRight)
        row--;
    else if (elem == upLeft && prev == downLeft)
        col--;
    else if (elem == downRight && prev == upRight)
        col++;
    else if (!sameRow && elem == downRight && prev == upLeft)
        col++;
    else if (sameRow && elem == downRight && prev == upLeft)
        row++;
    return lastRow;
}

// fills whole track as empty (grass)
void Track::initLayout() {
    TrackPartData emptyPart;
    partCounter = 0;
    trackPartData.clear();
    trackPartData.reserve(width * height);
    for (int i = 0; i < width * height; i++) {
        emptyPart.loadPos(height - i/width, i%width);
        emptyPart.setID(-1);
        trackPartData.emplace_back(emptyPart);
    }
}

/* Transforms positions in meters to indexes of matrix.
 *-1 to transform to display coordinates. See loadPos().*/
TrackPartData & Track::getTrackPart(int posX, int posY) {
    int row = posToIndex((height-1)*BLOCKSIZE - findNearestPos(posY));
    int col = posToIndex(findNearestPos(posX));
    return trackPartData[row*width + col];
}

void Track::setTrackPartType(int row, int col, trackPartType type) {
    trackPartData[row*width + col].setType(type);
}

/* Finds nearest bottom-left corner in BLOCKSIZE multiples [meters]
 * for example: pos==234 => returns: 200*/
int Track::findNearestPos(int pos) {
    return (pos/BLOCKSIZE)*BLOCKSIZE;
}

// Transforms from meters to matrix display indexes
int Track::posToIndex(int pos) {
    return pos/BLOCKSIZE;
}

// Transforms data structure of whole track into a string
std::string Track::serialize() {
    std::string msg;
    msg = std::to_string(startCol) +
          "," +
          std::to_string(startRow) +
          "," +
          std::to_string(width) +
          "," +
          std::to_string(height) +
          "," +
          std::to_string(partCounter) +
          "," +
          name;
    for (auto & it : trackPartData) {
        msg += "," + it.serialize();
    }
    msg += "\n";
    return msg;
}

// Constructor: Creates track based on serialized string
Track::Track(const std::string & trackStr) {
    size_t pos = 0;

    startCol = std::stoi(parseTrackParam(trackStr, pos, ','));
    startRow = std::stoi(parseTrackParam(trackStr, pos, ','));
    width = std::stoi(parseTrackParam(trackStr, pos, ','));
    height = std::stoi(parseTrackParam(trackStr, pos, ','));
    partCounter = std::stoi(parseTrackParam(trackStr, pos, ','));
    name = parseTrackParam(trackStr, pos, ',');
    parseTrackParts(trackStr, pos);
}

// Parses track parts in string to create a track (posx, posy, type)
void Track::parseTrackParts(const std::string & trackStr, size_t & pos) {
    int x;
    int y;
    trackPartType type;
    TrackPartData part;
    for (int i=1; i <= width*height; i++){
        x = std::stoi(parseTrackParam(trackStr, pos, ','));
        y = std::stoi(parseTrackParam(trackStr, pos, ','));
        type = static_cast<trackPartType>(std::stoi(parseTrackParam(trackStr,
                                                                     pos,
                                                                     ',')));
        part.loadPosClient(x,y);
        part.loadType(type);
        trackPartData.emplace_back(part);
    }
}

// Parses initial non-parts attributes of track
std::string Track::parseTrackParam(const std::string & initString,
                                   size_t & pos,
                                   const char delim) {
    std::string substr;

    size_t nextPos = initString.find(delim, pos);
    size_t len = nextPos - pos;
    substr = initString.substr(pos, len);
    pos = nextPos + 1;
    return substr;
}

std::vector<TrackPartData> Track::getTrackPartData() const {
    return trackPartData;
}

Point Track::getCarStartingPos(int order) {
    int blocksToStartLine = order/STARTCARSPERBLOCK;
    Point blockPoint = trackSequence.at(partCounter - 1 - blocksToStartLine);
    trackPartType t = getTrackPart(blockPoint.getX(), blockPoint.getY()).getType();
    if (order % 2 == 0 && (t == finishH || t == horizontal))
        return Point(blockPoint.getX() + BLOCKSIZE/2, blockPoint.getY() + BLOCKSIZE*7/20);
    else if (order % 2 == 1 && (t == finishH || t == horizontal))
        return Point(blockPoint.getX() + BLOCKSIZE/2, blockPoint.getY() + BLOCKSIZE*13/20);
    else if (order % 2 == 0 && (t == finishV || t == vertical))
        return Point(blockPoint.getX() + BLOCKSIZE*7/20, blockPoint.getY() + BLOCKSIZE/2);
    else if (order % 2 == 1 && (t == finishV || t == vertical))
        return Point(blockPoint.getX() + BLOCKSIZE*13/20, blockPoint.getY() + BLOCKSIZE/2);

    else if (order % 2 == 0 && (t == upLeft))
        return Point(blockPoint.getX() + BLOCKSIZE*1/4, blockPoint.getY() + BLOCKSIZE*3/4);
    else if (order % 2 == 0 && (t == downRight))
        return Point(blockPoint.getX() + BLOCKSIZE*1/2, blockPoint.getY() + BLOCKSIZE*1/2);
    else if (order % 2 == 1 && (t == upLeft))
        return Point(blockPoint.getX() + BLOCKSIZE*1/2, blockPoint.getY() + BLOCKSIZE*1/2);
    else if (order % 2 == 1 && (t == downRight))
        return Point(blockPoint.getX() + BLOCKSIZE*3/4, blockPoint.getY() + BLOCKSIZE*1/4);

    else if (order % 2 == 0 && (t == downLeft))
        return Point(blockPoint.getX() + BLOCKSIZE*1/4, blockPoint.getY() + BLOCKSIZE*1/4);
    else if (order % 2 == 0 && (t == upRight))
        return Point(blockPoint.getX() + BLOCKSIZE*1/2, blockPoint.getY() + BLOCKSIZE*1/2);
    else if (order % 2 == 0 && (t == downLeft))
        return Point(blockPoint.getX() + BLOCKSIZE*1/2, blockPoint.getY() + BLOCKSIZE*1/2);
    else if (order % 2 == 0 && (t == upRight))
        return Point(blockPoint.getX() + BLOCKSIZE*3/4, blockPoint.getY() + BLOCKSIZE*3/4);
}

uint16_t Track::getCarStartingRotation(int order) {
    Point act = trackSequence.at(partCounter - 1 - order/STARTCARSPERBLOCK);
    Point prev = trackSequence.at(partCounter - 2 - order/STARTCARSPERBLOCK);
    trackPartType t = getTrackPart(act.getX(), act.getY()).getType();

    if (act.getX() > prev.getX()) {
        if (t == upLeft) return 45;
        else if (t == downLeft) return 135;
        else if (t == horizontal || t == finishH) return 90;
    } else if (act.getX() < prev.getX()) {
        if (t == upRight) return -45;
        else if (t == downRight) return -135;
        else if (t == horizontal || t == finishH) return -90;
    } else if (act.getY() > prev.getY()) {
        if (t == downLeft) return -45;
        else if (t == downRight) return 45;
        else if (t == vertical || t == finishV) return 0;
    } else if (act.getY() < prev.getY()) {
        if (t == upRight) return 135;
        else if (t == upLeft) return -135;
        else if (t == vertical || t == finishV) return 180;
    }
}

/* Sets start line and next position to start line */
void Track::setTrackStart(int row, int col, int rowN, int colN) {
    startRow = row;
    startCol = col;
    nextToStartRow = rowN;
    nextToStartCol = colN;
}

bool Track::jumpedTrackPart(int xCar, int yCar, int lastTrackPartID) {
    TrackPartData part = getTrackPart(findNearestPos(xCar), findNearestPos(yCar));
    uint16_t x = 0, y = 0;
    int currentID = part.getID();
    if (!isTrackPart(part.getType()))
        return false;  // is outside of track
    else if (currentID == lastTrackPartID)
        return false;  // is on same block
    else if (lastTrackPartID == partCounter - 1 && (currentID == 0 || currentID == 1))
        return false;
    else return currentID > lastTrackPartID + JUMPEDBLOCKS;
        /*if (lastTrackPartID == partCounter - 1) {
        // finish line
        x = trackSequence.at(0).getX();
        y = trackSequence.at(0).getY();
    } else {
        // any other
        x = trackSequence.at(lastTrackPartID + 1).getX();
        y = trackSequence.at(lastTrackPartID + 1).getY();
    }
    return !(xCar >= x && xCar <= x + BLOCKSIZE && yCar >= y && yCar <= y + BLOCKSIZE);*/
}

Point Track::getTrackPartPoint(int trackID) {
    return trackSequence.at(trackID);
}

// Gets car ID based on starting position
int Track::getStartingID(int order) {
    return partCounter - 1 - order/STARTCARSPERBLOCK;
}

int Track::getCurrentID(int posX, int posY) {
    int x = findNearestPos(posX);
    int y = findNearestPos(posY);
    std::cout << x << "," << y << " ";
    for(auto & it : trackSequence) {
        if (it.second.getX() == x && it.second.getY() == y)
            return it.first;
    }
}

int Track::getPartsNumber() {
    return partCounter;
}