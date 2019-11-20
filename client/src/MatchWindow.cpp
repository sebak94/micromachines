#include <SDL2/SDL_ttf.h>
#include <vector>
#include <sstream>
#include "../include/MatchWindow.h"

#define LOGOPATH "../common/images/micromachines.png"
#define BUTTONCREATEPATH "../common/images/buttons/buttonCreate.png"
#define BUTTONJOINPATH "../common/images/buttons/buttonJoin.png"
#define BUTTONPLAYPATH "../common/images/buttons/buttonPlay.png"
#define BUTTONRETURNPATH "../common/images/buttons/buttonReturn.png"
#define BUTTONARROWPATH "../common/images/arrow.png"
#define FONTNAME "../common/fonts/OpenSans-Bold.ttf"
#define BACK "../common/images/back.png"

MatchWindow::MatchWindow(SdlWindow &sdlWindow) : window(sdlWindow),
    logo(LOGOPATH, window), background(BACK, window) {
    TTF_Init();
    createMatchButtons();

    //Track vacío mientras carga
    trackNames.clear();

    //Defino que el juego sea entre 2 y 5 jugadores
    players.push_back("2");
    players.push_back("3");
    players.push_back("4");
    players.push_back("5");
    itPlayers = players.begin();

    //Hardcodeo los nombres de las partidas por ahora
    matchNames.push_back("0");
    matchNames.push_back("1");
    itMatchNames = matchNames.begin();
}

MatchWindow::~MatchWindow() {}

void MatchWindow::setTrackNames(std::vector<std::string> tracks) {
    if (tracks != trackNames) {
        trackNames.clear();
        trackNames = tracks;
        itTrackNames = trackNames.begin();
    }
}

void MatchWindow::createMatchButtons() {
    //Los creo con areas vacias y despues cuando los dibujo los actualizo acorde al tamaño de la ventana
    SDL_Rect area = {};
    createMatchButton = Button(window.getRenderer(), area, BUTTONCREATEPATH);
    joinMatchButton = Button(window.getRenderer(), area, BUTTONJOINPATH);
    arrowButton = Button(window.getRenderer(), area, BUTTONARROWPATH);
    arrowButton2 = Button(window.getRenderer(), area, BUTTONARROWPATH);
    playButton = Button(window.getRenderer(), area, BUTTONPLAYPATH);
    returnButton = Button(window.getRenderer(), area, BUTTONRETURNPATH);
}

void MatchWindow::updateSelectingButtons(const SDL_Event *event) {
    createMatchButton.updateEvent(event);
    joinMatchButton.updateEvent(event);
    if (createMatchButton.isClicked()) {
        state = creatingMatch;
    }
    if (joinMatchButton.isClicked()) {
        state = joiningMatch;
    }
}

void MatchWindow::updateCreatingButtons(const SDL_Event *event) {
    arrowButton2.updateEvent(event);
    if (arrowButton2.isClicked()) {
        itPlayers++;
        if (itPlayers == players.end())
            itPlayers = players.begin();
    }
}

void MatchWindow::updateNonSelectingButtons(const SDL_Event *event) {
    arrowButton.updateEvent(event);
    playButton.updateEvent(event);
    returnButton.updateEvent(event);
    ready = false;
    if (arrowButton.isClicked()) {
        if (state == creatingMatch) {
            itTrackNames++;
            if (itTrackNames == trackNames.end())
                itTrackNames = trackNames.begin();
        } else if (state == joiningMatch) {
            itMatchNames++;
            if (itMatchNames == matchNames.end())
                itMatchNames = matchNames.begin();
        }
    }
    if (playButton.isClicked()) {
        ready = true;
    }
    if (returnButton.isClicked()) {
        state = selectingMode;
    }
}

std::string MatchWindow::serializeData() {
    std::string response{};
    if (state == creatingMatch) {
        //response += "C,";
        response += textTrack.getText() + ",";
        response += textPlayers.getText();
        response += "\n";
    } else if (state == joiningMatch) {
        //response += "J,";
        response += textMatch.getText() + ",";
        response += "0";
        response += "\n";
    }
    return response;
}

bool MatchWindow::isReady() {
    return ready;
}

