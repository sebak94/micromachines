#ifndef __ACCEPTOR_TH_H__
#define __ACCEPTOR_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "../../common/include/lock.h"
#include "model/micromachines_th.h"
#include "../include/client_th.h"
#include "vector"
#include "games_th.h"
#include <mutex>

#define DEADCLIENTSTIMEPERIOD 1000000

class ClientList {
    private:
    std::mutex m;
    std::vector<ClientTh*> clients;
    GamesTh &games;

    public:
    ClientList(GamesTh &games);
    void addClient(ClientTh *client);
    void deleteDeadClients();
    ~ClientList();
};

class LookForDeadClientsTh: public Thread {
    private:
    bool keep_looking;
    ClientList &clients;
    GamesTh &games;

    public:
    LookForDeadClientsTh(ClientList &clients, GamesTh &games);
    void run() override;
    void stop() override;
    ~LookForDeadClientsTh() = default;
};

class AcceptorTh: public Thread {
    private:
    Socket skt;
    bool keep_accepting;
    GamesTh &games;
    ClientList clients;
    std::vector<Socket*> sockets;
    LookForDeadClientsTh *looking_th;

    void deleteResources();

    public:
    AcceptorTh(const char *service, GamesTh &games);
    virtual void run() override;
    virtual void stop() override;
    ~AcceptorTh();
};

#endif
