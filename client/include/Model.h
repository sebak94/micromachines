#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <map>
#include "Car.h"
#include "../../common/include/TrackPartData.h"
#include "../../common/include/ModifierList.h"

typedef enum {
    mainMenu,
    creating,
    joining,
    waitingPlayers,
    startCountdown,
    playing,
    waitingEnd,
    gameEnded
} GameState;

class Model {
private:
    std::map<std::string, Car*> cars;
    std::vector<TrackPartData> track;
    std::string myColor;
    int totalLaps = 10;
    GameState gameState = mainMenu;
    std::vector<std::string> trackNames;
    std::vector<std::string> matchNames;
    std::vector<std::string> matchResults;
    std::vector<Modifier> modifiers;

public:
    Model();
    ~Model();
    void setTrack(std::vector<TrackPartData> track);
    std::vector<TrackPartData>& getTrack();
    void setMyColor(std::string str); //Se usa una sola vez, para setear el color de mi auto
    std::string getMyColor() const;
    std::map<std::string, Car*>& getCars();
    void updateCar(std::string str);
    void setGameState(std::string str);
    void setTotalLaps(const std::string& str);
    int getTotalLaps();
    GameState getGameState();
    void setTrackNames(std::string tracks);
    std::vector<std::string> getTrackNames();
    std::vector<std::string>& getMatchResults();
    void updateMatchResults(std::string results);
    std::vector<Modifier>& getModifiers();
    void createModifiers(std::string str);
    void setGameState(GameState state);
    void setMatchNames(std::string matches);
    std::vector<std::string> getMatchNames();

private:

    std::string parse(const std::string &str, size_t &pos, const char delim);


};

#endif
