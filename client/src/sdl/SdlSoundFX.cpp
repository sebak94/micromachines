//
// Created by fperezboco on 22/11/19.
//

#include "../../include/sdl/SdlSoundFX.h"
#include "../../include/sdl/SdlException.h"

SdlSoundFX::SdlSoundFX(const std::string &filename) {
    int errCode = SDL_Init(SDL_INIT_AUDIO);
    if (errCode) {
        throw SdlException("Error en la inicialización del audio", SDL_GetError());
    }
    //Initialize SDL_mixer
    if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) {
        throw SdlException("Error en la inicialización del audio", SDL_GetError());
    }

    sound = Mix_LoadWAV(filename.c_str());
    if( sound == nullptr ) {
        throw SdlException("Error en el archivo de audio", SDL_GetError());
    }
}

SdlSoundFX::~SdlSoundFX() {
    Mix_FreeChunk(sound);
    sound = nullptr;
}

void SdlSoundFX::play(int ticks, uint8_t loops) {
    Mix_PlayChannelTimed(-1, sound, loops, ticks);
}

// vol from 0 to 100 [%]
void SdlSoundFX::volume(uint8_t vol) {
    Mix_VolumeChunk(sound, MIX_MAX_VOLUME*vol/100);
}
