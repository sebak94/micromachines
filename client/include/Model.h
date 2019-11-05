#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "Car.h"
#include "../../common/include/TrackPartData.h"

class Model {
private:
    std::vector<Car> cars;
    std::vector<TrackPartData> trackPartData;

public:
    Model();
    ~Model();
    std::vector<Car>& getCars();
    void setTrackPartData(std::vector<TrackPartData> trackPartData);
    std::vector<TrackPartData> getTrackPartData() const;
};

#endif
