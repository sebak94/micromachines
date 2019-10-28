#ifndef __DRAWER_H__
#define __DRAWER_H__

#include <map>
#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "sdl/SdlWindow.h"
#include "sdl/SdlSurface.h"
#include "PictureLoader.h"
#include "PicType.h"
#include "Block.h"
#include "Car.h"
#include "Camera.h"

class Drawer : public Thread {
private:
    bool running;
    Socket &socket;
    SdlWindow window;
    std::map<PicType, SdlSurface*> pictures;
    PictureLoader loader;
    //x e y son para mover la pista, el auto es fijo, se mueve la pista. Las inicio en el (0,0)
    int x = 0;
    int y = 0;

public:
    Drawer(Socket &socket);
    ~Drawer();
    virtual void run() override;
    int testSdl(std::map<PicType, SdlSurface*> &pictures, std::string text);

private:
    std::string receive();
    void showAnimation(SdlWindow &window);
    void showBackground(int xPos, int yPos, std::map<PicType, SdlSurface*> &pictures, std::vector<Block> &blocks, Camera &camera);
    void showCars(int xPos, int yPos, std::vector<Car> &cars, Camera &camera);
};

#endif
