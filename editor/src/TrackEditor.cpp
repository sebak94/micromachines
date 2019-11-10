//
// Created by fperezboco on 3/11/19.
//

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <zconf.h>
#include "../../common/include/Prompt.h"

/* Runs track editor from configuration to end */
void TrackEditor::run(Window & editor) {
    editor.startGUI(WINDOW_NAME);
    quit = false;
    //inputTrackCharacteristics(editor);
    /*testing*/
    trackName = "test28";
    trackWidth = 3;
    trackHeight = 3;
    TrackList tracklist;
    tracklist.readTracks();
    Track test = tracklist.getTrack("test28");
    test.print();
    /*endtesting*/
    grid = TrackGrid(editor, trackWidth, trackHeight);
    backgroundGrid = TrackGrid(editor, trackWidth, trackHeight);
    grid.createSamples();
    createButtons(editor);
    createWayArrow(editor);
    editTrack(editor);
}

/* Creates screen with grid and samples for track edition */
void TrackEditor::editTrack(Window & editor){
    std::thread drawer = std::thread(&TrackEditor::drawTh, this, std::ref(editor));
    while (!quit){
        while ( SDL_PollEvent( &event ) ) {
            if ( event.type == SDL_QUIT ) {
                quit = true;
            }
            updateGridEvents();
            grid.updateSamples(&event);
            grid.applyAllSamplesToGrid();
            saveButton.updateEvent(&event);
            wayButton.updateEvent(&event);
            exitButton.updateEvent(&event);
        }
        processButtonClicks();
    }
    drawer.join();
}

/* Draws editor in separated thread. FPS control. */
void TrackEditor::drawTh(Window & editor){
    while (!quit) {
        auto start = std::chrono::system_clock::now();
        editor.clearScreen();
        editor.fillBackground(255, 255, 255, 0);
        backgroundGrid.draw(editor.renderer);
        grid.draw(editor.renderer);
        saveButton.draw(editor.renderer);
        wayButton.draw(editor.renderer);
        exitButton.draw(editor.renderer);
        drawWayArrow(editor.renderer);
        SDL_RenderPresent(editor.renderer);
        auto end = std::chrono::system_clock::now();
        int microsecsPassed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        usleep(1000000 * 1 / FPS - microsecsPassed);
    }
}

/* Updates based un clicks and buttons */
void TrackEditor::processButtonClicks() {
    if (saveButton.isClicked())
        saveTrack();
    if (wayButton.isClicked() && findStartLine())
        setWay(grid.getType(startRow, startCol));
    if (exitButton.isClicked())
        quit = true;
}

/* Sets track way */
void TrackEditor::setWay(trackPartType type) {
    if (!waySet && type == finishH) {
        nextToStartCol = startCol + 1;
        nextToStartRow = startRow;
        waySet = true;
    } else if (!waySet && type == finishV) {
        nextToStartCol = startCol;
        nextToStartRow = startRow + 1;
        waySet = true;
    } else if (type == finishH) {
        if (nextToStartCol > startCol) nextToStartCol -= 2;
        else nextToStartCol +=2;
    } else if (type == finishV) {
        if (nextToStartRow > startRow) nextToStartRow -= 2;
        else nextToStartRow +=2;
    }
}

/* Creates finish line arrow */
void TrackEditor::createWayArrow(Window & editor) {
    int w = ARROW_SIZE, h = 2 * ARROW_SIZE, x = 0, y = 0;
    SDL_Rect arrowPos = {x, y, w, h};
    wayArrow = Button(editor.renderer, arrowPos, WAY_ARROW_PATH);
}

/* Draws finish line arrow depending on start block */
void TrackEditor::drawWayArrow(SDL_Renderer * renderer) {
    int w = ARROW_SIZE, h = 2 * ARROW_SIZE;
    int x = grid.getPixelPosX(startCol) - w / 2;
    int y = grid.getPixelPosY(startRow) - h / 2;
    double angle = 0;
    wayArrow.updatePos(x, y);
    if (startCol > nextToStartCol)
        angle = 270;
    else if (startRow < nextToStartRow)
        angle = 180;
    else if (startCol < nextToStartCol)
        angle = 90;
    else if (startRow > nextToStartRow)
        angle = 0;
    if(waySet)
        wayArrow.drawRotated(renderer, angle);
}

