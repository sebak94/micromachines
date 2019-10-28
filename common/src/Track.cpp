//
// Created by fperezboco on 27/10/19.
//

#include <jsoncpp/json/json.h>
#include "Track.h"
#include "TrackList.h"

void Track::loadTrack(const Json::Value &maps, int trackNumber) {
    trackLayout.clear();
    name.clear();
    for (int j = 0; j < maps[TRACKS_ID][trackNumber][LAYOUT_ID].size(); j++) {
        trackLayout.emplace_back(maps[TRACKS_ID][trackNumber][LAYOUT_ID][j].asString());
    }
    width = maps[TRACKS_ID][trackNumber][WIDTH_ID].asInt();
    height = maps[TRACKS_ID][trackNumber][HEIGHT_ID].asInt();
    name = maps[TRACKS_ID][trackNumber][NAME_ID].asString();
    startRow = maps[TRACKS_ID][trackNumber][START_ID][0].asInt();  // row
    startCol = maps[TRACKS_ID][trackNumber][START_ID][1].asInt();  // col
    initLayout();
    configure();
}

std::string Track::getName() {
    return name;
}

void Track::print() {
    int column = 0;
    for (int i=0; i < layout.size(); i++) {
        printElem(layout[i]);
        column++;
        if (column == width) {
            std::cout << std::endl;
            column = 0;
        }
    }
}

void Track::printElem(const trackElem & elem) {
    if (elem==empty)
        std::cout << "░";
    if (elem==downRight)
        std::cout << "╔";
    if (elem==downLeft)
        std::cout << "╗";
    if (elem==upRight)
        std::cout << "╚";
    if (elem==upLeft)
        std::cout << "╝";
    if (elem==horizontal)
        std::cout << "═";
    if (elem==vertical)
        std::cout << "║";
}

// Counts rows and cols from 0
void Track::loadElem(int row, int col, trackElem elem) {
    layout[row*width + col] = elem;
}

trackElem Track::getElem(int row, int col) {
    return layout[row*width + col];
}

void Track::configure() {
    int row = startRow;
    int col = startCol;
    loadElem(row, col, identifyElem(trackLayout[0]));
    trackElem prevElem = getElem(row, col);
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

trackElem Track::identifyElem(const std::string & layoutElem){
    if (layoutElem == LAYOUT_DL)
        return downLeft;
    if (layoutElem == LAYOUT_DR)
        return downRight;
    if (layoutElem == LAYOUT_H)
        return horizontal;
    if (layoutElem == LAYOUT_UL)
        return upLeft;
    if (layoutElem == LAYOUT_UR)
        return upRight;
    if (layoutElem == LAYOUT_V)
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

void Track::initLayout() {
    layout.clear();
    layout.reserve(width*height);
    for (int i=0; i < width*height; i++) {
        layout.emplace_back(empty);
    }
}