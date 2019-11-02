//
// Created by fperezboco on 27/10/19.
//

#ifndef MAP_TRACK_H
#define MAP_TRACK_H


#include <string>
#include <vector>
#include <jsoncpp/json/json.h>

#define BLOCKSIZE 100  // meters
#define MAXTRACKSIZE 150000 // meters (per axis)

class TrackPartData;

typedef enum {
    empty,
    downRight,
    downLeft,
    upRight,
    upLeft,
    horizontal,
    vertical
} trackElem;

class Track {
private:
    int startCol = 0;
    int startRow = 0;
    int width = 0;  // blocks
    int height = 0;  // blocks
    int partCounter = 0;  // number of track elements
    std::vector<std::string> trackLayout{};
    std::vector<TrackPartData> trackPartData{};
    std::string name{};

public:
    void loadTrack(const Json::Value &fileTracks, int trackNumber);
    void print();
    void initLayout();
    void loadElem(int row, int col, trackElem elem);
    void configure();
    static trackElem identifyElem(const std::string &layoutElem);
    std::string getName();
    trackElem getElemType(int row, int col);
    static void setNextCoord(int &row, int &col, trackElem elem, trackElem prev);
    static bool isCurve(const trackElem & elem);
    static void printElem(const TrackPartData &part);

    TrackPartData & getTrackPart(int posX, int posY);

    static int findNearestPos(int pos);

    static int posToIndex(int pos);

    bool isOnTrack(int row, int col);

    int toRadius(int posX, int posY);

    bool inCurveRange(bool invertedX, bool invertedY, int x, int y);
};

#endif //MAP_TRACK_H