/* Sends track to Json file if track is OK */
void TrackEditor::saveTrack() {
        createTrack();
        if (findStartLine() && trackValid()){
            std::cout << "Track saved." << std::endl;
            trackToFileLayout();
            updateTrackFile();
        }
}

/* Looks for start line. Returns false if not present or duplicated */
bool TrackEditor::findStartLine() {
    int oldCol = startCol, oldRow = startRow;
    bool found = grid.findStartLine(startCol, startRow);
    if (found && (oldCol != startCol || oldRow != startRow))
        waySet = false;
    return found;
}

/* Creates string vector with grid data for Json file.
 * Previous must be a curve. */
void TrackEditor::trackToFileLayout() {
    fileLayout.clear();
    trackPartType actual;
    int row = startRow, lastRow = row, col = startCol;

    trackPartType previousCurve = setStartingPreviousTrackPart(row, col);
    fileLayout.emplace_back(track.typeToFileType(row, col));
    row = nextToStartRow, col = nextToStartCol;
    for (int j = 1; j < parts; j++){
        actual = track.getPartType(row, col);
        fileLayout.emplace_back(track.typeToFileType(row, col));
        lastRow = track.setNextCoord(row, col, actual, previousCurve, lastRow);
        if (Track::isCurve(actual)) previousCurve = actual;
    }
}

/* Auxiliar function to start traversing the track */
trackPartType TrackEditor::setStartingPreviousTrackPart(int row, int col) {
    int t = track.getPartType(row, col);
    if (t == finishV && startRow < nextToStartRow) return downLeft;
    if (t == finishV && startRow > nextToStartRow) return upLeft;
    if (t == finishH && startCol < nextToStartCol) return upRight;
    if (t == finishH && startCol > nextToStartRow) return upLeft;
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
    newTrack[START_ID][0] = startRow;
    newTrack[START_ID][1] = startCol;
    newTrack[NEXT_TO_START_ID][0] = nextToStartRow;
    newTrack[NEXT_TO_START_ID][1] = nextToStartCol;
    for (int i = 0; i < grandstands.size(); i++){
        newTrack[GRANDSTANDS_ID][i][0] = grandstands[i].getTypeAsString();
        newTrack[GRANDSTANDS_ID][i][1] = grandstands[i].getRow();
        newTrack[GRANDSTANDS_ID][i][2] = grandstands[i].getCol();
    }
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
    parts = 0; grandstands.clear();
    track = Track(trackWidth, trackHeight, trackName);
    int row = 0, col = 0;
    for (int i = 0; i < grid.getSize(); i++) {
        col = i % trackWidth, row = i / trackWidth;
        track.loadPart(row, col, grid.getType(i));
        if (track.isTrackPart(row, col))
            parts++;
        else if (Grandstand::isPublic(grid.getType(i)))
            grandstands.emplace_back(Grandstand(grid.getType(i),
                                     row,
                                     col,
                                     BLOCKSIZE,
                                     trackHeight));
    }
}

/* Updates grid textures and attributes. Checks if finish still present. */
void TrackEditor::updateGridEvents() {
    trackPartType t;
    for (int i=0; i < grid.getSize(); i++){
        grid.updateTrackBlockEvent(&event, i);
        backgroundGrid.updateTrackBlockEvent(&event, i);
    }
    t = grid.getType(startRow, startCol);
    if (t != finishH && t != finishV)
        waySet = false;
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
void TrackEditor::createButtons(Window & game) {
    int w = EDITOR_BUTTONS_WIDTH;
    int h = EDITOR_BUTTONS_HEIGHT;
    int x = (WINDOW_W - w)/(BUTTONS + 1);
    int y = WINDOW_H - (grid.getGridMarginHeight() + h)/2;
    SDL_Rect buttonPos = {x, y, w, h};
    saveButton = Button(game.renderer, buttonPos, SAVE_BUTTON_PATH);
    buttonPos.x = 2 * x;
    wayButton = Button(game.renderer, buttonPos, WAY_BUTTON_PATH);
    buttonPos.x = 3 * x;
    exitButton = Button(game.renderer, buttonPos, EXIT_BUTTON_PATH);
}