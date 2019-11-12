
#ifndef TESTVIR_H
#define TESTVIR_H

#include <string.h>

class TestVir {
public:
    virtual void execute(char *command) = 0;
};

#endif