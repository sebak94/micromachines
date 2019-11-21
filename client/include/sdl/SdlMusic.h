#ifndef __SDLMUSIC_H__
#define __SDLMUSIC_H__

#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

class SdlMusic {
private:
    Mix_Music *music = NULL;

public:
    explicit SdlMusic(const std::string &filename);
    ~SdlMusic();
    void play();
    void stop();
};

#endif
