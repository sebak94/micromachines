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
    void setMyColor(std::string str);
    void updateCar(std::string str);
    void setGameState(std::string str);
    void setTotalLaps(std::string str);
    int getTotalLaps();
    std::vector<TrackPartData>& getTrack();
    std::string getMyColor() const;
    std::map<std::string, Car*>& getCars();
    GameState getGameState();
    void setTrackName(const std::string& str);
    void setTrackList(const std::string &str);
    std::vector<std::string> getTrackList();

    bool trackIsSet();

    std::string getTrackName();
};

#endif
