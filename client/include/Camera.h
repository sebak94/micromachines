#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <map>
#include "Model.h"
#include "sdl/SdlSurface.h"

class Camera {
private:
    SdlWindow &window;
    Model &model;
    std::map<PicType, SdlSurface*> &pictures;
    double blockWidth;
    double blockHeight;

public:
    Camera(SdlWindow &window, Model &model, std::map<PicType, SdlSurface*> &pictures);
    ~Camera();
    void showBackground();
    void showTrack(int xMyCar, int yMyCar, int xPos, int yPos);
    void showCars(int xMyCar, int yMyCar, int xPos, int yPos);
};

#endif
