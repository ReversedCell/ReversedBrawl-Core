#include "../../inc/protocol/ServerHelloMessage.hpp"
#include <string.h>

ServerHelloMessage::ServerHelloMessage() {
    this->sessionToken = NULL;
    this->sessionTokenLength = 0;
}

ServerHelloMessage::~ServerHelloMessage() {
    if (this->sessionToken) {
        delete[] this->sessionToken;
    }
}

void ServerHelloMessage::encode() {
    stream->writeBytes(this->sessionToken, this->sessionTokenLength);
}

void ServerHelloMessage::decode() {
    this->sessionTokenLength = stream->readInt();
    this->sessionToken = stream->readBytes(this->sessionTokenLength, 1000);
}

short ServerHelloMessage::getMessageType() {
    return 20100;
}

char* ServerHelloMessage::removeServerHelloToken() {
    char* token = this->sessionToken;
    this->sessionToken = NULL;
    this->sessionTokenLength = 0;
    return token;
}

void ServerHelloMessage::setServerHelloToken(char* sessionToken, int length) {
    if (this->sessionToken) {
        delete[] this->sessionToken;
    }
    this->sessionToken = NULL;

    if (sessionToken) {
        this->sessionToken = new char[length];
        memcpy(this->sessionToken, sessionToken, length);
    }
    this->sessionTokenLength = length;
}
