#ifndef MESSAGING_HPP
#define MESSAGING_HPP

#define HEADER_SIZE 7

#include "../inc/StreamEncrypter.hpp"
#include "protocol/PiranhaMessage.hpp"

class Connection;

class Messaging {
    private:
    Connection* connection;
    StreamEncrypter* encrypter;
    StreamEncrypter* decrypter;
    int state;
    public:
    Messaging(Connection* connection);
    ~Messaging();
    Connection* getConnection();
    void send(PiranhaMessage* message);
    void encryptAndWrite(PiranhaMessage* message);
    int onReceive();
    bool isConnected();
    void disconnect();

    static void writeHeader(char* buffer, PiranhaMessage* message, int length);
};
#include "Connection.hpp"

#endif