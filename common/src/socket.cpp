#define _POSIX_C_SOURCE 200112L
#include "../include/socket.h"
#include "../include/socket_error.h"
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define INVALID_FD -1

Socket::Socket() {
	fd = INVALID_FD;
}

void Socket::BindAndListen(const char *service) {
	int status;

	struct addrinfo hints;
	struct addrinfo *result;

	int val = 1;

    memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(0, service, &hints, &result);
	if (status != 0) { 
		throw SocketError("Error al invocar getaddrinfo. ", gai_strerror(status));
	}

	fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (fd == INVALID_FD) {
		freeaddrinfo(result);
		throw SocketError("Error al crear el socket. ", gai_strerror(fd));
	}

	status = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	if (status == INVALID_FD) {
		close(fd);
		freeaddrinfo(result);
		throw SocketError("Error al invocar setsockopt. ", gai_strerror(status));
	}

	status = bind(fd, result->ai_addr, result->ai_addrlen);
	if (status == INVALID_FD) {
		close(fd);
		freeaddrinfo(result);
		throw SocketError("Error al hacer el bind. ", gai_strerror(status));
	}

	freeaddrinfo(result);

	status = listen(fd, 1);
	if (status == INVALID_FD) {
		close(fd);
		throw SocketError("Error al hacer el listen. ", gai_strerror(status));
	}
}

void Socket::Connect(const char *hostname, const char *service) {
	int status;
	bool are_we_connected = false;

	struct addrinfo hints;
	struct addrinfo *result, *ptr;

    memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;

	status = getaddrinfo(hostname, service, &hints, &result);

	if (status != 0) {
		throw SocketError("Error al invocar getaddrinfo. ", gai_strerror(status));
	}

	for (ptr = result; ptr != NULL && !are_we_connected; ptr = ptr->ai_next) {
		fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (fd != INVALID_FD) {
			status = connect(fd, ptr->ai_addr, ptr->ai_addrlen);
			if (status == INVALID_FD) {
				close(fd);
			}
			are_we_connected = status != -1;
		}
	}

	freeaddrinfo(result);

	if (are_we_connected == false) {
		throw SocketError("Error al conectar. ", gai_strerror(fd));
	}
}

void Socket::Accept(Socket *peerskt) {
	peerskt->fd = accept(fd, NULL, NULL);
	if (peerskt->fd == INVALID_FD) {
		throw SocketError("Error al aceptar nueva conexión. ", gai_strerror(peerskt->fd));
	}
}

int Socket::Send(const char *buf, size_t length) {
	size_t bytes_sent = 0;

    while (bytes_sent < length) {
		int status = send(fd, &buf[bytes_sent], length - bytes_sent, MSG_NOSIGNAL);

		if (status == INVALID_FD) {
        	throw SocketError("Error al enviar los bytes. ",
				gai_strerror(status));
		}

		if (status == 0) {
			throw SocketError("Conexión cerrada. ", gai_strerror(status));
		}
        
		bytes_sent += status;
	}

    return bytes_sent;
}

int Socket::Receive(char *buf, size_t length) {
	size_t bytes_recv = 0;

    while (bytes_recv < length) {
		int status = recv(fd, &buf[bytes_recv], length - bytes_recv, 0);

		if (status == INVALID_FD) {
        	throw SocketError("Error al recibir los bytes. ",
				gai_strerror(status));
		}

		if (status == 0) {
			throw SocketError("Conexión cerrada. ", gai_strerror(status));
		} else {
         	bytes_recv += status;
      	}
	}

    return bytes_recv;
}

void Socket::Release() {
	if (fd != INVALID_FD) {
		shutdown(fd, 2);
		close(fd);
	}
}

Socket::~Socket() {
	Release();
}
