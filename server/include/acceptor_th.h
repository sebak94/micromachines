#ifndef __ACCEPTOR_TH_H__
#define __ACCEPTOR_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "model/micromachines.h"
#include "../include/client_th.h"
#include "vector"

class AcceptorTh: public Thread {
    private:
    Socket skt;
    bool keep_accepting;
    Micromachines micromachines;
    std::vector<ClientTh*> clients;
    std::vector<Socket*> sockets;
    void deleteDeadClients();
    void deleteResources();

    public:
    AcceptorTh(const char *service, Micromachines micromachines);
    virtual void run() override;
    virtual void stop() override;
    ~AcceptorTh();
};

#endif
