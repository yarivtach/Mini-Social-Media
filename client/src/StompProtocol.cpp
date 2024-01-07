
#include "../include/ConnectionHandler.h"
#include "../include/StompProtocol.h"
#include "../include/User.h"
#include <fstream>
#include "../include/Frame.h"

using namespace std;


void remove_empty_strings_from_beginning(std::vector<std::string>& vec) {
    auto it = vec.begin();
    while (it != vec.end() && it->empty()) {
        it = vec.erase(it);
    }
}


static string toStringVector1(vector<string> vec){
    string output = "";
    for(string& S: vec){
        output = output + "\t" + S + "\n";
    }
    return output;
}

StompProtocol::StompProtocol()
    : ClientUser(new User()),
      connected(false),
      SubCounter(0),
      ReceiptCounter(0),
      CH(nullptr),
      shouldTerminate(false),
      GamesSummary(){}

StompProtocol::~StompProtocol(){
    if(CH){delete CH;}
    if(ClientUser){delete ClientUser;}
}
vector<std::string> StompProtocol::splitString4(string str, char delimiter) {
    vector<string> substrings;
    std::istringstream ss(str);
    std::string token;
    while(std::getline(ss, token, delimiter)) {
        substrings.push_back(token);
    }
    return substrings;
}

// void StompProtocol:: UserConnected(string _UserName){
//     ClientUser.setUserName(_UserName);
// }

string StompProtocol::getUserName(){
    return ClientUser->getUserName();
}

Summary StompProtocol::getGamesSummary(){
    return GamesSummary;
}

void StompProtocol::processSend(){
    while(!shouldTerminate){
        if(!connected){break;}
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string msg(buf);
        // cout << "client read line: " << msg << endl;
        if(msg == ""){continue;}
        vector<string> Splitter = splitString4(msg, ' ');
        if(Splitter[0] == "login"){
            if(ClientUser->getUserName()!= ""){
                std::cout<< "The client is already logged in, log out before trying agains"<<std::endl;
            }
            else{
                string ToSend = ProcessAndBuildSend(Splitter);
                // cout << "client processed this frame: \n " << msg << "\n" << endl;
                try{
                    if(!CH->sendLine(ToSend)){
                        std::cout << "Disconnected. Exiting..." << endl;
                    }
                }catch(...){}
            }
        }
        else if(Splitter[0] == "summary"){
            // cout << "got summery command" << endl;
            ProcessSummary(Splitter);
        }
        else{
            string ToSend = ProcessAndBuildSend(Splitter);
            // cout << "sent frame: \n" << ToSend << endl;
            // std::cout<< "Client processed a Frame: \n" << ToSend <<std::endl;
            try{
                if(!CH->sendLine(ToSend)){
                    // std::cout<< ToSend <<std::endl;
                    std::cout << "Disconnected. Exiting..." << endl;
                }
                // cout << "Frame send" << endl;
            }catch(...){}
        }
    }
}

void StompProtocol::processGet(){
    std::string msg;
    while(!shouldTerminate){
        if(!connected){break;}
        if(!CH->getLine(msg)){
            std::cout << "Disconnected. Exiting..." << std::endl;
            break;
        }
        // cout << msg << endl;
        // cout<< "got this message:\n" << msg << endl;
        vector<string> Splitter = splitString4(msg, '\n');
        // remove_empty_strings_from_beginning(Splitter);
        if(msg.find("ERROR") != string::npos){
            cout << msg << endl; // getErrorDesc??
            cout << "User will disconnect now" << endl;
            deleteUser();
            shouldTerminate = true;
        }
        else if(msg.find("MESSAGE") != string::npos){
            GamesSummary.UserReport(msg);
            cout << msg << endl;
        }
        else{
            vector<string> Splitter = splitString4(msg, '\n');
            ProcessAndBuildGet(Splitter);
        }
        msg = "";
    }
}

string StompProtocol::getErrorDesc(vector<string> msg){
    int count = 0;
    for(string& S : msg){
        if(count ==2){
            return S;
        }
        if(S == "---------------------"){
            count++;
        }
    }
}

