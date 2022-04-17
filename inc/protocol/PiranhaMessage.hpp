#ifndef PIRANHAMESSAGE_HPP
#define PIRANHAMESSAGE_HPP
#include "../util/ByteStream.hpp"

class PiranhaMessage {
    protected:
    short version;
    ByteStream* stream;
    public:
    PiranhaMessage();
    ~PiranhaMessage();
    virtual void encode() = 0;
    virtual void decode() = 0;
    virtual short getMessageType() = 0;

    ByteStream* getByteStream();
    char* getMessageBytes();
    int getEncodingLength();
    short getMessageVersion();
    void setMessageVersion(short version);
    bool isClientToServerMessage();
    bool isServerToClientMessage();
};
#endif