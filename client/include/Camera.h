#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <map>
#include <chrono>
#include <string>
#include <vector>
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
typedef std::chrono::milliseconds ms;

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
    void showTrack(int xMyCar, std::vector<TrackPartData> track, int yMyCar);
    void showModifiers(int xMyCar, int yMyCar, std::vector<Modifier> modifiers);
    void showCars(int xMyCar, int yMyCar, std::map<std::string, Car *> cars,
                  const std::string& myCar);
    void showCountdown();
    void updateBlockSize();
    void showLaps(int lap, int totalLaps);
    void drawPodium(std::vector<std::string> matchResults);
    void reset();
    void validateDrawDistance();
    void triggerExplosion(Car *car);
    void explodeCar(Car *car, int x, int y, int s);
    void showLife(Car *car, int &realX, int &realY, double &heightCar);
    void vibrateCar(Car *car, int &x, int &y);
    void calcBeginPos(double &xBegin, double &yBegin, int &x, int &y);
    void setCarSize(double &widthCar, double &heightCar);
    bool isOnWindow(const SDL_Rect &sdlDestRoad);
    void showNumber(SDL_Color &color, int fontsize, int displacement);
};

#endif
