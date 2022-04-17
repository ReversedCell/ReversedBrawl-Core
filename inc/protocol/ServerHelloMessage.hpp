#ifndef SERVERHELLO_HPP
#define SERVERHELLO_HPP

#include "PiranhaMessage.hpp"

class ServerHelloMessage : public PiranhaMessage {
    private:
    char* sessionToken;
    int sessionTokenLength;
    public:
    ServerHelloMessage();
    ~ServerHelloMessage();
    void encode() override;
    void decode() override;
    short getMessageType() override;
    char* removeServerHelloToken();
    void setServerHelloToken(char* sessionToken, int length);
};

#endif