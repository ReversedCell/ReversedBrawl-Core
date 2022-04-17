#include "../inc/TCPServerSocket.hpp"
#include "../inc/util/Debugger.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <pthread.h>
#include <string.h>
#include <unistd.h>


int TCPServerSocket::listener; // need to define it.
int TCPServerSocket::max_fd;
std::map<int, Connection*>* TCPServerSocket::connections;

void TCPServerSocket::init(int port) {
    TCPServerSocket::connections = new std::map<int, Connection*>();

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    TCPServerSocket::listener = fd;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

    if (bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        Debugger::error("Bind failed! Is address already in use?");
        exit(EXIT_FAILURE);
    }

    listen(fd, 100);

    TCPServerSocket::startAccept();
}

void TCPServerSocket::startAccept() {
    pthread_t thrd;
    pthread_create(&thrd, NULL, TCPServerSocket::update, NULL);
    pthread_detach(thrd);
}

void* TCPServerSocket::update(void* args) {
    int listener = TCPServerSocket::listener;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(listener, &master);
    max_fd = listener;

    while (true) {
        fd_set set = master;
        int nfds = select(max_fd+1, &set, NULL, NULL, NULL);
        if (nfds <= 0) continue;
        for (int fd = 0; fd <= max_fd; ++fd) {
            if (!FD_ISSET(fd, &set))
                continue;

            if (fd == listener) {
                TCPServerSocket::onAccept(fd, &master);
            }
            else {
                TCPServerSocket::onReceive(fd, &master);
            }
        }
    }

    return NULL;
}

void TCPServerSocket::onAccept(int fd, fd_set* master) {
    int clientfd = accept(fd, NULL, NULL);

    if (clientfd == -1) return;

    FD_SET(clientfd, master);
    if (max_fd < clientfd) max_fd = clientfd;
    
    Connection* connection = new Connection(clientfd);
    TCPServerSocket::connections->emplace(clientfd, connection);

    printf("New connection!\n");
}

void TCPServerSocket::onReceive(int fd, fd_set* master) {
    auto found = connections->find(fd);
    if (found == connections->end()) {
        printf("Unknown connection.\n");
        return;
    }
    Connection* connection = found->second;
    if (connection->getMessaging()->onReceive() == -1) {
        printf("Disconnected!\n");
        FD_CLR(fd, master);
        connections->erase(fd);
        connection->disconnect();
        delete connection;
    }
}