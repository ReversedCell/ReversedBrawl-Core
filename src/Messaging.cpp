#include "../inc/Messaging.hpp"
#include "../inc/protocol/PiranhaMessage.hpp"
#include "../inc/protocol/LogicLaserMessageFactory.hpp"
#include "../inc/MessageHandler.hpp"
#include "../inc/util/Debugger.hpp"
#include <iostream>
#include <string.h>

Messaging::Messaging(Connection* connection) {
    this->connection = connection;
    this->state = 2;
    this->decrypter = NULL;
    this->encrypter = NULL;
}

Messaging::~Messaging() {
    this->state = 0;
    if (this->encrypter) {
        delete encrypter;
    }
    if (this->decrypter) {
        delete decrypter;
    }
    // don't destruct Connection, since Messaging destructor is called already in Connection destructor
}

Connection* Messaging::getConnection() {
    return this->connection;
}

void Messaging::send(PiranhaMessage* message) {
    if (message->isClientToServerMessage()) {
        Debugger::warning("Messaging: Messaging::send client to server message type " + message->getMessageType());
        return;
    }
    MessageHandler::enqueueSend(connection, message);
}

void Messaging::encryptAndWrite(PiranhaMessage* message) {
    if (message->getEncodingLength() == 0) message->encode();
    int length = message->getEncodingLength();
    char* payload = message->getMessageBytes();

    if (this->encrypter && this->state == 5) {
        int newLength = length + encrypter->getEncryptionOverhead();
        char* encrypted = new char[newLength];
        encrypter->encrypt(payload, encrypted, length);
        length = newLength;
        payload = encrypted;
    }

    char* buffer = new char[length + HEADER_SIZE];

    Messaging::writeHeader(buffer, message, length);
    memcpy(buffer + HEADER_SIZE, payload, length);
    this->connection->writeBlocking(buffer, length + HEADER_SIZE);
    delete[] buffer;
}

int Messaging::onReceive() {
    char* header = new char[HEADER_SIZE];
    int r = connection->readBlocking(header, HEADER_SIZE);

    if (r <= 0) {
        return -1;
    }

    unsigned short type = (header[0] << 8 | header[1]);
    unsigned int length = header[2] << 16 | header[3] << 8 | header[4] & 0xFF;
    unsigned short version = (header[5] << 8 | header[6]);
    delete[] header;
    printf("Received Message: %d, length: %d, version: %d\n", type, length, version);

    char* payload = new char[length];
    int result = this->connection->readBlocking(payload, length);
    if (result <= 0) {
        std::string str = "Messaging: Messaging::onReceive Read failed. socket: " + std::to_string(connection->getSocket()) + ", ret " + std::to_string(result);
        Debugger::warning(str.c_str());
        return -1;
    }

    if (this->decrypter && this->state == 5) {
        char* encrypted = payload;
        payload = new char[length + this->decrypter->getEncryptionOverhead()];
        this->decrypter->decrypt(encrypted, payload, length);
        length += this->decrypter->getEncryptionOverhead();
        delete[] encrypted;
    }

    if (type == 10100) {
        this->state = 3;
    }
    else if (type == 10101) {
        this->state = 4;
        // Messaging::handlePepperLogin
    }

    PiranhaMessage* message = LogicLaserMessageFactory::createMessageByType(type);
    if (message) {
        message->getByteStream()->setByteArray(payload, length);
        message->decode();
        MessageHandler::enqueueReceive(connection, message);
    }
    else {
        std::string str = "Ignoring message of unknown type " + std::to_string(type);
        Debugger::warning(str.c_str());
        delete[] payload;
    }
    return 0;
}

bool Messaging::isConnected() {
    return this->connection->isOpen();
}

void Messaging::disconnect() {
    this->state = 0;
    this->connection->disconnect();
}

void Messaging::writeHeader(char* buffer, PiranhaMessage* message, int length) {
    short messageType = message->getMessageType();
    short messageVersion = message->getMessageVersion();

    buffer[0] = (char)(messageType >> 8);
    buffer[1] = (char)(messageType);
    buffer[2] = (char)(length >> 16);
    buffer[3] = (char)(length >> 8);
    buffer[4] = (char)(length);
    buffer[5] = (char)(messageVersion >> 8);
    buffer[6] = (char)(messageVersion);

    if (length > 0xFFFFFF) {
        Debugger::error("Trying to send too big message, type " + messageType);
    }
}