#ifndef __DRAWER_H__
#define __DRAWER_H__

#include <map>
#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "sdl/SdlWindow.h"
#include "sdl/SdlSurface.h"
#include "PictureLoader.h"
#include "Car.h"
#include "Camera.h"
#include "../include/ModelMonitor.h"
#include "sdl/SdlMusic.h"
#include "../../common/include/Button.h"

class Drawer : public Thread {
private:
    bool running;
    SdlWindow window;
    std::map<std::string, SdlSurface*> pictures;
    std::map<trackPartType, SdlSurface*> trackPictures;
    PictureLoader loader;
    ModelMonitor &modelMonitor;
    Camera camera;
    SdlMusic music;
    Button fullScreenButton;

public:
    Drawer(ModelMonitor &modelMonitor);
    ~Drawer();
    virtual void run() override;
    virtual void stop() override;
    void resize(int width, int height);
    void updateFullScreenButton(const SDL_Event * event);

private:
    void createFullScreenButton();
    void showFullScreenButton();
    void draw();
    void showAnimation(SdlWindow &window);
};

#endif
