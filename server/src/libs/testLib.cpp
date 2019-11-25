//
// Created by mati on 10/29/19.
//

//g++ -shared -fPIC testLib.cpp -o ../../include/libs/so/lib.so

#include <iostream>
#include "../../include/libs/testLib.h"

using namespace std;
bool state = false;

extern "C" void execute(char *command) {
    memcpy(command, "Y", 1);
}
