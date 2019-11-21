#ifndef __DRAWER_H__
#define __DRAWER_H__

#define ASPECTRATIO 3/2
#define BASERESOLUTIONMULTIPLIER 38  // for recording
#define HEIGHT 16*BASERESOLUTIONMULTIPLIER
#define WIDTH HEIGHT*ASPECTRATIO

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
#include "../../record/include/Record.h"
#include "MatchWindow.h"

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
    Button recButton;
    std::vector<char> lastFrame;
    Record video;
    std::mutex recordMutex;
    bool lastRecordState = false;
    MatchWindow matchWindow;

public:
    explicit Drawer(ModelMonitor &modelMonitor);
    ~Drawer();
    virtual void run() override;
    virtual void stop() override;
    void resize(int width, int height);
    void updateFullScreenButton(const SDL_Event * event);
    void updateRecButton(const SDL_Event *event);
    MatchWindow& getMatchWindow();

private:
    void createFullScreenButton();
    void showFullScreenButton();
    void draw();
    void showAnimation(SdlWindow &window);
    void createRecButton();
    void showRecButton();
    void recorderTh();
    void saveLastFrame();

    void drawWorld();

    void drawHUD();
};

#endif
