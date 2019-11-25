#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <map>
#include <chrono>
#include "Model.h"
#include "sdl/SdlSurface.h"
#include "../../common/include/TextTexture.h"
#include "sdl/SdlTexture.h"
#include "sdl/SdlAnimation.h"

#define LAPBOXPATH "../common/images/lapCounter.png"
#define LAPFONT "../common/fonts/OpenSans-Italic.ttf"
#define SECSTOSTART 5
#define SECOND 900

typedef std::chrono::time_point<std::chrono::steady_clock> saveTickChrono;

class Camera {
private:

    SdlWindow &window;
    std::map<Car*, SdlAnimation> explosions;
    std::map<std::string, SdlSurface*> &pictures;
    std::map<trackPartType, SdlSurface*> &trackPictures;
    std::map<Car*, int> carVibration;
    double blockWidth;
    double blockHeight;
    SdlSurface lapBox;
    TextTexture lapNumber;
    SdlTexture explosionTex;
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

    void triggerExplosion(Car *car);

    void explodeCar(Car *car, int x, int y, int w, int h);

    void showLife(Car *car, int &realX, int &realY, double &heightCar);

    void vibrateCar(Car *car, int &x, int &y);
};

#endif
