//
// Created by fperezboco on 4/11/19.
//

#include "../../editor/include/TrackEditor.h"
#include "../include/Prompt.h"
#include <algorithm>
#include <SDL2/SDL_ttf.h>

/* Creates dialog for user to config new track to edit */
bool Prompt::inputTrackCharacteristics(Window & game){
    createTitle(game);
    editOrNewTrack(game);
    if (createNewTrack) {
        createButtons(game);
        inputTrackName(game);
        inputTrackWidth(game);
        inputTrackHeight(game);
        SDL_StopTextInput();
        return true;
    } else {
        createButtons(game);
        inputFileTrack(game);
        return false;
    }
}

void Prompt::updateTrackSelection(Window &window, const std::string &textToShow) {
    TTF_Init();
    promptTextColor = {255, 255, 255, 0};
    fieldTextColor = {0, 0, 0, 0};
    promptText.textToTexture(window.renderer, "Track", promptTextColor, PROMPT_FONT, FONT_SIZE);
    textField.textToTexture(window.renderer, textToShow.c_str(), fieldTextColor, FIELD_FONT, FONT_SIZE);
    setTextPosition();
    SDL_Color fieldColor = {200, 200, 200, 0};
    textField.createFieldBox(getTextFieldPosX(), getTextFieldPosY(), fieldColor, (WINDOW_W - 7*getTextFieldPosX()));
}

void Prompt::renderTrackSelection(Window &window) {
    promptText.render(window.renderer, textPosXPrompt, textPosYPrompt);
    textField.renderFieldBox(window.renderer);
    textField.render(window.renderer, textPosXField, textPosYField);
}

/* Creates title texture */
void Prompt::createTitle(Window & game) {
    int w = WINDOW_W*9/10;
    int h = w/10;
    int x = WINDOW_W/2 - w/2;
    int y = WINDOW_H*0.2;
    SDL_Rect titlePos = {x, y, w, h};
    title = Button(game.renderer, titlePos, TITLE_PATH);
}

/* Creates save button to save config */
void Prompt::createButtons(Window & game) {
    int w = EDITOR_BUTTONS_WIDTH;
    int h = EDITOR_BUTTONS_HEIGHT;
    int x = WINDOW_W/2 - w/2;
    int y = WINDOW_H*0.8;
    SDL_Rect saveButtonPos = {x, y, w, h};
    saveButton = Button(game.renderer, saveButtonPos, SAVE_BUTTON_PATH);
}

/* Draws everything once */
void Prompt::drawAll(Window & game) {
    processTextPrompt(game.renderer, renderText);
    game.clearScreen();
    game.fillBackground(0, 0, 0, 0);
    saveButton.draw(game.renderer);
    title.draw(game.renderer);
    renderTextPrompt(game.renderer);
    SDL_RenderPresent(game.renderer);
}

/* Draws everything once */
void Prompt::drawMainMenu(Window & game) {
    game.clearScreen();
    game.fillBackground(0, 0, 0, 0);
    editButton.draw(game.renderer);
    newButton.draw(game.renderer);
    title.draw(game.renderer);
    SDL_RenderPresent(game.renderer);
}

/* Draws everything once */
void Prompt::drawTrackSelection(Window & game) {
    game.clearScreen();
    game.fillBackground(0, 0, 0, 0);
    editButton.draw(game.renderer);
    nextButton.draw(game.renderer);
    title.draw(game.renderer);
    renderTrackSelection(game);
    SDL_RenderPresent(game.renderer);
}

void Prompt::editOrNewTrack(Window & game) {
    int buttonSize = 250;
    int sep = 200;
    int x1 = WINDOW_W/2 - buttonSize/2 - sep;
    int x2 = WINDOW_W/2 - buttonSize/2 + sep;
    int y = WINDOW_H*0.45;
    SDL_Rect saveButtonPos = {x1, y, buttonSize, buttonSize};
    SDL_Rect editButtonPos = {x2, y, buttonSize, buttonSize};
    newButton = Button(game.renderer, saveButtonPos, NEW_BUTTON_PATH);
    editButton = Button(game.renderer, editButtonPos, EDIT_BUTTON_PATH);
    TrackList trackList;
    std::vector<std::string> trackNames = trackList.getTrackNames();
    while (!quit && !modeAccepted) {
        getModeEvent();
        processModeEvent();
        drawMainMenu(game);
    }
}

/* Creates dialog for user to config NAME of track to edit */
void Prompt::inputTrackName(Window & game) {
    TrackList trackList;
    std::vector<std::string> trackNames = trackList.getTrackNames();
    while (!quit && !nameAccepted) {
        initTextPrompt(game.renderer, std::string(MSG_TRACK_NAME));
        getNameEvent();
        processNameEvent(trackNames);
        drawAll(game);
    }
}

