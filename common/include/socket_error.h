#ifndef __SOCKET_ERROR_H__
#define __SOCKET_ERROR_H__

#include <typeinfo>

#define BUF_LEN 256

class SocketError: public std::exception {
    private:
    char error_msg[BUF_LEN];

    public:
    explicit SocketError(const char* fmt, ...) noexcept;
    virtual const char* what() const noexcept;
    virtual ~SocketError() noexcept {}
};

#endif
