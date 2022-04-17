#ifndef MESSAGEMANAGER_HPP
#define MESSAGEMANAGER_HPP

#include "PiranhaMessage.hpp"
#include "ClientHelloMessage.hpp"
#include "ServerHelloMessage.hpp"

class Connection;
class MessageManager {
    public:
    MessageManager(Connection* connection);
    void receiveMessage(PiranhaMessage* message);
    private:
    Connection* connection;
    void clientHelloReceived(ClientHelloMessage* message);
};

#include "../Connection.hpp"

#endif