void Prompt::inputFileTrack(Window & game) {
    TrackList trackList;
    trackNames = trackList.getTrackNames();
    int editButtonSize = 150;
    updateTrackSelection(game, " ");
    SDL_Rect nextButtonPos = {WINDOW_W - 6*getTextFieldPosX(), getTextFieldPosY() - 6, 160, 70};
    SDL_Rect editButtonPos = {WINDOW_W/2 - editButtonSize/2, WINDOW_H*11/16 , editButtonSize, editButtonSize};
    nextButton = Button(game.renderer, nextButtonPos, NEXT_BUTTON_PATH);
    editButton = Button(game.renderer, editButtonPos, EDIT_BUTTON_PATH);
    auto track = trackNames.begin();
    while (!quit && !nameAccepted) {
        updateTrackSelection(game, *track);
        getTrackSelectionEvent();
        processTrackSelectionEvent(track);
        drawTrackSelection(game);
    }
}

/* Creates dialog for user to config WIDTH of track to edit */
void Prompt::inputTrackWidth(Window & game) {
    while (!quit && !widthAccepted) {
        initTextPrompt(game.renderer, std::string(MSG_TRACK_WIDTH));
        getNumberEvent();
        processEventWidth();
        processTextPrompt(game.renderer, renderText);
        drawAll(game);
    }
}

/* Creates dialog for user to config HEIGHT of track to edit */
void Prompt::inputTrackHeight(Window & game) {
    while (!quit && !heightAccepted) {
        initTextPrompt(game.renderer, std::string(MSG_TRACK_HEIGHT));
        getNumberEvent();
        processEventHeight();
        processTextPrompt(game.renderer, renderText);
        drawAll(game);
    }
}

/* Checks if track exists on file */
bool Prompt::trackExists(std::vector<std::string> & trackNames, const std::string & trackName) {
    std::vector<std::string>::iterator it;
    it = std::find(trackNames.begin(), trackNames.end(), trackName);
    return it != trackNames.end();
}

/* Checks if track name is at least 1 in size, shorter than the limit and
 * that it's name is non-existent. If config is not OK, configs error
 * to display in red. If backspace is pressed, deletes error warnings.*/
void Prompt::processNameEvent(std::vector<std::string> & trackNames) {
    if (textInput && inputText.length() <= MAX_LENGTH_TRACK_NAME){
        inputText += event.text.text;
        renderText = true;
        nameError = " ";
    } else if (textInput) {
        renderWrongMessage = true;
        lengthReached = true;
        nameError = MSG_TOO_LARGE_NAME;
    }
    if (backspacePressed && inputText.length() > 1){
        inputText.pop_back();
        renderText = true;
        renderWrongMessage = false;
    }
    if ((returnPressed || savePressed) &&
    validNameSize() &&
    !trackExists(trackNames, inputText.substr(1,std::string::npos))){
        nameAccepted = true;
        std::cout << "NAME ACCEPTED: ";
        trackName = inputText.substr(1,std::string::npos);
        std::cout << trackName << std::endl;
        renderText = true;
        inputText = " ";
    } else if ((returnPressed || savePressed)){
        renderWrongMessage = true;
        nameAccepted = false;
        std::cout << "WRONG NAME" << std::endl;
        if (validNameSize())
            nameError = MSG_NAME_ALREADY_EXISTS;
        else
            nameError = MSG_TOO_SHORT_NAME;
        renderText = true;
    }
}

void Prompt::processTrackSelectionEvent(
        std::vector<std::string>::iterator &track) {
    if (editPressed) {
        nameAccepted = true;
        trackName = *track;
    } else if (nextTrackPressed) {
        track++;
        if (track == trackNames.end())
            track = trackNames.begin();
    }
}

void Prompt::processModeEvent() {
    if (newPressed){
        modeAccepted = true;
        createNewTrack = true;
    } else if (editPressed){
        modeAccepted = true;
        createNewTrack = false;
    }
}

/* Checks if track name has proper size */
bool Prompt::validNameSize() {
    return  inputText.length() <= MAX_LENGTH_TRACK_NAME &&
            inputText.length() >= MIN_LENGTH_TRACK_NAME;
}

/* Gets text input, backspace, return and save button events. */
void Prompt::getNameEvent() {
    renderText = false;
    returnPressed = false;
    textInput = false;
    backspacePressed = false;
    savePressed = false;
    while ( SDL_PollEvent( &event ) ) {
        if ( event.type == SDL_QUIT ) {
            quit = true;
        } else if ( event.type == SDL_TEXTINPUT ) {
            if(!(SDL_GetModState() & KMOD_CTRL && (isCtrlCommand())))
                textInput = true;
        } else if (event.type == SDL_KEYDOWN ) {
            checkBackspaceAndReturn();
        }
        saveButton.updateEvent(&event);
        if (saveButton.isClicked())
            savePressed = true;
    }
}

