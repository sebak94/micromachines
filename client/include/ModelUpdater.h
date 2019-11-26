#ifndef __MODELUPDATER_H
#define __MODELUPDATER_H

#include <string>
#include "../../common/include/thread.h"
#include "Model.h"
#include "../../common/include/socket.h"
#include "Drawer.h"

class ModelUpdater : public Thread {
private:
    bool running = false;
    Socket &socket;
    ModelMonitor &modelMonitor;
    Drawer* drawer;

public:
    ModelUpdater(Socket &socket,
            ModelMonitor &modelMonitor,
            Drawer* drawerThread);
    ~ModelUpdater();
    void run() override;
    void stop() override;

private:
    std::string receive();
};

#endif
