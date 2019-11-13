#ifndef __ACCEPTOR_TH_H__
#define __ACCEPTOR_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "../../common/include/lock.h"
#include "model/micromachines.h"
#include "../include/client_th.h"
#include "vector"
#include <mutex>

class ClientList {
    private:
    std::mutex m;
    std::vector<ClientTh*> clients;
    Micromachines &micromachines;

    public:
    ClientList(Micromachines &micromachines);
    void addClient(ClientTh *client);
    void deleteDeadClients();
    ~ClientList();
};

class LookForDeadClientsTh: public Thread {
    private:
    bool keep_looking;
    ClientList &clients;
    Micromachines &micromachines;

    public:
    LookForDeadClientsTh(ClientList &clients, Micromachines &micromachines);
    virtual void run() override;
    virtual void stop() override;
    ~LookForDeadClientsTh() = default;
};

class AcceptorTh: public Thread {
    private:
    Socket skt;
    bool keep_accepting;
    Micromachines &micromachines;
    ClientList clients;
    std::vector<Socket*> sockets;
    std::map<ColorType, Car*> cars;
    LookForDeadClientsTh *looking_th;

    void deleteResources();

    public:
    AcceptorTh(const char *service, Micromachines &micromachines);
    virtual void run() override;
    virtual void stop() override;
    ~AcceptorTh();
};

#endif