void Prompt::getModeEvent() {
    renderText = false;
    returnPressed = false;
    textInput = false;
    backspacePressed = false;
    savePressed = false;
    newPressed = false;
    editPressed = false;
    while ( SDL_PollEvent( &event ) ) {
        if ( event.type == SDL_QUIT )
            quit = true;
        newButton.updateEvent(&event);
        editButton.updateEvent(&event);
        if (newButton.isClicked())
            newPressed = true;
        if (editButton.isClicked())
            editPressed = true;
    }
}

void Prompt::getTrackSelectionEvent() {
    newPressed = false;
    nextTrackPressed = false;
    editPressed = false;
    while ( SDL_PollEvent( &event ) ) {
        if ( event.type == SDL_QUIT )
            quit = true;
        editButton.updateEvent(&event);
        nextButton.updateEvent(&event);
        if (editButton.isClicked())
            editPressed = true;
        if (nextButton.isClicked())
            nextTrackPressed = true;
    }
}

/* Gets number input, backspace, return and save button events.
 * Only allows 0,1,2,3,4,5,6,7,8,9.*/
void Prompt::getNumberEvent() {
    renderText = false;
    returnPressed = false;
    textInput = false;
    backspacePressed = false;
    savePressed = false;
    while ( SDL_PollEvent( &event ) ) {
        if ( event.type == SDL_QUIT ) {
            quit = true;
        } else if ( event.type == SDL_TEXTINPUT ) {
            if(!(SDL_GetModState() &
                 KMOD_CTRL))
            {
                if (isSDLNumber())
                    textInput = true;
            }
        } else if (event.type == SDL_KEYDOWN ) {
            checkBackspaceAndReturn();
        }
        saveButton.updateEvent(&event);
        if (saveButton.isClicked())
            savePressed = true;
    }
}

/* Checks if backspace or return have been pressed */
void Prompt::checkBackspaceAndReturn() {
    if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
        backspacePressed = true;
        lengthReached = false;
    } else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN){
        returnPressed = true;
    }
}

/* Ignores ctrl+c, ctrl+v commands */
bool Prompt::isCtrlCommand() {
    return event.text.text[ 0 ] == 'c'
           || event.text.text[ 0 ] == 'v'
           || event.text.text[ 0 ] == 'C'
           || event.text.text[ 0 ] == 'V';
}

/* Checks if event is a number */
bool Prompt::isSDLNumber() {
    return event.text.text[ 0 ] == '0'||
           event.text.text[ 0 ] == '1'||
           event.text.text[ 0 ] == '2'||
           event.text.text[ 0 ] == '3'||
           event.text.text[ 0 ] == '4'||
           event.text.text[ 0 ] == '5'||
           event.text.text[ 0 ] == '6'||
           event.text.text[ 0 ] == '7'||
           event.text.text[ 0 ] == '8'||
           event.text.text[ 0 ] == '9';
}

/* Checks if track width is at least 1 in size and shorter than the limit.
 * If config is not OK, configs error to display in red.
 * If backspace is pressed, deletes error warnings.*/
void Prompt::processEventWidth() {
    int inputDimension = 0;
    if (textInput && inputText.length() <= MAX_LENGTH_TRACK_DIMENSION){
        inputText += event.text.text;
        renderText = true;
        nameError = " ";
    } else if (textInput) {
        lengthReached = true;
        nameError = MSG_TOO_LARGE_SIZE;
        renderWrongMessage = true;
    }
    if (backspacePressed && inputText.length() > 1){
        inputText.pop_back();
        renderText = true;
        renderWrongMessage = false;
    }
    // 1: SDL does not render empty strings (0).
    if (inputText.length()>1)
        inputDimension = toDimension();
    if ((returnPressed || savePressed) && inputDimensionGood(inputDimension)) {
        widthAccepted = true;
        trackWidth = inputDimension;
        std::cout << "WIDTH ACCEPTED: ";
        std::cout << trackWidth << std::endl;
        renderText = true;
        inputText = " ";
    } else if ((returnPressed || savePressed)){
        renderWrongMessage = true;
        widthAccepted = false;
        std::cout << "WIDTH INVALID." << std::endl;
        if (!inputDimensionGood(inputDimension))
            nameError = MSG_DIM_WIDTH;
        renderText = true;
    }
}

/* Checks if track height is at least 1 in size and shorter than the limit.
 * If config is not OK, configs error to display in red.
 * If backspace is pressed, deletes error warnings.*/
