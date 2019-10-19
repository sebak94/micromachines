#ifndef __ACCEPTOR_TH_H__
#define __ACCEPTOR_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"
#include "./micromachines.h"

class AcceptorTh: public Thread {
    private:
    Socket skt;
    bool keep_accepting;
    Micromachines micromachines;

    public:
    AcceptorTh(const char *service, Micromachines micromachines);
    virtual void run() override;
    virtual void stop() override;
    ~AcceptorTh();
};

#endif
