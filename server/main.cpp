#include "include/acceptor_th.h"
#include "../common/include/socket_error.h"
#include <iostream>
#include <string>

#define SUCCESS 0

void run(const char* service) {
    try {
        AcceptorTh acceptor_th(service);
        acceptor_th.start();
        while (getc(stdin) != 'q') {}
        acceptor_th.stop();
        acceptor_th.join();
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cout << "El comando es: ./micromachines-sv <service>\n";
        return SUCCESS;
    }
    run(argv[1]);
    return SUCCESS;
}
