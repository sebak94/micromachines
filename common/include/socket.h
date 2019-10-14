#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdio.h>
#include <vector>

// Clase que se encarga de wrappear a la biblioteca sys/socket
class Socket {
    private:
    // Filedescriptor del socket
    int fd;

    public:
    // Inicializa el fd en un valor inválido
    Socket();
    // Crea un socket y se conecta a un servidor con los parámetros pasados
    void Connect(const char *hostname, const char *service);
    // Crea un socket para escuchar conexiones
    void BindAndListen(const char *service);
    // Acepta una conexión y guarda el fd nuevo en el Socket pasado
    void Accept(Socket *peerskt);
    // Envia length bytes almacenados en buf
    int Send(const char *buf, size_t length);
    // Recibe length bytes y los guarda en buf
    int Receive(char *buf, size_t length);
    // Hace el shutdown y el close
    void Release();
    // Libera todos los recursos tomados
    ~Socket();
};

#endif
