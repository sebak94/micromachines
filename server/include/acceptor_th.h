#ifndef __ACCEPTOR_TH_H__
#define __ACCEPTOR_TH_H__

#include "../../common/include/thread.h"
#include "../../common/include/socket.h"

class AcceptorTh: public Thread {
    private:
    Socket skt;
    bool keep_accepting;

    public:
    AcceptorTh(const char *service);
    virtual void run() override;
    void stop();
    ~AcceptorTh();
};

#endif
