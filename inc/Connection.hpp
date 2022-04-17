#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include "Messaging.hpp"
#include "protocol/MessageManager.hpp"
class Connection {
    private:
    int fd;
    Messaging* messaging;
    MessageManager* messageManager;
    bool open;
    public:
    Connection(int fd);
    ~Connection();
    bool isOpen();
    void writeBlocking(char* buf, int size);
    int readBlocking(char* buf, int size);
    void disconnect();
    int getSocket();
    Messaging* getMessaging();
    MessageManager* getMessageManager();
};


#endif