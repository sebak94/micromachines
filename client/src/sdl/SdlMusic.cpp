#include "../../include/sdl/SdlMusic.h"
#include "../../include/sdl/SdlException.h"

SdlMusic::SdlMusic(const std::string filename) {
    int errCode = SDL_Init(SDL_INIT_AUDIO);
    if (errCode) {
        throw SdlException("Error en la inicialización del audio", SDL_GetError());
    }
    //Initialize SDL_mixer
    if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) {
        throw SdlException("Error en la inicialización del audio", SDL_GetError());
    }

    music = Mix_LoadMUS(filename.c_str());
    if( music == NULL ) {
        throw SdlException("Error en el archivo de audio", SDL_GetError());
    }
}

SdlMusic::~SdlMusic() {
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}

void SdlMusic::play() {
    Mix_PlayMusic(music, -1);
}

void SdlMusic::stop() {
    Mix_HaltMusic();
}
