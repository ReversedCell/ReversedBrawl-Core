#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <queue>
#include "Connection.hpp"
#include "protocol/PiranhaMessage.hpp"

typedef struct QueueItem_t {
    Connection* connection;
    PiranhaMessage* message;
} QueueItem;

class MessageHandler {
    public:
    static void init();
    static void enqueueSend(Connection* connection, PiranhaMessage* message);
    static void enqueueReceive(Connection* connection, PiranhaMessage* message);
    private:
    static pthread_cond_t* sendCondition;
    static pthread_cond_t* recvCondition;
    static pthread_mutex_t* sendMutex;
    static pthread_mutex_t* recvMutex;
    static std::queue<QueueItem*>* outgoingMessagesQueue;
    static std::queue<QueueItem*>* incomingMessagesQueue;
    static pthread_t* sendThread;
    static pthread_t* recvThread;
    static void* updateSend(void* args);
    static void* updateRecv(void* args);
};

#endif