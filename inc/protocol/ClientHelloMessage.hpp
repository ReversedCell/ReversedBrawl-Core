#ifndef CLIENTHELLO_HPP
#define CLIENTHELLO_HPP
#include "PiranhaMessage.hpp"

class ClientHelloMessage : public PiranhaMessage {
    public:
    int protocol;
    int keyVersion;
    int clientMajor;
    int clientMinor;
    int clientBuild;
    std::string* fingerprintSha;
    int deviceType;
    int appStore;

    ClientHelloMessage();
    ~ClientHelloMessage();
    void encode() override;
    void decode() override;
    short getMessageType() override;
};

#endif