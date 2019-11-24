#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <map>
#include <chrono>
#include "Model.h"
#include "sdl/SdlSurface.h"
#include "../../common/include/TextTexture.h"
#include "sdl/SdlTexture.h"

#define LAPBOXPATH "../common/images/lapCounter.png"
#define LAPFONT "../common/fonts/OpenSans-Italic.ttf"
#define SECSTOSTART 5
#define SECOND 900

typedef std::chrono::time_point<std::chrono::steady_clock> saveTickChrono;

class Camera {
private:
    SdlWindow &window;
    std::map<std::string, SdlSurface*> &pictures;
    std::map<trackPartType, SdlSurface*> &trackPictures;
    double blockWidth;
    double blockHeight;
    SdlSurface lapBox;
    TextTexture lapNumber;
    double countDown = SECOND;
    double countDownNumber = SECSTOSTART;
    bool countDownStarted = false;
    double drawDistance;
    saveTickChrono start;

public:
    Camera(SdlWindow &window,
           std::map<std::string, SdlSurface *> &pictures,
           std::map<trackPartType, SdlSurface *> &trackPictures,
           double drawDistance);
    ~Camera();
    void showBackground();
    void showTrack(int xMyCar, int yMyCar, std::vector<TrackPartData> track);
    void showModifiers(int xMyCar, int yMyCar, std::vector<Modifier> modifiers);
    void showCars(int xMyCar, int yMyCar, std::map<std::string, Car *> cars,
                  const std::string& string);
    void showCountdown();
    void updateBlockSize();
    void showLaps(int lap, int totalLaps);
    void drawPodium(std::vector<std::string> matchResults);
    void reset();
    void validateDrawDistance();
};

#endif
