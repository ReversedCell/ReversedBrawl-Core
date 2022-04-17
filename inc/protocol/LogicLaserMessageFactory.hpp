#ifndef MESSAGEFACTORY_HPP
#define MESSAGEFACTORY_HPP
#include "PiranhaMessage.hpp"

class LogicLaserMessageFactory {
    public:
    static PiranhaMessage* createMessageByType(short type);
};

#endif