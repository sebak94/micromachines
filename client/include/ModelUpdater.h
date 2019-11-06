#ifndef __MODELUPDATER_H
#define __MODELUPDATER_H

#include "../../common/include/thread.h"
#include "Model.h"
#include "../../common/include/socket.h"
#include "Drawer.h"

class ModelUpdater : public Thread {
private:
    bool running;
    Socket &socket;
    ModelMonitor &modelMonitor;
    Thread* drawer;

public:
    ModelUpdater(Socket &socket, ModelMonitor &modelMonitor, Thread* drawerThread);
    ~ModelUpdater();
    virtual void run() override;
    virtual void stop() override;

private:
    std::string receive();
};

#endif
