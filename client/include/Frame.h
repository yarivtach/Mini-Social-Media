#pragma once
#include <string>
#include <vector>
using namespace std;
class StompProtocol;

;class Frame{

    private:

    public:
        Frame();
        virtual ~Frame();
        virtual string toString() = 0;

};

class ConnectedFrame : public Frame{

    private:
        string StompCommand;
        string Version;
        
    public:
        ConnectedFrame();
        virtual ~ConnectedFrame();
        string getStomp();
        string getVersion();
        virtual string toString();
};

class ConnectFrame : public Frame{

    private:
        string StompCommand;
        string AcceptVersion;
        string Host;
        string LogIn;
        string Passcode;
        
    public:
        ConnectFrame(vector<string> msg);
        virtual ~ConnectFrame();
        string getStomp();
        string getVersion();
        string getHost();
        string getLogIn();
        string getPass();
        virtual string toString();
};

class DisConnectFrame : public Frame{

    private:
        string StompCommand;
        string ReceiptId;
        
    public:
        DisConnectFrame(int ReceiptId);
        virtual ~DisConnectFrame();
        string getReceiptId();
        virtual string toString();
};

class ErrorFrame : public Frame{

    private:
        string StompCommand;
        string ReceiptId;
        string errMessage;
        string F;
        string errDescription;
        
    public:
        ErrorFrame(string ReceiptId , string errorMessage , string F , string errDescription );
        virtual ~ErrorFrame();
};

class MessageFrame : public Frame{

    private:
        string StompCommand;
        string Subsription;
        string MessageId;
        string Destination;
        string FrameBody;
        
    public:
        MessageFrame(vector<string> msg, int id);
        virtual ~MessageFrame();
        string getStomp();
        string getSub();
        string getMessageId();
        string getDestination();
        string getFrameBody();
        virtual string toString();
};

class ReceiptFrame : public Frame{

    private:
        string StompCommand;
        string ReceiptId;        
    public:
        ReceiptFrame(string ReceiptId);
        string getReceiptId();
        virtual string toString();
};

class ReportFrame : public Frame{

    private:
        string StompCommand;
        string Destination;
        string FrameBody;
        
    public:
        ReportFrame(vector<string> msg, int ReceiptId);
        virtual ~ReportFrame();
        string getStomp();
        string getDestination();
        string getFrameBody();
        virtual string toString();
};

class SendFrame : public Frame{

    private:
        string StompCommand;
        string Destination;
        string FrameBody;
        
    public:
        SendFrame(vector<string> msg, string UserName);
        vector<string> splitString1(string str, char delimiter);
        virtual ~SendFrame();
        string getStomp();
        string getDestination();
        string getFrameBody();
        // void setData(string line);
        virtual string toString();
};

class SubscribeFrame : public Frame{

    private:
        string StompCommand;
        string SubsriptionId;
        string ReceiptId;
        string Destination;
        string FrameBody;
        
    public:
        SubscribeFrame(string TopicName, int SubId, int _ReceiptId);
        virtual ~SubscribeFrame();
        string getStomp();
        string getSubId();
        string getReceiptId();
        string getDestination();
        string getFrameBody();
        virtual string toString();};

class UnSubscribeFrame : public Frame{

    private:
        string StompCommand;
        string SubsriptionId;
        string ReceiptId;
        
    public:
        UnSubscribeFrame(int SubId, int ReceiptId);
        virtual ~UnSubscribeFrame();
        string getStomp();
        string getSubId();
        virtual string toString();

};