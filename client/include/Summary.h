#pragma once
#include <string>
#include <unordered_map>
#include "Game.h"
using namespace std;

class Summary{

    private:
        unordered_map<string,unordered_map<string,Game>> SummaryMap;

    public:
        Summary();
        void printSummaryMap();
        unordered_map<string,unordered_map<string,Game>> getSummaryMap();
        vector<std::string> splitString4(string str, char delimiter);
        Game& getGame(string UserName, string GameName);
        void UserReport(string Report);
        void AddToMap(string GameName, string UserName, Game G);
        string toString(string GameName, string UserName);

};