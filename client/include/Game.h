#pragma once
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Game{

    private:
        string GameName;
        unordered_map<string,string> Team_A_Updates;
        unordered_map<string,string> Team_B_Updates;
        vector<string> events_info;
    public:
        Game();
        Game(string TopicName);
        vector<std::string> splitString2(string str, char delimiter);
        void AddTeamAUpdate(string line);
        void AddTeamBUpdate(string line);
        void AddDescription(string string);
        unordered_map<string,string> getTAU();
        unordered_map<string,string> getTBU();
        void AddEventInfo(string Time, string EventName, string Description);
        vector<string> getEventInfo();
        void addReport(vector<string> Report);
        string toStringUpdates(unordered_map<string,string> Updates);
        string toStringEvents();
        string toString();
        // void AddSectionToA(string Section);
        // void AddSectionToB(string Section);

};