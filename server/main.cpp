#include "./include/mm_server.h"
#include <iostream>

#define SUCCESS 0


int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cout << "El comando es: ./micromachines-sv <service>\n";
        return SUCCESS;
    }

    MMServer server;
    server.start(argv[1]);
    
    return SUCCESS;
}
