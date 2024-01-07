
#include <string>
#include "../include/User.h"
#include <iostream>
using namespace std;

User::User():UserName(""), TopicToSubMap(), ReceiptIdToGameNameMap(){}

User::User(string Uname): UserName(Uname), TopicToSubMap(), ReceiptIdToGameNameMap(){}

User::~User(){}

string User::getUserName(){
    return UserName;
}

void User::setUserName(string Uname){
    UserName = Uname;
}

unordered_map<string, int> User::getTopicToSubMap(){
    return TopicToSubMap;
}

void User::ClearMaps(){
    TopicToSubMap.clear();
    ReceiptIdToGameNameMap.clear();
}

void User::addNewTopicSub(string topic, int sub){
    TopicToSubMap.insert({topic, sub});
}

int User::getSubIdByTopic(string TopicName){
    return TopicToSubMap[TopicName];
}

unordered_map<string,string> User::getReceiptIdToMap(){
    return ReceiptIdToGameNameMap;
}

void User::addJoinReceiptToGame(string ReceiptId, string GameName){
    string line = "Joined channel " + GameName + "\n";
    ReceiptIdToGameNameMap.insert({ReceiptId, line});
}

void User::addExitReceiptToGame(string ReceiptId, string GameName){
    string line = "Exited channel " + GameName + "\n";
    ReceiptIdToGameNameMap.insert({ReceiptId, line});
}

void User::addLogOutReceipt(string ReceiptId){
    string line = "Disconnected\n";
    ReceiptIdToGameNameMap.insert({ReceiptId, line});
}

string User::getGameNameByReceiptId(string ReceiptId){
    return ReceiptIdToGameNameMap[ReceiptId];
}

void User::RemoveReceiptAndPrint(string line){
    // cout << "ReceiptLine: " << line << endl;
    int place = line.find(":");
    string ReceiptId = line.substr(place + 1);
    std::cout << ReceiptIdToGameNameMap[ReceiptId] << std::endl;
    ReceiptIdToGameNameMap.erase(ReceiptId);
}