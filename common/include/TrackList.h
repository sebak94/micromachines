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

#define TRACKS_FILE_PATH "../tracks_layout.json"
#define LAYOUT_ID "layout"
#define TRACKS_ID "tracks"
#define NAME_ID "name"
#define WIDTH_ID "width"
#define HEIGHT_ID "height"
#define START_ID "start"
#define NEXT_TO_START_ID "next-to-start"
#define GRANDSTANDS_ID "grandstands"

#define LAYOUT_DR "down-right"
#define LAYOUT_H "horizontal"
#define LAYOUT_DL "down-left"
#define LAYOUT_V "vertical"
#define LAYOUT_UL "up-left"
#define LAYOUT_UR "up-right"
#define LAYOUT_E "empty"
#define LAYOUT_FH "finishH"
#define LAYOUT_FV "finishV"

#include "TrackPartData.h"

class Track;

class TrackList {
private:
    std::map<std::string, Track> tracks;

public:
    void readTracks();
    void printTrack(const std::string &name);
    Track& getTrack(const std::string &name);

    std::vector<std::string> getTrackNames();
};

#endif //MAP_TRACKLIST_H
