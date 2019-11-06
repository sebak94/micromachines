//
// Created by fperezboco on 18/10/19.
//

#include "Error.h"

#ifndef TP_WINDOW_H
#define TP_WINDOW_H

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_system.h>
#include <string>

#define WINDOW_NAME "Track Editor"

#define WINDOW_W 800// width
#define WINDOW_H WINDOW_W  // height

class Window {
public:
    SDL_Window* gameWindow = nullptr;
    SDL_Surface* gameSurface = nullptr;
    SDL_Renderer* renderer = nullptr;
    void fillBackground(int r, int g, int b, int a);
    void startGUI(const std::string &name);
    void clearScreen();
    ~Window();
};


#endif //TP_WINDOW_H
