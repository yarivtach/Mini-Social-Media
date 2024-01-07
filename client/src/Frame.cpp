#include "../include/Frame.h"
#include <sstream>
#include "../include/StompProtocol.h"
#include "../include/event.h"
using namespace std;

static vector<string> MapToVector(map<string, string> theMap){
    vector<string> output;
    for (const auto& [key, value] : theMap) {
        output.push_back(key + ": " + value);
    }
    return output;
}

static string toStringVector(vector<string> vec){
    string output = "";
    for(string& S: vec){
        output = output + "\t" + S + "\n";
    }
    return output;
}

Frame::Frame(){}

Frame::~Frame(){}

ConnectFrame::ConnectFrame(vector<string> msg): Frame() , StompCommand("CONNECT"), AcceptVersion("1.2") , Host(msg[1]) ,LogIn(msg[2]), Passcode(msg[3]) {}

ConnectFrame::~ConnectFrame(){}

string ConnectFrame::getStomp() {
    return StompCommand;
}

string ConnectFrame::getVersion() {
    return AcceptVersion;
}

string ConnectFrame::getHost() {
    return Host;
}

string ConnectFrame::getLogIn() {
    return LogIn;
}

string ConnectFrame::getPass() {
    return Passcode;
}

string ConnectFrame:: toString() {
    string ans = StompCommand + "\n"
           "Accept-Version: " + AcceptVersion + "\n"
           "Host: " + Host + "\n"
           "Login: " + LogIn + "\n"
           "Passcode: " + Passcode;
    return ans;
}



// MessageFrame::MessageFrame(vector<string> msg, int id): Frame(), StompCommand("ERROR"), Subsription(""), MessageId("") , Destination("") , FrameBody("") {}

// MessageFrame::~MessageFrame(){}

string MessageFrame::getStomp() {
    return StompCommand;
}

string MessageFrame::getSub() {
    return Subsription;
}

string MessageFrame::getMessageId() {
    return MessageId;
}

string MessageFrame::getDestination() {
    return Destination;
}

string MessageFrame::getFrameBody() {
    return FrameBody;
}

string MessageFrame::toString() {
    return  StompCommand + "\n"
            "Subscription: " + Subsription + "\n"
            "Message-Id: " + MessageId + "\n"
            "Destination: " + Destination + "\n"
            "Frame Body: " + FrameBody;
}

ReceiptFrame::ReceiptFrame(string _ReceiptId):Frame() , StompCommand("RECEIPT"), ReceiptId(_ReceiptId) {}

string ReceiptFrame::getReceiptId(){
    return ReceiptId;
}

string ReceiptFrame::toString(){
    string ans = (StompCommand + "\n" +
                  "receipt-id:" +  + "\n" +  
                  "\n"
                  );
    return ans;
}

DisConnectFrame::DisConnectFrame(int _ReceiptId):Frame() , StompCommand("DISCONNECT"), ReceiptId(std::to_string(_ReceiptId)){}

DisConnectFrame::~DisConnectFrame(){}

string DisConnectFrame::getReceiptId(){
    return ReceiptId;
}

string DisConnectFrame::toString(){                
    string ans = (StompCommand + "\n" +
        "receipt-id:" + ReceiptId + "\n" +  
        "\n");
        return ans;
}


ErrorFrame::ErrorFrame(string _ReceiptId , string _errMessage , string _F , string _errDescription):Frame() , StompCommand("ERROR"), ReceiptId(_ReceiptId) ,errMessage(_errMessage) , F(_F) , errDescription(_errDescription){}

ErrorFrame::~ErrorFrame(){}

ConnectedFrame::ConnectedFrame(): Frame(), StompCommand("CONNECTED"), Version("1.2"){}

ConnectedFrame::~ConnectedFrame(){}

string ConnectedFrame:: getStomp(){
    return StompCommand;
}

string ConnectedFrame:: getVersion(){
    return Version;
}

string ConnectedFrame:: toString(){
    string ans = (StompCommand + "\nversion: " + Version);
    return ans;
}

