//
// Created by fperezboco on 27/10/19.
//

#ifndef MAP_TRACK_H
#define MAP_TRACK_H

#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include "TrackPartData.h"

#define BLOCKSIZE 100  // meters
#define MAXTRACKSIZE 150000 // meters (per axis)

class TrackPartData;

#ifndef MAP_TRACKTYPE
#define MAP_TRACKTYPE
typedef enum {
    empty,
    downRight,
    downLeft,
    upRight,
    upLeft,
    horizontal,
    vertical
} trackPartType;
#endif

class Track {
private:
    int startCol = 0;
    int startRow = 0;
    int width = 0;  // blocks
    int height = 0;  // blocks
    int partCounter = 0;  // number of track elements
    std::vector<TrackPartData> trackPartData{};
    std::string name{};

public:
    Track();
    Track(int width, int height, const std::string &name);
    explicit Track(const std::string &trackStr);
    void loadTrack(const Json::Value &fileTracks, int trackNumber);
    void print();
    void initLayout();
    void loadPart(int row, int col, trackPartType elem);
    void parseTrackParts(const std::string &trackStr, size_t &pos);
    void initTrackParts(const std::vector<std::string> & trackLayout);
    std::string getName();
    std::string serialize();
    trackPartType getPartType(int row, int col);
    TrackPartData & getTrackPart(int posX, int posY);
    static trackPartType identifyElem(const std::string &layoutElem);
    static std::string parseTrackParam(const std::string &initString, size_t &pos, const char delim);
    static int setNextCoord(int &row, int &col, trackPartType elem, trackPartType prev, int lastRow);
    static void printElem(const TrackPartData &part);
    static bool isCurve(const trackPartType & elem);
    static bool inCurveRange(bool invertedX, bool invertedY, int x, int y);
    static int findNearestPos(int pos);
    static int posToIndex(int pos);
    bool isOnTrack(int posX, int posY);

    bool validateTrack();

    static bool validateConnection(trackPartType prev, trackPartType actual);

    int findFirstCorner();

    std::string typeToFileType(int row, int col);

};

#endif //MAP_TRACK_H
