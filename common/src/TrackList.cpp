//
// Created by fperezboco on 27/10/19.
//

#include "../include/TrackList.h"

// loads all tracks in a list
void TrackList::readTracks() {
    Track track;
    Json::Value maps;
    Json::Reader reader;
    std::ifstream tracksFile(TRACKS_FILE_PATH, std::ifstream::binary);
    if (!reader.parse(tracksFile, maps )){
        std::cout << reader.getFormattedErrorMessages() << std::endl;
    } else {
        for (int i = 0; i < maps[TRACKS_ID].size(); i++) {
            track.loadTrack(maps, i);
            tracks[track.getName()] = track;
        }
    }
    tracksFile.close();
}

// Prints sketch of track named <name> into terminal
void TrackList::printTrack(const std::string & name) {
    tracks[name].print();
}

// Returns data of track named <name>
Track & TrackList::getTrack(const std::string & name) {
    return tracks[name];
}

/* Gets all track names from Json file */
std::vector<std::string> TrackList::getTrackNames() {
    std::vector<std::string> nameList{};
    Track track;
    Json::Value maps;
    Json::Reader reader;
    std::ifstream tracksFile(TRACKS_FILE_PATH, std::ifstream::binary);
    if (!reader.parse(tracksFile, maps )){
        std::cout << reader.getFormattedErrorMessages() << std::endl;
    } else {
        for (int i = 0; i < maps[TRACKS_ID].size(); i++) {
             nameList.push_back(maps[TRACKS_ID][i][NAME_ID].asString());
        }
    }
    tracksFile.close();
    return nameList;
}

Track::Track() = default;