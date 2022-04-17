#include <iostream>
#include "../inc/TCPServerSocket.hpp"
#include "../inc/MessageHandler.hpp"

int main(int argc, const char** argv) {
    std::cout << "Starting server..." << std::endl;
    MessageHandler::init();
    TCPServerSocket::init(9339);

    std::cout << "Server started! Let's play Brawl Stars!" << std::endl;
    std::cin.get();
    return 0;
}