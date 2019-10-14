#ifndef __SERVER_H__
#define __SERVER_H__

#include "../../common/include/socket.h"
#include "string"

class Server {
    private:
    Socket skt;

    public:
    void send(std::string command);
    std::string receive();
    Server(const char *hostname, const char *service);
    ~Server();
};

#endif
