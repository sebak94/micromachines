#include <iostream>
#include "include/ThreadsManager.h"

#define SUCCESS 0

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        std::cout << "./client <hostname> <service>";
        return SUCCESS;
    }
    ThreadsManager th_manager(argv[1], argv[2]);
    th_manager.runThreads();
    return SUCCESS;
}
