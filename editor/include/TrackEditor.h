//
// Created by fperezboco on 3/11/19.
//

#ifndef MAP_TRACKEDITOR_H
#define MAP_TRACKEDITOR_H

#include <vector>
#include <SDL2/SDL_events.h>
#include "TrackGrid.h"
#include "../../common/include/Button.h"
#include "../../common/include/thread.h"
#include "../../common/include/Grandstand.h"

#define WAY_ARROW_PATH "../editor/images/wayArrow.png"

#define EDITOR_BUTTONS_WIDTH 150
#define EDITOR_BUTTONS_HEIGHT 65
#define BUTTONS 3
#define FPS 60
#define ARROW_SIZE 50

class Prompt;

class TrackEditor {
protected:
    SDL_Event event{};
    std::string trackName{};
    bool quit = false;
    int trackWidth = 0;
    int trackHeight = 0;
    TrackGrid grid;
    TrackGrid backgroundGrid;
    Button saveButton;
    Button wayButton;
    Button wayArrow;
    Button exitButton;
    Track track;
    std::vector<std::string> fileLayout;
    std::vector<Grandstand> grandstands;
    int firstCornerIndex = 0;
    int parts = 0;
    int startCol = 0;
    int startRow = 0;
    int nextToStartCol = 0;
    int nextToStartRow = 0;
    bool waySet = false;

public:
    void run(Window &editor);
    virtual void inputTrackCharacteristics(Window &game);
    void updateGridEvents();
    void processButtonClicks();
    void createButtons(Window &game);
    void createWayArrow(Window &game);
    void editTrack(Window &editor);
    void saveTrack();
    void createTrack();
    void updateTrackFile();
    bool trackValid();
    void trackToFileLayout();
    bool createJsonTrack(Json::Value &newTrack);
    bool writeJsonValue(const Json::Value &newTrack);
    bool appendJsonValue(const Json::Value &newTrack, Json::Value &tracks);
    void drawTh(Window &editor);
    bool findStartLine();
    void setWay(trackPartType type);
    void drawWayArrow(SDL_Renderer *renderer);
    trackPartType setStartingPreviousTrackPart(int row, int col);
};


#endif //MAP_TRACKEDITOR_H
