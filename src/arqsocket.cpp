#include <cstring>
#include <stdlib.h>
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

		cleanup_winsock();
        throw arqanore::ArqanoreException("getaddrinfo(): " + err_str);
    }

    return addrinfo;
}

void arqanore::ArqSocket::init_winsock() {
#ifdef _WIN32
	WSADATA wsaData;
	
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	if (result != 0) {
		throw arqanore::ArqanoreException("init_winsock(): " + std::to_string(result));
	}
#endif
}

void arqanore::ArqSocket::cleanup_winsock() {
#ifdef _WIN32
	WSACleanup();
#endif
}

void arqanore::ArqSocket::handle_errno(std::string function) {
#ifdef _WIN32
	int err_code = WSAGetLastError();
	std::string err_str;
	
	if (err_code == WSAECONNREFUSED) {
		err_str = "Connection refused";
	} else {
		std::to_string(err_code);
	}

	cleanup_winsock();
	throw arqanore::ArqanoreException(function + "(): " + err_str);
#else
	auto err_cstr = strerror(errno);
	auto err_str = std::string(err_cstr);

	throw arqanore::ArqanoreException(function + "(): " + err_str);
#endif
}

arqanore::ArqSocket::ArqSocket() {
    handler = INVALID_SOCKET;
	
	this->init_winsock();
}

void arqanore::ArqSocket::set_rcv_timeout(long msec) {
	int result = 0;
	
#ifdef _WIN32
    result = setsockopt(handler, SOL_SOCKET, SO_RCVTIMEO, (char *) &msec, sizeof(msec));
#else
	struct timeval timeout;
	timeout.tv_sec = msec / 1000;
	timeout.tv_usec = (msec * 1000) % 1000000;
	
	result = setsockopt(handler, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
#endif

	if (result == -1) {
		handle_errno("setsockopt");
	}
}

void arqanore::ArqSocket::set_send_timeout(long msec) {
	int result = 0;
	
#ifdef _WIN32
    result = setsockopt(handler, SOL_SOCKET, SO_SNDTIMEO, (char *) &msec, sizeof(msec));
#else
	struct timeval timeout;
	timeout.tv_sec = msec / 1000;
	timeout.tv_usec = (msec * 1000) % 1000000;
	
	result = setsockopt(handler, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
#endif

	if (result == -1) {
		handle_errno("setsockopt");
	}
}

void arqanore::ArqSocket::connect(std::string host, int port) {
    auto addrinfo = get_addrinfo(host, port);
    auto socket = ::socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);

    if (socket == INVALID_SOCKET) {
		handle_errno("socket");
    }

    auto result = ::connect(socket, addrinfo->ai_addr, addrinfo->ai_addrlen);

    if (result == SOCKET_ERROR) {
		handle_errno("connect");
    }

    this->handler = socket;
}

int arqanore::ArqSocket::send(std::string data) {
    auto data_cstr = data.c_str();
    auto data_len = data.length();
    auto result = ::send(handler, data_cstr, data_len, 0);

    if (result == -1) {
		handle_errno("send");
    }

    return result;
}

int arqanore::ArqSocket::read(char *buffer, int buffer_length) {
    auto result = ::recv(handler, buffer, buffer_length - 1, 0);

    if (result == -1) {
        handle_errno("read");
    }

    return result;
}

void arqanore::ArqSocket::close() {
#ifdef _WIN32
	closesocket(handler);
	cleanup_winsock();
#else
	auto result = ::close(handler);

    if (result == -1) {
        handle_errno("close");
    }
#endif
}
