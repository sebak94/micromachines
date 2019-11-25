//
// Created by fperezboco on 22/11/19.
//

#ifndef MICROMACHINES_SOUNDTH_H
#define MICROMACHINES_SOUNDTH_H


#include "sdl/SdlSoundFX.h"
#include "../../common/include/thread.h"
#include "ModelMonitor.h"
#include "sdl/SdlWindow.h"
#include "../../common/include/Config.h"
#include "sdl/SdlMusic.h"
#define PLAY_MUSIC "play music"

class SoundTh : public Thread {
private:
    SdlWindow & window;
    ModelMonitor & modelMonitor;
    Config config;
    double drawDistance;
    SdlMusic menuMusic;
    SdlMusic raceMusic;
    SdlSoundFX carSound;
    SdlSoundFX engineStartSound;
    SdlSoundFX collisionSound;
    SdlSoundFX explosionSound;
    SdlSoundFX winSound;
    SdlSoundFX brakeSound;
    bool running = false;
    GameState lastState = gameEnded;
    bool once = false;

public:
    SoundTh(ModelMonitor &modelMonitor, SdlWindow &window,
            Config &config);
    void run() override;
    void stop() override;
    void playCarSounds();
    double calcSoundLevel(int x1, int y1, int x2, int y2);
    void musicPlayOnce(SdlMusic &music1, SdlMusic &music2);
    void playOnce();
    void soundPlayOnce(SdlSoundFX &sound, int duration);

    void playCarSoundFX(uint8_t volume, int ticks, bool collided,
                        bool exploded);
};


#endif //MICROMACHINES_SOUNDTH_H