void MatchWindow::updateMatchButtons(const SDL_Event *event) {
    //Agrego estos if para no actualizar siempre todos los botones
    if (state == selectingMode) {
        updateSelectingButtons(event);
    }
    if (state == creatingMatch) {
        updateCreatingButtons(event);
    }
    if (state != selectingMode) {
        updateNonSelectingButtons(event);
    }
}

void MatchWindow::showBackground() {
    SDL_Rect backArea = {0, 0, window.getWidth(), window.getHeight()};
    background.render(backArea);
    SDL_Rect areaLogo = {window.getWidth() / 6, window.getHeight() / 6, (int)(window.getWidth() / 1.5), window.getHeight() / 4};
    logo.render(areaLogo);
}

void MatchWindow::render() {
    if (state == selectingMode) {
        selectingScreen();
    } else if (state == creatingMatch) {
        creationScreen();
    } else if (state == joiningMatch) {
        joiningScreen();
    }
}

void MatchWindow::selectingScreen() {
    showBackground();
    int w = window.getWidth() / 4;
    int h = window.getHeight() / 10;

    createMatchButton.updateSize(w, h);
    createMatchButton.updatePos(window.getWidth() / 4, window.getHeight() / 2);
    createMatchButton.draw(window.getRenderer());

    joinMatchButton.updateSize(w, h);
    joinMatchButton.updatePos((int)(window.getWidth() / 2), window.getHeight() / 2);
    joinMatchButton.draw(window.getRenderer());
}

void MatchWindow::creationScreen() {
    showBackground();
    if (!trackNames.empty()) {
        showSelectText("Choose track: ", *itTrackNames, window.getWidth() / 9, window.getHeight() / 2, arrowButton, textTrack);
        showSelectText("Number of players: ", *itPlayers, window.getWidth() / 9, window.getHeight() / 1.5, arrowButton2, textPlayers);
    }
    showPlayAndReturn();
}

void MatchWindow::joiningScreen() {
    showBackground();
    showSelectText("Available matchs: ", *itMatchNames, window.getWidth() / 9, window.getHeight() / 2, arrowButton, textMatch);
    showPlayAndReturn();
}

void MatchWindow::showSelectText(std::string title, std::string textToShow, int x, int y, Button &buttonToChangeText, TextTexture &text) {
    SDL_Color black = {0, 0, 0, 0};
    SDL_Color white = {255,255,255,0};
    SDL_Color lightGrey = {226, 214, 214, 0};

    TextTexture textTitle;
    int fontSize = (window.getWidth() + window.getHeight()) / 60;
    textTitle.textToTexture(window.getRenderer(), title, white, FONTNAME, fontSize);
    textTitle.render(window.getRenderer(), x, y);

    fontSize = (window.getWidth() + window.getHeight()) / 85;
    text.textToTexture(window.getRenderer(), textToShow, black, FONTNAME, fontSize);
    text.createFieldBox(x, y + textTitle.getHeight(), lightGrey, window.getWidth() / 3);
    text.renderFieldBox(window.getRenderer());
    text.render(window.getRenderer(), x, y + textTitle.getHeight());

    buttonToChangeText.updatePos(window.getWidth() / 2, y + textTitle.getHeight());
    buttonToChangeText.updateSize(window.getWidth() / 20, window.getHeight() / 20);
    buttonToChangeText.draw(window.getRenderer());
}

void MatchWindow::showPlayAndReturn() {
    int w = window.getWidth() / 4;
    int h = window.getHeight() / 10;
    playButton.updatePos(window.getWidth() / 1.5, window.getHeight() / 1.2);
    playButton.updateSize(w, h);
    playButton.draw(window.getRenderer());
    returnButton.updatePos(window.getWidth() / 9, window.getHeight() / 1.2);
    returnButton.updateSize(w, h);
    returnButton.draw(window.getRenderer());
}

StateWindow MatchWindow::getState() const {
    return this->state;
}

bool MatchWindow::isModeSelected() {
    return this->state != selectingMode;
}

std::string MatchWindow::getSelection() {
    if (this->state == creatingMatch) return "C\n";
    else if (this->state == joiningMatch) return "J\n";
}
