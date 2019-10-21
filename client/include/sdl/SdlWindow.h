#ifndef __SDLWINDOW_H__
#define __SDLWINDOW_H__

#include <SDL2/SDL_render.h>

class SdlWindow {
private:
    int width;
    int height;
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    SdlWindow(int width, int height);
    ~SdlWindow();
    int getWidth() const;
    int getHeight() const;
    void fill();
    void render();
    SDL_Renderer* getRenderer() const;
};

#endif
