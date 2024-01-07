#pragma once
#include <unordered_map>
#include <string>

using namespace std;
class User{

    private:
        string UserName;
        unordered_map<string, int> TopicToSubMap;
        unordered_map<string, string> ReceiptIdToGameNameMap;
        
    public:
        User(string Uname);
        User();
        virtual ~User();
        string getUserName();
        void setUserName(string Uname);
        void ClearMaps();
        unordered_map<string,int> getTopicToSubMap();
        void addNewTopicSub(string topic, int sub);
        unordered_map<string,string> getReceiptIdToMap();
        void addNewTopicSub(int ReceiptId, string GameName);
        int getSubIdByTopic(string TopicName);
        string getGameNameByReceiptId(string ReceiptId);
        void addJoinReceiptToGame(string ReceiptId, string GameName);
        void addExitReceiptToGame(string ReceiptId, string GameName);
        void RemoveReceiptAndPrint(string ReceiptId);
        void addLogOutReceipt(string ReceiptId);
};