#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>

class Model {
    public:
    virtual void update() = 0;
    virtual std::string serialize() = 0;
};

#endif
