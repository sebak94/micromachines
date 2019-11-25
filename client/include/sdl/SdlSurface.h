#ifndef __SDLSURFACE_H__
#define __SDLSURFACE_H__

#include <string>
#include "SdlWindow.h"

class SdlSurface {
private:
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;

public:
    SdlSurface(const std::string &filename, const SdlWindow& window);
    ~SdlSurface();
    int render(SDL_Rect& sdlDest) const;
    int renderRotate(SDL_Rect& sdlDest,
            double angle,
            SDL_RendererFlip flip) const;

private:
    SDL_Surface* loadSurface(const std::string &filename);
};

#endif
