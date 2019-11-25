//
// Created by fperezboco on 22/11/19.
//

#include "../include/SoundTh.h"
#include "../include/ModelMonitor.h"
#include "../include/sdl/SdlWindow.h"
#include "../../common/include/Config.h"
#include <unistd.h>
#include <iostream>

#define VOL_ATTENUATION 0.2
#define SOUND_REFRESH 400000
#define BRAKE_LEN 500
#define DRAW_DISTANCE "draw distance [4 - 8]"
#define CAR_SOUND_PATH "../common/sounds/engine.wav"
#define ENGINE_START_PATH "../common/sounds/engineStartUp.wav"
#define MUSICPATH1 "../common/sounds/beat.wav"
#define MUSICPATH2 "../common/sounds/50s-bit.ogg"
#define RACE_FINISH_PATH "../common/sounds/win.wav"
#define COLLISION_PATH "../common/sounds/punch.ogg"
#define EXPLODE_PATH "../common/sounds/explodemini.wav"
#define TIRES_PATH "../common/sounds/tires.wav"

SoundTh::SoundTh(ModelMonitor &modelMonitor, SdlWindow &window,
                 Config &config) :
        window(window),
        modelMonitor(modelMonitor),
        config(config),
        drawDistance(config.getAsDouble(DRAW_DISTANCE)),
        menuMusic(MUSICPATH1),
        raceMusic(MUSICPATH2),
        carSound(CAR_SOUND_PATH),
        engineStartSound(ENGINE_START_PATH),
        collisionSound(COLLISION_PATH),
        explosionSound(EXPLODE_PATH),
        winSound(RACE_FINISH_PATH),
        brakeSound(TIRES_PATH){
}

void SoundTh::run() {
    running = true;
    while (running) {
        auto start = std::chrono::system_clock::now();
        try {
            playOnce();
            if (modelMonitor.getGameState() == mainMenu) {
                musicPlayOnce(raceMusic, menuMusic);
            } else if (modelMonitor.getGameState() == waitingPlayers) {
                menuMusic.stop(config.isSet(PLAY_MUSIC));
                soundPlayOnce(engineStartSound,0);
            } else if (modelMonitor.getGameState() == startCountdown) {
                menuMusic.stop(config.isSet(PLAY_MUSIC));
                playCarSounds();
            } else if (modelMonitor.getGameState() == playing) {
                playCarSounds();
                musicPlayOnce(menuMusic, raceMusic);
            } else if (modelMonitor.getGameState() == waitingEnd) {
                winSound.volume(50);
                soundPlayOnce(winSound, 0);
            }
        } catch (std::exception &e) {
            printf("SoundTh::run() exception catched: %s\n", e.what());
            running = false;
        }
        auto end = std::chrono::system_clock::now();
        int microsecsPassed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        if (SOUND_REFRESH > microsecsPassed)
            usleep(SOUND_REFRESH - microsecsPassed);
    }
}

// sets if it must play once
void SoundTh::playOnce() {
    GameState actualState = modelMonitor.getGameState();
    once = false;
    if (actualState != lastState) {
        once = true;
        lastState = actualState;
    }
}

// duration 0 to play entire sound. plays only if changed state
void SoundTh::soundPlayOnce(SdlSoundFX & sound, int duration) {
    if (once) sound.play(duration, 0);
}

// plays only if changed state
void SoundTh::musicPlayOnce(SdlMusic & music1, SdlMusic & music2) {
    if (once) {
        music1.stop(config.isSet(PLAY_MUSIC));
        music2.play(config.isSet(PLAY_MUSIC));
    }
}

void SoundTh::stop() {
    running = false;
}

void SoundTh::playCarSounds() {
    std::map<std::string, Car *> cars = modelMonitor.getCars();
    Car * myCar = cars[modelMonitor.getMyColor()];
    int xMyCar = myCar->getX();
    int yMyCar = myCar->getY();
    for (auto & it : cars) {
        Car * car = it.second;
        if (car == myCar) {
            playCarSoundFX(5, -1, car->collided(), car->explodedSound());
        } else {
            double vol = calcSoundLevel(xMyCar, yMyCar, car->getX(), car->getY());
            vol *= VOL_ATTENUATION;
            playCarSoundFX(vol, -1, car->collided(), car->explodedSound());
        }
    }
}

void SoundTh::playCarSoundFX(uint8_t volume, int ticks, bool collided,
                             bool exploded) {
    carSound.volume(volume);
    carSound.play(ticks, 0);
    if (exploded) {
        explosionSound.volume(50);
        explosionSound.play(ticks, 0);
    } else if (collided) {
        collisionSound.volume(volume);
        collisionSound.play(ticks, 0);
    }
    if (modelMonitor.isBraking())
        brakeSound.play(BRAKE_LEN, 0);
}

// if distance is greater than drawDistance/2, volume is 0.
double SoundTh::calcSoundLevel(int x1, int y1, int x2, int y2) {
    int sqDistance = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
    double limit = (100 * drawDistance / 2) * (100 * drawDistance / 2);
    if (sqDistance > limit) return 0;
    else return 100 - (sqDistance * 100 / limit);
}