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
} trackPartType;

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
    explicit Track(const std::string &trackStr);

    void loadTrack(const Json::Value &fileTracks, int trackNumber);
    void print();
    void initLayout();
    void loadPart(int row, int col, trackPartType elem);
    void initTrackParts(const std::vector<std::string> & trackLayout);
    static trackPartType identifyElem(const std::string &layoutElem);
    std::string getName();
    trackPartType getPartType(int row, int col);
    static void setNextCoord(int &row, int &col, trackPartType elem, trackPartType prev);
    static bool isCurve(const trackPartType & elem);
    static void printElem(const TrackPartData &part);
    TrackPartData & getTrackPart(int posX, int posY);
    static int findNearestPos(int pos);
    static int posToIndex(int pos);
    bool isOnTrack(int posX, int posY);
    static bool inCurveRange(bool invertedX, bool invertedY, int x, int y);

    std::string serialize();

    static std::string parseTrackParam(const std::string &initString, size_t &pos, const char delim);

    void parseTrackParts(const std::string &trackStr, size_t &pos);
};

#endif //MAP_TRACK_H
