//
// Created by fperezboco on 3/11/19.
//

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_ttf.h>
#include "../../common/include/Prompt.h"
#include "../include/TrackEditor.h"
#include "../include/TrackGrid.h"

/* Runs track editor from configuration to end */
void TrackEditor::run(Window & editor) {
    editor.startGUI(WINDOW_NAME);
    quit = false;
    inputTrackCharacteristics(editor);
    grid = TrackGrid(editor, trackWidth, trackHeight);
    backgroundGrid = TrackGrid(editor, trackWidth, trackHeight);
    grid.createSamples();
    createSaveButton(editor);
    editTrack(editor);
}

/* Creates screen with grid and samples for track edition */
void TrackEditor::editTrack(Window & editor){
    while (!quit){
        while ( SDL_PollEvent( &event ) ) {
            if ( event.type == SDL_QUIT ) {
                quit = true;
            }
            editor.clearScreen();
            editor.fillBackground(255, 255, 255, 0);
            backgroundGrid.draw(editor.renderer);
            updateGridEvents();
            grid.updateSamples(&event);
            grid.applyAllSamplesToGrid();
            grid.draw(editor.renderer);
            saveButton.updateEvent(&event);
            saveButton.draw(editor.renderer);
            SDL_RenderPresent(editor.renderer);
        }
        saveTrack();
    }
}

/* Sends track to Json file if track is OK */
void TrackEditor::saveTrack() {
    if (saveButton.isClicked()){
        createTrack();
        if (trackValid()){
            trackToFileLayout();
            updateTrackFile();
        }
    }
}

/* Creates string vector with grid data for Json file */
void TrackEditor::trackToFileLayout() {
    fileLayout.clear();
    int row = firstCornerIndex / trackWidth;
    int col = firstCornerIndex % trackWidth;
    int i = 0;
    trackPartType actual;
    trackPartType previous = track.getPartType(row, col);
    fileLayout.emplace_back(track.typeToFileType(row, col));
    col++;  // assumes top-left corner is a down-right curve
    // +1 to form a closed loop with the track
    for (int j = 1; j < parts; j++){
        actual = track.getPartType(row, col);
        fileLayout.emplace_back(track.typeToFileType(row, col));
        track.setNextCoord(row, col, actual, previous, 0);
        if (track.isCurve(actual))
            previous = actual;
    }
}

/* Creates Json structure for new track and writes file */
void TrackEditor::updateTrackFile() {
    Json::Value newTrack;
    createJsonTrack(newTrack);
    writeJsonValue(newTrack);
}

/* Creates Json track structure*/
bool TrackEditor::createJsonTrack(Json::Value & newTrack) {
    newTrack[NAME_ID] = track.getName();
    newTrack[WIDTH_ID] = trackWidth;
    newTrack[HEIGHT_ID] = trackHeight;
    newTrack[START_ID][0] = firstCornerIndex / trackWidth;
    newTrack[START_ID][1] = firstCornerIndex % trackWidth;
    for (int i = 0; i < parts; i++)
        newTrack[LAYOUT_ID][i] = fileLayout[i];
}

/* Writes new value to file. Replaces track if existent */
bool TrackEditor::writeJsonValue(const Json::Value & newTrack) {
    Json::Value tracks;
    Json::Reader reader;
    std::fstream tracksFile;
    tracksFile.open(TRACKS_FILE_PATH, std::ios::in);
    if (!reader.parse(tracksFile, tracks))
        std::cout << reader.getFormattedErrorMessages() << std::endl;
    tracksFile.close();
    appendJsonValue(newTrack, tracks);
    tracksFile.open(TRACKS_FILE_PATH, std::ios::out);
    tracksFile << tracks.toStyledString() << std::endl;
    tracksFile.close();
    track.print();
}

/* Add tracks to file or replaces it if existent */
bool TrackEditor::appendJsonValue(const Json::Value & newTrack,
                                  Json::Value & tracks) {
    int trackIndex = -1; // non-existent
    Json::Value removed;
    for (int i = 0; i < tracks[TRACKS_ID].size(); i++) {
        if (tracks[TRACKS_ID][i][NAME_ID].asString() == track.getName())
            trackIndex = i;
    }
    if (trackIndex == -1){
        tracks[TRACKS_ID].append(newTrack);
    } else {
        tracks[TRACKS_ID].removeIndex(trackIndex, &removed);
        tracks[TRACKS_ID].append(newTrack);
    }
}

/* Checks if track parts are properly connected */
bool TrackEditor::trackValid() {
    return track.validateTrack();
}

/* Creates track from grid */
void TrackEditor::createTrack() {
    parts = 0;
    track = Track(trackWidth, trackHeight, trackName);
    int row = 0, col = 0;
    for (int i = 0; i < grid.getSize(); i++, col++) {
        if (col == trackWidth)
            col = 0, row++;
        track.loadPart(row, col, grid.getType(i));
        if (track.getPartType(row, col) != empty) parts++;
    }
    firstCornerIndex = track.findFirstCorner();
}

/* Updates grid textures and attributes */
void TrackEditor::updateGridEvents() {
    for (int i=0; i < grid.getSize(); i++){
        grid.updateTrackBlockEvent(&event, i);
        backgroundGrid.updateTrackBlockEvent(&event, i);
    }
}

/* Creates an interface with the user to config name, width and height */
void TrackEditor::inputTrackCharacteristics(Window & game) {
    Prompt config;  // for input name, width, height
    config.inputTrackCharacteristics(game);
    trackName = config.getTrackName();
    trackWidth = config.getWidth();
    trackHeight = config.getHeight();
}

/* Creates save button to save track */
void TrackEditor::createSaveButton(Window & game) {
    int w = SAVE_BUTTON_WIDTH;
    int h = SAVE_BUTTON_HEIGHT;
    int x = WINDOW_W/2 - w/2;
    int y = WINDOW_H - (grid.getGridMarginHeight() + h)/2;
    SDL_Rect saveButtonPos = {x, y, w, h};
    saveButton = Button(game.renderer, saveButtonPos, SAVE_BUTTON_PATH);
}