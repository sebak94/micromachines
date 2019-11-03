//
// Created by fperezboco on 27/10/19.
//

#ifndef MAP_TRACKLIST_H
#define MAP_TRACKLIST_H

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "Track.h"

#define TRACKS_FILE_PATH "tracks_layout.json"
#define LAYOUT_ID "layout"
#define TRACKS_ID "tracks"
#define NAME_ID "name"
#define WIDTH_ID "width"
#define HEIGHT_ID "height"
#define START_ID "start"

#define LAYOUT_DR "down-right"
#define LAYOUT_H "horizontal"
#define LAYOUT_DL "down-left"
#define LAYOUT_V "vertical"
#define LAYOUT_UL "up-left"
#define LAYOUT_UR "up-right"

#include "TrackPartData.h"

class TrackList {
private:
    std::map<std::string, Track> tracks;

public:
    void readTracks();
    void printTrack(const std::string &name);
    Track &getTrack(const std::string &name);
};

#endif //MAP_TRACKLIST_H
