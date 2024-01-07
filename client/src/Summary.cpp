
#include "../include/Summary.h"
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

Summary::Summary(): SummaryMap() {}

void Summary::printSummaryMap() {
    for (auto it = SummaryMap.begin(); it != SummaryMap.end(); it++) {
        cout << it->first << ": {";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            cout << it2->first << ": " << it2->second.toString() << ", ";
        }
        cout << "}" << endl;
    }
}

unordered_map<string,unordered_map<string,Game>> Summary::getSummaryMap(){
    return SummaryMap;
}

vector<std::string> Summary::splitString4(string str, char delimiter) {
    vector<string> substrings;
    std::istringstream ss(str);
    std::string token;
    while(std::getline(ss, token, delimiter)) {
        substrings.push_back(token);
    }
    return substrings;
}

Game& Summary::getGame(string UserName, string GameName) {
    auto it = SummaryMap.find(GameName);
    if (it != SummaryMap.end()) {
        auto it2 = it->second.find(UserName);
        if (it2 != it->second.end()) {
            return it2->second;
        }
    }
    //if not found create a new game and add it to the map
    Game newgame = Game(GameName);
    SummaryMap[GameName].emplace(UserName, newgame);
    return SummaryMap[GameName][UserName];
}

void Summary::AddToMap(string GameName, string UserName, Game G){
    SummaryMap[GameName].emplace(UserName, G);
}

void Summary::UserReport(string Report){
    // cout << "in user report" << endl;
    // cout << Report << endl;
    vector<string> Splitter = splitString4(Report, '\n');
    string UserName = Splitter[8].substr(Splitter[8].find(":") + 2, Splitter[8].size()-Splitter[8].find(":")-1);
    // cout << "UserName" << UserName << endl;
    string TeamA = Splitter[9].substr(Splitter[9].find(":") + 2, Splitter[9].size()-Splitter[9].find(":")-1);
    string TeamB = Splitter[10].substr(Splitter[10].find(":") + 2, Splitter[10].size()-Splitter[10].find(":")-1);
    string GameName = TeamA + "_" + TeamB;
    // cout << "GameName" << GameName << endl;
    // string EventName = Splitter[5].substr(Splitter[5].find(":") + 1, Splitter[5].size()-Splitter[5].find(":")-1);
    // cout << "EventName" << EventName << endl;
    // string EventTime = Splitter[6].substr(Splitter[6].find(":") + 1, Splitter[6].size()-Splitter[6].find(":")-1);
    // cout << "EventTime" << EventTime << endl;
    Game& G = getGame(UserName, GameName);
    // cout << "got game" << endl;
    G.addReport(Splitter);
    // cout << "added report" << endl;
    AddToMap(GameName, UserName, G);
    // cout << "added to map" << endl;
}


string Summary::toString(string TopicName, string UserName){
    auto it = SummaryMap.find(TopicName);
    if (it != SummaryMap.end()) {
        auto it2 = it->second.find(UserName);
        if (it2 != it->second.end()) {
            // cout<< "im gonna print" << endl;
            return it2->second.toString();
        }
    }
    //if not found 
    cout<< "SummaryMap does not contain the key " << TopicName << " for user " << UserName << endl;
    return "";
}