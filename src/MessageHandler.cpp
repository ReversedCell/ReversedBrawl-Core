#include "../inc/MessageHandler.hpp"

pthread_cond_t* MessageHandler::sendCondition;
pthread_cond_t* MessageHandler::recvCondition;
pthread_mutex_t* MessageHandler::sendMutex;
pthread_mutex_t* MessageHandler::recvMutex;
std::queue<QueueItem*>* MessageHandler::outgoingMessagesQueue;
std::queue<QueueItem*>* MessageHandler::incomingMessagesQueue;
pthread_t* MessageHandler::sendThread;
pthread_t* MessageHandler::recvThread;

void MessageHandler::init() {
    sendCondition = new pthread_cond_t(PTHREAD_COND_INITIALIZER);
    recvCondition = new pthread_cond_t(PTHREAD_COND_INITIALIZER);

    sendMutex = new pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER);
    recvMutex = new pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER);

    outgoingMessagesQueue = new std::queue<QueueItem*>();
    incomingMessagesQueue = new std::queue<QueueItem*>();

    sendThread = new pthread_t();
    recvThread = new pthread_t();

    pthread_create(sendThread, NULL, MessageHandler::updateSend, NULL);
    pthread_detach(*sendThread);

    pthread_create(recvThread, NULL, MessageHandler::updateRecv, NULL);
    pthread_detach(*sendThread);
}

void MessageHandler::enqueueSend(Connection* connection, PiranhaMessage* message) {
    QueueItem* item = new QueueItem();
    item->connection = connection;
    item->message = message;
    outgoingMessagesQueue->push(item);
    pthread_cond_signal(sendCondition);
}

void MessageHandler::enqueueReceive(Connection* connection, PiranhaMessage* message) {
    QueueItem* item = new QueueItem();
    item->connection = connection;
    item->message = message;
    incomingMessagesQueue->push(item);
    pthread_cond_signal(recvCondition);
}

void* MessageHandler::updateSend(void* args) {
    pthread_mutex_lock(sendMutex);
    while (true) {
        pthread_cond_wait(sendCondition, sendMutex);
        pthread_mutex_unlock(sendMutex);
        while (outgoingMessagesQueue->size()) {
            QueueItem* item = outgoingMessagesQueue->front();
            outgoingMessagesQueue->pop();

            item->connection->getMessaging()->encryptAndWrite(item->message);
            delete item->message;
            delete item;
        }
        pthread_mutex_lock(sendMutex);
    }
}

void* MessageHandler::updateRecv(void* args) {
    pthread_mutex_lock(recvMutex);
    while (true) {
        pthread_cond_wait(recvCondition, recvMutex);
        pthread_mutex_unlock(recvMutex);
        while (incomingMessagesQueue->size()) {
            QueueItem* item = incomingMessagesQueue->front();
            incomingMessagesQueue->pop();

            item->connection->getMessageManager()->receiveMessage(item->message);
            delete item->message;
            delete item;
        }
        pthread_mutex_lock(recvMutex);
    }
}