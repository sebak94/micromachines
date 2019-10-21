#include <SDL2/SDL_image.h>
#include <string>
#include "../../include/sdl/SdlSurface.h"
#include "../../include/sdl/SdlException.h"

SdlSurface::SdlSurface(const std::string &filename, const SdlWindow& window)
        : renderer(window.getRenderer()) {
    this->surface = loadSurface(filename);
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

int SdlSurface::render(SDL_Rect sdlDest) const {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderer, this->surface);
    return SDL_RenderCopy(this->renderer, texture, NULL, &sdlDest);
}
