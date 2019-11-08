//
// Created by fperezboco on 27/10/19.
//

#include <jsoncpp/json/json.h>
#include <cmath>
#include "../include/TrackPartData.h"
#include "../include/Track.h"
#include "../include/TrackList.h"

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
    std::vector<std::string> fileLayout{}; // loads list of elements as temporal strings
    for (const auto & j : fileTracks[TRACKS_ID][trackNumber][LAYOUT_ID])
        fileLayout.emplace_back(j.asString());
    width = fileTracks[TRACKS_ID][trackNumber][WIDTH_ID].asInt();  // track width in blocks
    height = fileTracks[TRACKS_ID][trackNumber][HEIGHT_ID].asInt();  // track width in blocks
    name = fileTracks[TRACKS_ID][trackNumber][NAME_ID].asString();
    startRow = fileTracks[TRACKS_ID][trackNumber][START_ID][0].asInt();  // row
    startCol = fileTracks[TRACKS_ID][trackNumber][START_ID][1].asInt();  // col
    initLayout();  // fills all blocks as empty (grass)
    initTrackParts(fileLayout);  // sets not empty blocks in matrix order
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
void Track::loadPart(int row, int col, trackPartType elem) {
    TrackPartData part;
    part.loadType(elem);
    part.loadPos(height - row, col);
    part.setID(partCounter);
    trackPartData[row*width + col] = part;
    if (elem!=empty) partCounter++;
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
    if (!isCurve(type)) {
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
    int row = startRow;
    int lastRow = row;
    int col = startCol;
    loadPart(row, col, identifyElem(trackLayout[0]));
    trackPartType prevElem = getPartType(row, col);
    trackPartType actualElem;
    col++;  // assumes top-left corner is a down-right curve
    for (int i=1; i < trackLayout.size(); i++){
        actualElem = identifyElem(trackLayout[i]);
        loadPart(row, col, actualElem);
        lastRow = setNextCoord(row, col, actualElem, prevElem, lastRow);
        if (isCurve(actualElem))
            prevElem = actualElem;
    }
}

// Checks if track parts are properly connected. Assumes first is downRight
bool Track::validateTrack() {
    int row = startRow;
    int lastRow = row;
    int col = startCol;
    int i = 0;
    trackPartType actual;
    trackPartType previous = getPartType(row, col);
    if (previous != downRight)
        return false;
    col++;  // assumes top-left corner is a down-right curve
    // +1 to form a closed loop with the track
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

/* Finds first downRight corner for file processing*/
int Track::findFirstCorner() {
    int f = -1;
    for (int i = 0; i < trackPartData.size() && f == -1; i++){
        if (trackPartData[i].getType() == downRight)
            f = i;
    }
    startRow = f / width;
    startCol = f % width;
    return f;
}

// Checks if "actual" Part is properly connected based on last curve "prev"
bool Track::validateConnection(trackPartType prev, trackPartType actual) {
    if (actual == horizontal && (prev == upRight || prev == downRight))
        return true;
    else if (actual == horizontal && (prev == upLeft || prev == downLeft))
        return true;
    else if (actual == vertical && (prev == upRight || prev == upLeft))
        return true;
    else if(actual == vertical && (prev == downRight || prev == downLeft))
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

// Checks if track part is a curve
bool Track::isCurve(const trackPartType & elem) {
    return !(elem == horizontal || elem == vertical);
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
        default:
            return LAYOUT_E;
    }
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
    int row = 0, col = 0;
    partCounter = 0;
    trackPartData.clear();
    trackPartData.reserve(width * height);
    for (int i = 0; i < width * height; i++) {
        emptyPart.loadPos(height - i/width, i%width);
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

// Finds nearest bottom-left corner in BLOCKSIZE multiples [meters]
int Track::findNearestPos(int pos) {
    for(int nearest = 0; nearest < MAXTRACKSIZE; nearest+=BLOCKSIZE ) {
        if(pos >= nearest && pos < nearest + BLOCKSIZE)
            return nearest;
    }
    return -1;  // not found
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
