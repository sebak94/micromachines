#ifndef __SDLSURFACE_H__
#define __SDLSURFACE_H__

#include <string>
#include "SdlWindow.h"

class SdlSurface {
private:
    SDL_Renderer* renderer;
    SDL_Surface* surface;

public:
    SdlSurface(const std::string &filename, const SdlWindow& window);
    ~SdlSurface();
    int render(SDL_Rect sdlDest) const;

private:
    SDL_Surface* loadSurface(const std::string &filename);
};

#endif
