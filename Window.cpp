//
// Created by fperezboco on 18/10/19.
//

#include "Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_system.h>
#include <string>
#include <SDL2/SDL_image.h>

/* also creates renderer for textures in that window */
void Window::startGUI(const std::string &name) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw Error("Doing INIT %s", SDL_GetError());
    } else {
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
            throw Error("Setting Hint");
        gameWindow = SDL_CreateWindow(name.c_str(),
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      WINDOW_W,
                                      WINDOW_H,
                                      SDL_WINDOW_SHOWN);
        if (gameWindow == nullptr)
            throw Error( "Creating Window %s", SDL_GetError());
        renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            throw Error("Creating Renderer %s", SDL_GetError());
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
            throw Error("Setting IMG_Init %s", SDL_GetError());
        }
    }
}

void Window::fillBackground(int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

Window::~Window() {
    SDL_DestroyWindow( gameWindow );
    gameWindow = nullptr;
    SDL_Quit();
}