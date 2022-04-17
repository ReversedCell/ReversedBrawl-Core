#ifndef TCP_SERVER_SOCKET
#define TCP_SERVER_SOCKET

#include <sys/select.h>
#include <map>
#include "../inc/Connection.hpp"

class TCPServerSocket {
    private:
    static int listener;
    static int max_fd;
    static std::map<int, Connection*>* connections;
    static void* update(void* args);
    static void onAccept(int fd, fd_set* master);
    static void onReceive(int fd, fd_set* master);
    public:
    static void init(int port);
    static void startAccept();
};

#endif