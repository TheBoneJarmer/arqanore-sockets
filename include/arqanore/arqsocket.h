#pragma once

// Windows
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

// Linux
#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef int SOCKET;
static const int INVALID_SOCKET = -1;
static const int SOCKET_ERROR = -1;
#endif

#include "arqanore/exceptions.h"

namespace arqanore {
    class ArqSocket {
    private:
        SOCKET handler;

        struct addrinfo *get_addrinfo(std::string host, int port);

        void init_winsock();

        void cleanup_winsock();

        void handle_error(std::string function);

        void set_read_timeout(int socket, long ms);

        void set_send_timeout(int socket, long ms);

    public:
        int read_timeout;
        int send_timeout;

        ArqSocket();

        void connect(std::string host, int port);

        int send(std::string data);

        int read(char buffer[], int buffer_length);

        void close();
    };
}