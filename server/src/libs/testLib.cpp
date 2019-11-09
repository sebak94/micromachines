//
// Created by mati on 10/29/19.
//

//g++ -shared -fPIC testLib.cpp testLib2.cpp -o ../../include/libs/so/lib.so

#include <iostream>
#include "../../include/libs/testLib.h"

using namespace std;

extern "C" void execute() {
    printf("TestLib::execute: Hello World!! \n");
}
