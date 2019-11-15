#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <map>
#include "Model.h"
#include "sdl/SdlSurface.h"
#include "../../common/include/TextTexture.h"
#include "sdl/SdlTexture.h"

#define LAPBOXPATH "../common/images/lapCounter.png"
#define LAPFONT "../common/fonts/OpenSans-Italic.ttf"

class Camera {
private:
    SdlWindow &window;
    std::map<std::string, SdlSurface*> &pictures;
    std::map<trackPartType, SdlSurface*> &trackPictures;
    double blockWidth;
    double blockHeight;
    SdlSurface lapBox;
    TextTexture lapNumber;

public:
    Camera(SdlWindow &window, std::map<std::string, SdlSurface*> &pictures, std::map<trackPartType, SdlSurface*> &trackPictures);
    ~Camera();
    void showBackground();
    void showTrack(int xMyCar, int yMyCar, std::vector<TrackPartData> &track);
    void showCars(int xMyCar, int yMyCar, std::map<std::string, Car*> &cars);
    void updateBlockSize();
    void showLaps(int i);
};

#endif
