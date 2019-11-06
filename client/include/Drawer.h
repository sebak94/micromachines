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

class Drawer : public Thread {
private:
    bool running;
    Socket &socket;
    SdlWindow window;
    std::map<std::string, SdlSurface*> pictures;
    std::map<trackPartType, SdlSurface*> trackPictures;
    PictureLoader loader;
    Model model;
    Camera camera;

public:
    Drawer(Socket &socket);
    ~Drawer();
    virtual void run() override;
    virtual void stop() override;
    int draw();

private:
    std::string receive();
    void showAnimation(SdlWindow &window);
};

#endif
