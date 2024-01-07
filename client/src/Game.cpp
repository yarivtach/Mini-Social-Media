#include "../include/Game.h"
#include <sstream>
#include <iostream>
using namespace std;

Game::Game(): GameName(), Team_A_Updates(), Team_B_Updates(), events_info(){}

vector<std::string> Game::splitString2(string str, char delimiter) {
    vector<string> substrings;
    std::istringstream ss(str);
    std::string token;
    while(std::getline(ss, token, delimiter)) {
        substrings.push_back(token);
    }
    return substrings;
}

Game::Game(string TopicName): GameName(TopicName), Team_A_Updates(), Team_B_Updates(), events_info(){}

void Game::AddTeamAUpdate(string line){
    vector<string> Splitter = splitString2(line, ':');
    Team_A_Updates[Splitter[0]] = Splitter[1];
}

void Game::AddTeamBUpdate(string line){
    vector<string> Splitter = splitString2(line, ':');
    Team_B_Updates[Splitter[0]] = Splitter[1];
}

void Game::AddEventInfo(string Time, string EventName, string Description){
    string ans = (Time + " - " + EventName + "\n\n" +
                  Description + "\n");
                //   cout << ans << endl;
    events_info.push_back(ans);
}

void Game::addReport(vector<string> Splitter){
    char Section;
    string Des = "";
    string Time;
    string EventName;
    int size = Splitter.size();
    for (int i = 0; i < size; i++)
    {
        if(Splitter[i].empty()){continue;}
        else if(Splitter[i].find("team a updates:") != string::npos){Section = 'A';}
        else if(Splitter[i].find("team b updates:") != string::npos){Section = 'B';}
        else if(Splitter[i].find("--------------------") != string::npos){Section = 'C';}
        else if(Splitter[i].find("description:") != string::npos){Section = 'D';}
        else if(Splitter[i].find("event name:") != string::npos){
            EventName = Splitter[i].substr(Splitter[i].find(":") + 2, Splitter[i].size()-Splitter[i].find(":")-1);
            Section = 'E';
            }
        else if(Splitter[i].find("time: ") != string::npos){
            Time = Splitter[i].substr(Splitter[i].find(":") + 2, Splitter[i].size()-Splitter[i].find(":")-1);
            Section = 'E';
        }
        else if(Section == 'A'){
            if(!Splitter[i].empty()){
                AddTeamAUpdate(Splitter[i]);
            }
        }
        else if(Section == 'B'){
            if(!Splitter[i].empty()){
                AddTeamBUpdate(Splitter[i]);
            }
        }
        else if(Section == 'D'){
            Des = Des + Splitter[i] + "\n";
        }
        if(Section == 'C'){
            // cout<< Des << endl;
            AddEventInfo(Time, EventName, Des);
            Section = 'E';
            Des = "";
        }
    }
}

unordered_map<string,string> Game::getTAU(){
    return Team_A_Updates;
}

unordered_map<string,string> Game::getTBU(){
    return Team_B_Updates;
}

vector<string> Game::getEventInfo(){
    return events_info;
}

string Game::toStringUpdates(unordered_map<string,string> Updates){
    string ans = "";
    for (const auto& [key, value] : Updates) {
        ans = ans + key + ":" + value + "\n";  
    }
    return ans;
}

string Game::toStringEvents(){
    string ans = "";
    for(string& S : events_info){
        ans = ans + S + "\n\n\n";
    }
    return ans;
}

string Game::toString(){
    // cout << "in game to string" << endl;
    vector<string> Splitter = splitString2(GameName, '_');
    string title = Splitter[0] + " vs " + Splitter[1];
    // cout << "title: \n" << title << endl;
    string TeamATitle = Splitter[0] + " stats:";
    string TeamAUpdates = toStringUpdates(Team_A_Updates);
    // cout << "TeamATitle: \n" << TeamAUpdates << endl;
    string TeamBTitle = Splitter[1] + " stats:";
    string TeamBUpdates = toStringUpdates(Team_B_Updates);
    // cout << "TeamBTitle: \n" << TeamBUpdates << endl;
    string EventTitle = "Game event reports:";
    string EventsDescription = toStringEvents();
    // cout << "Game event reports: \n" << EventsDescription << endl;
    string ans = (title + "\n\n" +
                  TeamATitle + "\n" +
                  TeamAUpdates + "\n\n" + 
                  TeamBTitle + "\n" +
                  TeamBUpdates + "\n\n" +
                  EventTitle + "\n" +
                  EventsDescription);
    return ans;
}
