#ifndef __MODEL_H__
#define __MODEL_H__

class Model {
    public:
    virtual void update() = 0;
    virtual void serialize() = 0;
};

#endif
