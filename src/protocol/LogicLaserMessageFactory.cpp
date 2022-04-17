#include "../../inc/protocol/LogicLaserMessageFactory.hpp"
#include "../../inc/protocol/ClientHelloMessage.hpp"

PiranhaMessage* LogicLaserMessageFactory::createMessageByType(short type) {
    switch (type) {
        case 10100:
            return new ClientHelloMessage();
        default:
            return NULL;
    }

    return NULL;
}