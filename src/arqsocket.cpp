#include <cstring>
#include "arqanore/arqsocket.h"

struct addrinfo *arqanore::ArqSocket::get_addrinfo(std::string host, int port) {
    struct addrinfo *addrinfo = nullptr;
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string port_str = std::to_string(port);
    const char *port_cstr = port_str.c_str();
    const char *host_cstr = host.c_str();

    int result = getaddrinfo(host_cstr, port_cstr, &hints, &addrinfo);

    if (result != 0) {
        auto err_cstr = gai_strerror(result);
        auto err_str = std::string(err_cstr);

        throw arqanore::ArqanoreException("getaddrinfo(): " + err_str);
    }

    return addrinfo;
}

arqanore::ArqSocket::ArqSocket() {
    handler = INVALID_SOCKET;
}

void arqanore::ArqSocket::connect(std::string host, int port) {
    auto addrinfo = get_addrinfo(host, port);
    auto socket = ::socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);

    if (socket == INVALID_SOCKET) {
        auto err_cstr = strerror(errno);
        auto err_str = std::string(err_cstr);

        throw arqanore::ArqanoreException("socket(): " + err_str);
    }

    auto result = ::connect(socket, addrinfo->ai_addr, addrinfo->ai_addrlen);

    if (result == SOCKET_ERROR) {
        auto err_cstr = strerror(errno);
        auto err_str = std::string(err_cstr);

        throw arqanore::ArqanoreException("connect(): " + err_str);
    }

    this->handler = socket;
}

int arqanore::ArqSocket::send(std::string data) {
    auto data_cstr = data.c_str();
    auto data_len = data.length();
    auto result = ::send(handler, data_cstr, data_len, 0);

    if (result == -1) {
        auto err_cstr = strerror(errno);
        auto err_str = std::string(err_cstr);

        throw arqanore::ArqanoreException("send(): " + err_str);
    }

    return result;
}

int arqanore::ArqSocket::read(char *buffer, int buffer_length) {
    auto result = ::read(handler, buffer, buffer_length - 1);

    if (result == -1) {
        auto err_cstr = strerror(errno);
        auto err_str = std::string(err_cstr);

        throw arqanore::ArqanoreException("result(): " + err_str);
    }

    return result;
}

void arqanore::ArqSocket::close() {
    auto result = ::close(handler);

    if (result == -1) {
        auto err_cstr = strerror(errno);
        auto err_str = std::string(err_cstr);

        throw arqanore::ArqanoreException("close(): " + err_str);
    }
}
