#include "../../inc/protocol/MessageManager.hpp"
#include "../../inc/util/Utils.hpp"
#include <string.h>

MessageManager::MessageManager(Connection* connection) {
    this->connection = connection;
}

void MessageManager::receiveMessage(PiranhaMessage* message) {
    switch (message->getMessageType()) {
        case 10100:
            this->clientHelloReceived((ClientHelloMessage*)message);
            break;
    }
}

void MessageManager::clientHelloReceived(ClientHelloMessage* message) {
    // todo: checks for protocol, key version, client version

    ServerHelloMessage* hello = new ServerHelloMessage();

    char* sessionToken = new char[24];
    memset(sessionToken, 0, 24);
    randombytes(sessionToken, 0, 24);

    hello->setServerHelloToken(sessionToken, 24); // copies sessionToken.
    delete[] sessionToken;

    connection->getMessaging()->send(hello);
}