SendFrame::SendFrame(vector<string> msg, string UserName):Frame(), StompCommand("SEND"), Destination(), FrameBody(""){
    names_and_events Struc = parseEventsFile(msg[1]);
    string TeamAName = Struc.team_a_name;
    string TeamBName = Struc.team_b_name;
    Destination = TeamAName + "_" + TeamBName;
    string user = UserName;
    vector<Event> events = Struc.events;
    for(Event& ReportData: events){
        string event_name = ReportData.get_name();
        string time = std::to_string(ReportData.get_time());
        map<string,string> general_game_updates = ReportData.get_game_updates();
        vector<string> GGU_vector = MapToVector(general_game_updates);
        string GGU_string = toStringVector(GGU_vector);
        map<string, string> team_a_updates = ReportData.get_team_a_updates();
        vector<string> TAU_vector = MapToVector(team_a_updates);
        string TAU_string = toStringVector(TAU_vector);
        map<string, string> team_b_updates = ReportData.get_team_b_updates();
        vector<string> TBU_vector = MapToVector(team_b_updates);
        string TBU_string = toStringVector(TBU_vector);
        string description = ReportData.get_discription();
        FrameBody = FrameBody + ("user: " + user + "\nteam a: " + 
                                  TeamAName + "\nteam b: " + 
                                  TeamBName + "\nevent name: " + event_name + "\ntime: " + 
                                  time +"\ngeneral game updates: \n" +
                                  GGU_string + "\nteam a updates: \n" +
                                  TAU_string + "\nteam b updates: \n" +
                                  TBU_string + "\ndescription: \n" +
                                  description + "\n--------------------\n");
    }   
}

SendFrame::~SendFrame(){}

vector<std::string> SendFrame::splitString1(string str, char delimiter) {
    vector<string> substrings;
    std::istringstream ss(str);
    std::string token;
    while(std::getline(ss, token, delimiter)) {
        substrings.push_back(token);
    }
    return substrings;
}

// void SendFrame:: setData(string line){
//     vector<string> keyData = splitString1(line, ' ');
//     if(keyData[0] == "SUBSCRIBE"){
//         StompCommand = keyData[1];
//     }
//     else if(keyData[0] == "destination"){
//         Destination = keyData[1].substr(1);
//     }
//     else{
//         FrameBody = keyData[1];
//     }
// }

string SendFrame::toString(){
    string ans = ("SEND\ndestination:/" + Destination + "\n" +
                  FrameBody);
    return ans;
}

SubscribeFrame::SubscribeFrame(string TopicName, int SubId, int _ReceiptId): StompCommand("SUBSCRIBE"), SubsriptionId(std::to_string(SubId)),ReceiptId(std::to_string(_ReceiptId)), Destination(TopicName), FrameBody((string)("")){}

SubscribeFrame::~SubscribeFrame(){}

string SubscribeFrame::getStomp(){
    return StompCommand;
}

string SubscribeFrame::getSubId(){
    return SubsriptionId;
}

string SubscribeFrame::getReceiptId(){
    return ReceiptId;
}

string SubscribeFrame::getDestination(){
    return Destination;
}

string SubscribeFrame::getFrameBody(){
    return FrameBody;
}
    
string SubscribeFrame::toString(){
    string ans = (StompCommand + "\n" +
                  "destination:/" + Destination + "\n" + 
                  "id:" + SubsriptionId + "\n" +
                  "receipt:" + ReceiptId +
                  "\n"
                  );
    return ans;
}

UnSubscribeFrame::UnSubscribeFrame(int _SubId, int _ReceiptId): StompCommand("UNSUBSCRIBE"), SubsriptionId(std::to_string(_SubId)), ReceiptId(std::to_string(_ReceiptId)){} // need to find the sub id by the topic.

UnSubscribeFrame::~UnSubscribeFrame(){}
string UnSubscribeFrame::toString(){
    string ans = ("UNSUBSCRIBE\nid:" + SubsriptionId +
                  "\nreceipt:" + ReceiptId);
    return ans;
}



