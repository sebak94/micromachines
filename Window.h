//
// Created by fperezboco on 18/10/19.
//

#include "Error.h"

#ifndef TP_WINDOW_H
#define TP_WINDOW_H

#define WINDOW_W 640  // width
#define WINDOW_H 480  // height

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_system.h>
#include <string>

class Window {
public:
    SDL_Window* gameWindow = nullptr;
    SDL_Surface* gameSurface = nullptr;
    SDL_Renderer* renderer = nullptr;

    void startGUI(const std::string &name);
    ~Window();

    void fillBackground(int r, int g, int b, int a);
};


#endif //TP_WINDOW_H
