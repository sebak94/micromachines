#include <iostream>
#include "include/server.h"

#define SUCCESS 0

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        std::cout << "./client <hostname> <service>";
        return SUCCESS;
    }
    Server server(argv[1], argv[2]);
    server.runThreads();
    return SUCCESS;
}
