//
// Created by fperezboco on 27/10/19.
//

#ifndef MAP_TRACK_H
#define MAP_TRACK_H


#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include "Grandstand.h"
#include "TrackPartData.h"
#include "../include/TrackList.h"
#include "../../server/include/model/point.h"


#define BLOCKSIZE 100  // meters
#define MAXTRACKSIZE 150000 // meters (per axis)
#define JUMPEDBLOCKS 2
#define STARTCARSPERBLOCK 2

#ifndef MAP_TRACKTYPE

#define MAP_TRACKTYPE
typedef enum {
    empty,
    downRight,
    downLeft,
    upRight,
    upLeft,
    horizontal,
    vertical,
    finishH,
    finishV,
    public1Up,
    public1Down,
    public1Left,
    public1Right
} trackPartType;
#endif

class TrackPartData;
class Point;

class Track {
protected:
    int startCol = 0;
    int startRow = 0;
    int nextToStartCol = 0;
    int nextToStartRow = 0;
    int width = 0;  // blocks
    int height = 0;  // blocks
    int partCounter = 0;  // number of track elements
    std::vector<TrackPartData> trackPartData{};
    std::vector<Grandstand> grandstands;
    std::map<int, Point> trackSequence;
    std::string name{};

public:
    Track(int width, int height, const std::string &name);
    Track();
    void setTrackStart(int row, int col, int rowN, int colN);
    explicit Track(const std::string &trackStr);
    void loadTrack(const Json::Value &fileTracks, int trackNumber);
    void print();
    void initLayout();
    void loadPart(int row, int col, trackPartType elem);
    void initTrackParts(const std::vector<std::string> & trackLayout);
    static trackPartType identifyElem(const std::string &layoutElem);
    std::string getName();
    trackPartType getPartType(int row, int col);
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
    std::vector<TrackPartData> getTrackPartData() const;
    bool validateTrack();
    bool validateConnection(trackPartType prev, trackPartType actual);
    std::string typeToFileType(int row, int col);
    int setNextCoord(int &row, int &col, trackPartType elem, trackPartType prev, int lastRow);
    trackPartType setStartingPreviousTrackPart(int row, int col);
    static bool isTrackPart(trackPartType type);
    static bool isTrackLinePart(trackPartType type);
    bool isTrackPart(int row, int col);

    void setTrackPartType(int row, int col, trackPartType type);

    void loadGrandstands(const Json::Value &fileTracks, int trackNumber);

    Point getCarStartingPos(int order);

    void saveTrackSequence(int row, int col);

    uint16_t getCarStartingRotation(int order);

    bool jumpedTrackPart(int posX, int posY, int lastTrackPartID);

    Point getTrackPartPoint(int trackID);

    int getStartingID(int order);

    int getCurrentID(int posX, int posY);

    int getPartsNumber();
};


#endif //MAP_TRACK_H
