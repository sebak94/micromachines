//
// Created by fperezboco on 27/10/19.
//

#ifndef MAP_TRACK_H
#define MAP_TRACK_H


#include <string>
#include <vector>

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
    int width = 0;
    int height = 0;
    int startCol = 0;
    int startRow = 0;
    std::vector<std::string> trackLayout{};
    std::vector<trackElem> layout{};
    std::string name{};

public:
    void loadTrack(const Json::Value &maps, int trackNumber);
    std::string getName();
    void print();
    void initLayout();
    void loadElem(int row, int col, trackElem elem);
    static trackElem identifyElem(const std::string &layoutElem);
    void configure();
    trackElem getElem(int row, int col);
    static void setNextCoord(int &row, int &col, trackElem elem, trackElem prev);
    static bool isCurve(const trackElem & elem);
    void printElem(const trackElem &elem);
};


#endif //MAP_TRACK_H