void Prompt::processEventHeight() {
    int inputDimension = 0;
    if (textInput && inputText.length() <= MAX_LENGTH_TRACK_DIMENSION){
        inputText += event.text.text;
        renderText = true;
        nameError = " ";
    } else if (textInput) {
        nameError = MSG_TOO_LARGE_SIZE;
        lengthReached = true;
    }
    if (backspacePressed && inputText.length() > 1){
        inputText.pop_back();
        renderText = true;
        renderWrongMessage = false;
    }
    // 1: SDL does not render empty strings (0).
    if (inputText.length()>1)
        inputDimension = toDimension();
    if ((returnPressed || savePressed) && inputDimensionGood(inputDimension)) {
        heightAccepted = true;
        trackHeight = inputDimension;
        std::cout << "HEIGHT ACCEPTED: ";
        std::cout << trackWidth << std::endl;
        renderText = true;
        inputText = " ";
    } else if ((returnPressed || savePressed)){
        renderWrongMessage = true;
        heightAccepted = false;
        std::cout << "HEIGHT INVALID." << std::endl;
        if (!inputDimensionGood(inputDimension))
            nameError = MSG_DIM_HEIGHT;
        renderText = true;
    }
}

/* Transforms SDL input text to a number */
int Prompt::toDimension(){
    return std::stoi(inputText.substr(1,std::string::npos));
}

/* Checks if dimension size is in the limits */
bool Prompt::inputDimensionGood(int dimSize) {
    return (dimSize <= MAX_TRACK_SIZE) && (dimSize>1);
}

/* Configs text colors, positions, fonts and textures. Creates box field.*/
void Prompt::initTextPrompt(SDL_Renderer *renderer, const std::string &promptString) {
    TTF_Init();
    promptTextColor = {255, 255, 255, 0};
    fieldTextColor = {0, 0, 0, 0};
    wrongTextColor = {255, 0, 0, 0};
    promptText.textToTexture(renderer, promptString.c_str(), promptTextColor, PROMPT_FONT, FONT_SIZE);
    textField.textToTexture(renderer, inputText.c_str(), fieldTextColor, FIELD_FONT, FONT_SIZE);
    wrongText.textToTexture(renderer, nameError.c_str(), wrongTextColor, FIELD_FONT, FONT_SIZE);
    SDL_StartTextInput();
    setTextPosition();
    SDL_Color fieldColor = {200, 200, 200, 0};
    textField.createFieldBox(getTextFieldPosX(), getTextFieldPosY(), fieldColor, (WINDOW_W - 2*getTextFieldPosX()));
}

/* Configs text position */
void Prompt::setTextPosition(){
    textPosXPrompt = (WINDOW_W - promptText.getWidth()) / 2;
    textPosXField = PROMPTXSPACING;
    textPosYPrompt = (WINDOW_H - promptText.getHeight()) / 2 - promptText.getHeight() / 2 - PROMPTYSPACING;
    textPosYField = (WINDOW_H - promptText.getHeight()) / 2 + promptText.getHeight()/2 + PROMPTYSPACING;
}

/* Transforms text to texture and avoids empty string errors */
void Prompt::processTextPrompt(SDL_Renderer * renderer, bool & renderText) {
    if(renderText)
    {
        if( !inputText.empty())
            textField.textToTexture(renderer, inputText.c_str(), fieldTextColor, FIELD_FONT, FONT_SIZE);
        else
            textField.textToTexture(renderer, " ", fieldTextColor, FIELD_FONT, FONT_SIZE);
        if( !nameError.empty())
            wrongText.textToTexture(renderer, nameError.c_str(), wrongTextColor, FIELD_FONT, FONT_SIZE);
        else
            wrongText.textToTexture(renderer, " ", wrongTextColor, FIELD_FONT, FONT_SIZE);
    }
}

/* Renders all texts and boxes */
void Prompt::renderTextPrompt(SDL_Renderer * renderer) {
    promptText.render(renderer, textPosXPrompt, textPosYPrompt);
    textField.renderFieldBox(renderer);
    textField.render(renderer, textPosXField, textPosYField);
    if (renderWrongMessage) {
        wrongText.render(renderer,
                         (WINDOW_W - wrongText.getWidth())/2,
                         textPosYField + textField.getHeight() + 20);
    }
}

/* Gets input text position */
int Prompt::getTextFieldPosX() {
    return textPosXField;
}

/* Gets input text position */
int Prompt::getTextFieldPosY() {
    return textPosYField;
}

/* Gets track name set by user */
std::string Prompt::getTrackName() {
    return trackName;
}

/* Gets track width set by user */
int Prompt::getWidth() {
    return trackWidth;
}

/* Gets track height set by user */
int Prompt::getHeight() {
    return trackHeight;
}

Prompt::Prompt() = default;
