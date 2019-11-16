#ifndef __MODELMONITOR_H
#define __MODELMONITOR_H

#include <mutex>
#include "../include/Model.h"

class ModelMonitor {
private:
    std::mutex m;
    Model &model;

public:
    ModelMonitor(Model& model);
    ~ModelMonitor();
    void setTrack(std::vector<TrackPartData> track);
    std::vector<TrackPartData>& getTrack();
    void setMyColor(std::string str);
    std::string getMyColor() const;
    std::map<std::string, Car*>& getCars();
    void updateCar(std::string str);
    void setGameState(std::string str);
    GameState getGameState();
    void setTrackNames(std::string tracks);
    std::vector<std::string> getTrackNames();
};

#endif
