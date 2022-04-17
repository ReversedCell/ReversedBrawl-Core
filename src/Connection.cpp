#include "../inc/Connection.hpp"
#include <sys/socket.h>
#include <unistd.h>

Connection::Connection(int fd) {
    this->fd = fd;
    this->open = true;
    this->messaging = new Messaging(this);
    this->messageManager = new MessageManager(this);
}

Connection::~Connection() {
    this->fd = 0;
    this->open = false;
    delete this->messaging;
    delete this->messageManager;
}

bool Connection::isOpen() {
    return this->open;
}

void Connection::writeBlocking(char* buf, int size) {
    if (this->open) {
        send(this->fd, buf, size, MSG_NOSIGNAL);
    }
}

int Connection::readBlocking(char* buf, int size) {
    if (this->open) {
        return recv(this->fd, buf, size, MSG_NOSIGNAL);
    }
    return -1;
}

void Connection::disconnect() {
    this->open = false;
    close(this->fd);
}

int Connection::getSocket() {
    return this->fd;
}

Messaging* Connection::getMessaging() {
    return this->messaging;
}

MessageManager* Connection::getMessageManager() {
    return this->messageManager;
}
