#ifndef __SDLSURFACE_H__
#define __SDLSURFACE_H__

#include <string>
#include "SdlWindow.h"

class SdlSurface {
private:
    SDL_Surface* surface;

public:
    SdlSurface(const std::string &filename);
    ~SdlSurface();
    int render(SDL_Rect sdlDest, const SdlWindow& window) const;
    int renderRotate(SDL_Rect sdlDest, double angle, SDL_RendererFlip flip, const SdlWindow& window) const;

private:
    SDL_Surface* loadSurface(const std::string &filename);
};

#endif
