//
// Created by mati on 10/29/19.
//

#ifndef MICROMACHINES_TESTLIB_H
#define MICROMACHINES_TESTLIB_H

#include "testVir.h"
#include <string.h>

class TestLib : TestVir {
public:
    void execute(char *command);
};

#endif //MICROMACHINES_TESTLIB_H
