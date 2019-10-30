//
// Created by mati on 10/29/19.
//

//g++ -shared -fPIC testLib.cpp -o testLib.so

#include <iostream>
#include "testLib.h"

using namespace std;

void TestLib::init() {
    printf("TestLib::init: Hello World!! \n");
}

extern "C" TestLib *create() {
    return new TestLib;
}

extern "C" void destroy(TestLib *Tl) {
    delete Tl;
}