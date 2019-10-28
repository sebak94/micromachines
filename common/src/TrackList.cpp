//
// Created by fperezboco on 27/10/19.
//

#include "TrackList.h"

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

void TrackList::printTrack(const std::string & name) {
    tracks[name].print();
}

Track & TrackList::getTrack(const std::string & name) {
    return tracks[name];
}