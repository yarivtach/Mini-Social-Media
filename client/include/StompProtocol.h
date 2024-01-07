#pragma once
#include "../include/ConnectionHandler.h"
#include "ConnectionHandler.h"
#include "User.h"
#include "Summary.h"
#include <string>
#include <vector>
class Frame;
// TODO: implement the STOMP protocol
class StompProtocol
{

private:
    User* ClientUser;
    bool connected;
    int SubCounter;
    int ReceiptCounter;
    ConnectionHandler* CH;
    bool shouldTerminate;
    Summary GamesSummary;

public:
    StompProtocol();
    virtual ~StompProtocol();
    void processSend();
    vector<std::string> splitString4(string str, char delimiter);
    string ProcessAndBuildSend(vector<string> msg);
    void processGet();
    void ProcessAndBuildGet(vector<string> msg);
    void UserConnect(string _UserName);
    void UserConnected(string _UserName);
    string getUserName();
    int getSubId();
    void IncSubCounter();
    int getSubIdAndInc();
    int getReceiptId();
    void IncReceiptCounter();
    int getReceiptIdAndInc();
    void deleteUser();
    // ConnectionHandler* getCH();
    void setCH(string host, short port);
    Summary getGamesSummary();
    void ProcessSummary(vector<string> msg);
    void writeFile(string json, string path);
    string getErrorDesc(vector<string> msg);
    void start();
};
