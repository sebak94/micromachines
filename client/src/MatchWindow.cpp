#include <SDL2/SDL_ttf.h>
#include <vector>
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

    //Hardcodeo los nombres de las pistas y las partidas por ahora
    trackNames.push_back("track");
    trackNames.push_back("super track");
    itTrackNames = trackNames.begin();
    matchNames.push_back("partida 1");
    matchNames.push_back("partida 2");
    itMatchNames = matchNames.begin();
    players.push_back("2");
    players.push_back("3");
    players.push_back("4");
    players.push_back("5");
    itPlayers = players.begin();
}

MatchWindow::~MatchWindow() {}

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
        state = creating;
    }
    if (joinMatchButton.isClicked()) {
        state = joining;
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
    if (arrowButton.isClicked()) {
        if (state == creating) {
            itTrackNames++;
            if (itTrackNames == trackNames.end())
                itTrackNames = trackNames.begin();
        } else if (state == joining) {
            itMatchNames++;
            if (itMatchNames == matchNames.end())
                itMatchNames = matchNames.begin();
        }
    }
    if (playButton.isClicked()) {
        printf("play clicked\n");
    }
    if (returnButton.isClicked()) {
        state = selecting;
    }
}

void MatchWindow::updateMatchButtons(const SDL_Event *event) {
    //Agrego estos if para no actualizar siempre todos los botones
    if (state == selecting) {
        updateSelectingButtons(event);
    }
    if (state == creating) {
        updateCreatingButtons(event);
    }
    if (state != selecting) {
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
    if (state == selecting) {
        selectingScreen();
    } else if (state == creating) {
        creationScreen();
    } else if (state == joining) {
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
    showSelectText("Elegir escenario: ", *itTrackNames, window.getWidth() / 9, window.getHeight() / 2, arrowButton, textTrack);
    showSelectText("Cantidad de jugadores: ", *itPlayers, window.getWidth() / 9, window.getHeight() / 1.5, arrowButton2, textPlayers);
    showPlayAndReturn();
}

void MatchWindow::joiningScreen() {
    showBackground();
    showSelectText("Partidas disponibles: ", *itMatchNames, window.getWidth() / 9, window.getHeight() / 2, arrowButton, textMatch);
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