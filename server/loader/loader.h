//
// Created by mati on 30/10/19.
//

#ifndef MICROMACHINES_LOADER_H
#define MICROMACHINES_LOADER_H

#include "../../common/include/directory_parser.h"
#include <iostream>
#include <dlfcn.h>
#include "testVir.h"

class Loader {
private:
    DirectoryParser parser;

public:
    Loader(const char * path);

    ~Loader();

    void load_dynamic_libraries();
};


#endif //MICROMACHINES_LOADER_H
