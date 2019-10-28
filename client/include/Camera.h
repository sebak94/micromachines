#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Model.h"

class Camera {
private:
    Model &model;
    int camWidth;
    int camHeight;

public:
    Camera(Model &model, int camWidth, int camHeigth);
    ~Camera();
};

#endif
