//
// Created by fperezboco on 3/11/19.
//

#ifndef MAP_TRACKEDITOR_H
#define MAP_TRACKEDITOR_H

#include <vector>
#include <SDL2/SDL_events.h>
#include "Window.h"
#include "TrackGrid.h"
#include "../../common/include/Button.h"

#define SAVE_BUTTON_WIDTH 150
#define SAVE_BUTTON_HEIGHT 65

class Prompt;

class TrackEditor {
protected:
    SDL_Event event{};
    bool quit = false;
    std::string trackName{};
    int trackWidth = 0;
    int trackHeight = 0;
    TrackGrid grid;
    TrackGrid backgroundGrid;
    Button saveButton;
    Track track;
    std::vector<std::string> fileLayout;
    int firstCornerIndex = 0;
    int parts = 0;

public:
    void run(Window &editor);
    virtual void inputTrackCharacteristics(Window &game);
    void updateGridEvents();
    void createSaveButton(Window &game);
    void editTrack(Window &editor);
    void saveTrack();
    void createTrack();
    void updateTrackFile();
    bool trackValid();
    void trackToFileLayout();

    bool createJsonTrack(Json::Value &newTrack);

    bool writeJsonValue(const Json::Value &newTrack);

    bool appendJsonValue(const Json::Value &newTrack, Json::Value &tracks);
};


#endif //MAP_TRACKEDITOR_H
