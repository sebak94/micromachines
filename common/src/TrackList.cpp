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
    if (!reader.parse(tracksFile, maps)){
        std::cout << reader.getFormattedErrorMessages() << std::endl;
    } else {
        for (uint32_t i = 0; i < maps[TRACKS_ID].size(); i++) {
            track.loadTrack(maps, i);
            tracks[track.getName()] = track;
        }
    }
    tracksFile.close();
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
        for (uint32_t i = 0; i < maps[TRACKS_ID].size(); i++) {
             nameList.push_back(maps[TRACKS_ID][i][NAME_ID].asString());
        }
    }
    tracksFile.close();
    return nameList;
}

Track::Track() = default;

std::string TrackList::serialize() {
    std::string trackNames{};
    for (auto & track : tracks)
        trackNames += track.first + ",";
    trackNames.back() = '\n';
    return trackNames;
}

std::vector<std::string> TrackList::getTrackNames(const std::string &str) {
    size_t pos = 0;
    std::string substr{};
    std::vector<std::string> list{};
    while( parse(str, pos, ',', substr) ) {
        std::cout << substr << std::endl;
        list.emplace_back(substr);
    }
    parse(str,pos,'\n',substr);
    std::cout << substr << std::endl;
    list.emplace_back(substr);
    return list;
}

bool TrackList::parse(const std::string &str, size_t &pos, const char delim,
                      std::string &substr) {
    substr.clear();
    size_t nextPos = str.find(delim, pos);
    if (nextPos == std::string::npos)
        return false;
    size_t len = nextPos - pos;
    substr = str.substr(pos, len);
    pos = nextPos + 1;
    return true;
}
