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
    void setTotalLaps(std::string str);
    int getTotalLaps();
    void setTrackNames(std::string tracks);
    std::vector<std::string> getTrackNames();
    void setGameState(GameState state);
    void setMatchNames(std::string matches);
    std::vector<std::string> getMatchNames();
    std::vector<std::string> &getMatchResults();
    void updateMatchResults(std::string results);
    std::vector<Modifier>& getModifiers();
    void createModifier(std::string str);
};

#endif
