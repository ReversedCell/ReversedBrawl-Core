#include "../../inc/protocol/ClientHelloMessage.hpp"

ClientHelloMessage::ClientHelloMessage() {
    this->fingerprintSha = new std::string();

    protocol = 0;
    keyVersion = 0;
    clientMajor = 0;
    clientMinor = 0;
    clientBuild = 0;
    deviceType = 0;
    appStore = 0;
}

ClientHelloMessage::~ClientHelloMessage() {
    delete this->fingerprintSha;
}

void ClientHelloMessage::encode() {
    stream->writeInt(protocol);
    stream->writeInt(keyVersion);
    stream->writeInt(clientMajor);
    stream->writeInt(clientMinor);
    stream->writeInt(clientBuild);
    stream->writeString(fingerprintSha);
    stream->writeInt(deviceType);
    stream->writeInt(appStore);
}

void ClientHelloMessage::decode() {
    protocol = stream->readInt();
    keyVersion = stream->readInt();
    clientMajor = stream->readInt();
    clientMinor = stream->readInt();
    clientBuild = stream->readInt();
    stream->readString(fingerprintSha);
    deviceType = stream->readInt();
    appStore = stream->readInt();
}

short ClientHelloMessage::getMessageType() {
    return 10100;
}
