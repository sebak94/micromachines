#include <SDL2/SDL_image.h>
#include <string>
#include "../../include/sdl/SdlSurface.h"
#include "../../include/sdl/SdlException.h"

SdlSurface::SdlSurface(const std::string &filename, const SdlWindow& window) : renderer(window.getRenderer()) {
    this->surface = loadSurface(filename);
    this->texture = SDL_CreateTextureFromSurface(window.getRenderer(), this->surface);
}

SdlSurface::~SdlSurface() {
    SDL_FreeSurface(this->surface);
}

SDL_Surface* SdlSurface::loadSurface(const std::string &filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface) {
        throw SdlException("Error al cargar la superficie", SDL_GetError());
    }
    return surface;
}

int SdlSurface::render(SDL_Rect& sdlDest) const {
    return SDL_RenderCopy(renderer, texture, NULL, &sdlDest);
}

int SdlSurface::renderRotate(SDL_Rect& sdlDest, double angle, SDL_RendererFlip flip) const {
    return SDL_RenderCopyEx(renderer, texture, NULL, &sdlDest, angle, NULL, flip);
}