string StompProtocol::ProcessAndBuildSend(vector<string> msg){
    string StompCommand = msg[0];
    if(StompCommand == "login"){
        // vector<string> IP = splitString4(msg[1], ':');
        // setCH(IP[0], std::stoi(IP[1]));
        // UserConnect(msg[2]);
        ConnectFrame ans = ConnectFrame(msg);
        return ans.toString();
    }
    else if(StompCommand == "report"){
        // cout << "enter report" << endl;
        SendFrame ans = SendFrame(msg, ClientUser->getUserName()); 
        string Report = ans.toString();
        // cout << "The Report: \n" << Report << endl;
        // GamesSummary.UserReport(Report);
        return Report;
        }
    else if(StompCommand == "join"){
        ClientUser->addNewTopicSub(msg[1], getSubId());
        SubscribeFrame ans = SubscribeFrame(msg[1], getSubIdAndInc(), getReceiptIdAndInc());
        ClientUser->addJoinReceiptToGame(ans.getReceiptId(), ans.getDestination());
        return ans.toString();
    }
    else if(StompCommand == "exit"){
        int ReceiptId = getReceiptIdAndInc();
        ClientUser->addExitReceiptToGame(std::to_string(ReceiptId), msg[1]);
        UnSubscribeFrame ans = UnSubscribeFrame(ClientUser->getSubIdByTopic(msg[1]), ReceiptId);
        return ans.toString();
    }
    else if(StompCommand == "logout"){
        DisConnectFrame ans = DisConnectFrame(getReceiptIdAndInc());
        shouldTerminate = true;
        connected = false;
        ClientUser->addLogOutReceipt(ans.getReceiptId());
        return ans.toString();
        }
}

void StompProtocol::ProcessAndBuildGet(vector<string> msg){
    int place = 0;
    for (place = 0; place < msg.size(); place++) {
        if (msg[place] != "") {
            break;
        }
    }
    string StompCommand = msg[place];
    if(StompCommand == "CONNECTED" ){

        cout << "Login successful\n" << endl;
    }
    else{

        ClientUser->RemoveReceiptAndPrint(msg[place+1]);
        if(shouldTerminate){
            CH->close();
        }
    }    
}

void StompProtocol::ProcessSummary(vector<string> msg){
    string GameName = msg[1];
    // cout << "GameName: " << GameName << endl;
    string UserName = msg[2];
    // cout << "UserName: " << UserName << endl;
    string FileName = msg[3];
    // cout << "FileName: " << FileName << endl;
    string json = GamesSummary.toString(GameName, UserName);
    // cout << "json: \n" << json << endl;
    writeFile(json, FileName);
    cout << "Summary file created" << endl;
    // cout << json << endl;
}

void StompProtocol::writeFile(string json, string path){
    ofstream outFile;
    try{
        outFile.open(path, ios::trunc);
        outFile << json;
        outFile.close();
    }catch(...){cout << "Error occured while trying to save file " << endl;}
}

void StompProtocol::UserConnect(string Name){
    connected = true;
    ClientUser->setUserName(Name);
}

int StompProtocol::getSubId(){
    return SubCounter;
}

void StompProtocol::IncSubCounter(){
    SubCounter++;
}

int StompProtocol::getSubIdAndInc(){
    int temp = getSubId();
    IncSubCounter();
    return temp;
}

int StompProtocol::getReceiptId(){
    return ReceiptCounter;
}

void StompProtocol::IncReceiptCounter(){
    ReceiptCounter++;
}

int StompProtocol::getReceiptIdAndInc(){
    int temp = getReceiptId();
    IncReceiptCounter();
    return temp;
}

void StompProtocol::deleteUser(){
    ClientUser->setUserName("");
    ClientUser->ClearMaps();
}

// ConnectionHandler& StompProtocol::getCH(){
//     return CH;
// }

void StompProtocol::setCH(string host, short port){
    CH = new ConnectionHandler(host, port);
    CH->connect();
}

void StompProtocol::start(){
    while(!connected){
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string msg(buf);
        vector<string> Splitter = splitString4(msg, ' ');
        if(Splitter[0] == "login"){
            vector<string> IP = splitString4(Splitter[1], ':');
            setCH(IP[0], std::stoi(IP[1]));
            UserConnect(Splitter[2]);
            connected = true;
            string ToSend = ProcessAndBuildSend(Splitter);
            // std::cout<< "Client processed a Frame: \n" << ToSend <<std::endl;
            try{
                if(!CH->sendLine(ToSend)){
                    std::cout << "Disconnected. Exiting..." << endl;
                }
            }catch(...){}
        }
        else{
            cout << "Please enter a command in this pattern : login {host:port} {username} {password}" <<endl;
        }
    }
}



