#pragma once

// Windows
#ifdef _WIN32
#pragma comment(lib,"ws2_32.lib")
#define WIN32_LEAN_AND_MEAN
#undef TEXT
#include <winsock2.h>
#include <ws2tcpip.h>

// Linux
#else
typedef int SOCKET;

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

static const int INVALID_SOCKET = -1;
static const int SOCKET_ERROR = -1;
#endif

#include "arqanore/exceptions.h"

namespace arqanore {
    class ArqSocket {
    private:
        SOCKET handler;
        struct addrinfo *get_addrinfo(std::string host, int port);

    public:
        ArqSocket();

        void connect(std::string host, int port);

        int send(std::string data);

        int read(char buffer[], int buffer_length);

        void close();
    };
}