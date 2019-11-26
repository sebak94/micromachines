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
    int startCol = 0;  // start line
    int startRow = 0;  // start line
    int nextToStartCol = 0;  // next to start line
    int nextToStartRow = 0;  // next to start line
    int width;  // blocks
    int height;  // blocks
    int partCounter = 0;  // number of track elements
    std::vector<TrackPartData> trackPartData{};  // data of blocks
    std::vector<Grandstand> grandstands;  // public
    std::map<int, Point> trackSequence;  // stores race blocks positions ordered
    std::string name;  // of track

public:
    Track(int width, int height, const std::string &name);
    Track();
    explicit Track(const std::string &trackStr);
    void loadGrandstands(const Json::Value &fileTracks, int trackNumber);
    void initTrackParts(const std::vector<std::string> & trackLayout);
    void parseTrackParts(const std::string &trackStr, size_t &pos);
    void loadTrack(const Json::Value &fileTracks, int trackNumber);
    void setTrackPartType(int row, int col, trackPartType type);
    void setTrackStart(int row, int col, int rowN, int colN);
    void loadPart(int row, int col, trackPartType elem);
    void saveTrackSequence(int row, int col);
    void initLayout();
    void print();
    bool validateConnection(trackPartType prev, trackPartType actual);
    bool jumpedTrackPart(int posX, int posY, int lastTrackPartID);
    bool isOnTrack(int posX, int posY);
    bool isTrackPart(int row, int col);
    bool hasGrandstands();
    bool validateTrack();
    int setNextCoord(int &row, int &col, trackPartType elem,
                     trackPartType prev, int lastRow);
    int getStartingID(int order);
    int getPartsNumber();
    int getTrackW();
    int getTrackH();
    static void printElem(const TrackPartData &part);
    static bool inCurveRange(bool invertedX, bool invertedY, int x, int y);
    static bool isCurve(const trackPartType & elem);
    static bool isTrackPart(trackPartType type);
    static int findNearestPos(int pos);
    static int posToIndex(int pos);
    static trackPartType identifyElem(const std::string &layoutElem);
    static std::string parseTrackParam(const std::string &initString,
                                       size_t &pos, const char delimiter);
    float getCarStartingRotation(int order);
    std::vector<TrackPartData> getTrackPartData() const;
    TrackPartData & getTrackPart(int posX, int posY);
    trackPartType getPartType(int row, int col);
    trackPartType setStartingPreviousTrackPart(int row, int col);
    std::string getName();
    std::string serialize();
    std::string typeToFileType(int row, int col);
    Grandstand getRandomGrandstand();
    Point getCarStartingPos(int order);
    Point getTrackPartPoint(int trackID);
};


#endif //MAP_TRACK_H
