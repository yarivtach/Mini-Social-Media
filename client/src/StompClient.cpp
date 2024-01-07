#include <stdlib.h>
#include "../include/ConnectionHandler.h"
#include "../include/StompProtocol.h"
#include <iostream>
#include <mutex>
#include <thread>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/

static vector<std::string> splitString(string str, char delimiter) {
    vector<string> substrings;
    std::istringstream ss(str);
    std::string token;
    while(std::getline(ss, token, delimiter)) {
        substrings.push_back(token);
    }
    return substrings;
}

int main (int argc, char *argv[]) {
    while(1){
        StompProtocol* protocol = new StompProtocol();
        protocol->start();
        std::thread readerSocket(&StompProtocol::processGet , &(*protocol));
        std::thread readerKeyboard(&StompProtocol::processSend , &(*protocol));
        readerKeyboard.join();
        readerSocket.join();
        delete protocol;
    }
}