//
// Created by fperezboco on 22/11/19.
//

#ifndef MICROMACHINES_SDLSOUNDFX_H
#define MICROMACHINES_SDLSOUNDFX_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <string>

class SdlSoundFX {
private:
    Mix_Chunk * sound;

public:
    ~SdlSoundFX();
    explicit SdlSoundFX(const std::string &filename);
    void play(int ticks, uint8_t loops);
    void volume(uint8_t vol);
};


#endif  // MICROMACHINES_SDLSOUNDFX_H
