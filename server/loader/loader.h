//
// Created by mati on 30/10/19.
//

#ifndef MICROMACHINES_LOADER_H
#define MICROMACHINES_LOADER_H

#include "../../common/include/directory_parser.h"
#include <iostream>
#include <dlfcn.h>
#include "testVir.h"

typedef TestVir *execute_t();

class Loader {
private:
    DirectoryParser parser;
    std::vector<execute_t *> functors;

public:
    Loader();

    ~Loader();

    void load_dynamic_libraries();

    void execute();
};


#endif //MICROMACHINES_LOADER_H
