//
// Created by mati on 10/29/19.
//

//g++ -ldl main.cpp -o test && ./test

#include <iostream>
#include <dlfcn.h>
#include "testVir.h"

using namespace std;

int main(int argc, char *argv[]) {
    void *handle;
    handle = dlopen("./testLib.so", RTLD_NOW);

    if (!handle) printf("The error is %s", dlerror());

    typedef TestVir *create_t();
    typedef void destroy_t(TestVir *);

    create_t *creat = (create_t *) dlsym(handle, "create");
    destroy_t *destroy = (destroy_t *) dlsym(handle, "destroy");

    if (!creat || !destroy) printf("The error is %s\n", dlerror());

    TestVir *tst = creat();
    printf("NOT OK\n");
    tst->init();
    printf("OK\n");
    destroy(tst);

    return 0;
}