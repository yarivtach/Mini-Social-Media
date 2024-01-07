package bgu.spl.net.impl.stomp;
// import java.lang.String;

import java.util.List;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.FramesClients.ConnectFrame;
import bgu.spl.net.impl.FramesClients.ConnectedFrame;
import bgu.spl.net.impl.FramesClients.DisConnectFrame;
import bgu.spl.net.impl.FramesClients.ErrorFrame;
import bgu.spl.net.impl.FramesClients.Frame;
import bgu.spl.net.impl.FramesClients.MessageFrame;
import bgu.spl.net.impl.FramesClients.ReceiptFrame;
import bgu.spl.net.impl.FramesClients.SendFrame;
import bgu.spl.net.impl.FramesClients.SubscribeFrame;
import bgu.spl.net.impl.FramesClients.UnSubscribeFrame;
import bgu.spl.net.srv.ConnectionsImpl;


public class STOMPProtocol implements StompMessagingProtocol<String> { 

    private int ClientId;
    private ConnectionsImpl<String> Connections;
    private boolean shouldTerminate = false;
    
    public STOMPProtocol(ConnectionsImpl<String> connections){
        Connections = connections;
    }

    @Override
    public void start(int connectionId){
        ClientId = connectionId;
    }

    public void process(String message, int ClientId){       
        String[] Splitter = message.split("\n");
        Frame response = ProccesAndBuildFrame(Splitter);
        // ConnectionHandler<String> curConH = Connections.getConnectionHandlerByID(ClientId);
        // if(!curConH.equals(null)){
        //     Connections.getConnectionHandlerByID(ClientId).send(response.toString());
        // }
        if(response instanceof ErrorFrame){
            Connections.getConnectionHandlerByID(ClientId).send(response.toString());
            // System.out.println("Frame sent: \n" + response.toString());
            Connections.endConnection(ClientId);
        }
        else if(response instanceof MessageFrame){
            MessageFrame CastedResponse = (MessageFrame)response;
            String dest = CastedResponse.getDestination();
            List<Topic> TList = Connections.getTopicList();
            for (Topic top : TList) {
                if(top.getName().equals(dest)){
                    List<User> UserL = Connections.getUserList();
                    for (User U : UserL) {
                        if(U.getSubIdByTopic(top) != null){
                            int CId = U.getClientId();
                            Connections.getConnectionHandlerByID(CId).send(CastedResponse.toString());
                            // System.out.println("Frame sent: \n" + CastedResponse.toString());
                        }
                    }
                }
                break;
            }
        }
        else{

            Connections.getConnectionHandlerByID(this.ClientId).send(response.toString());
            // System.out.println("Frame sent: \n" + response.toString());
        }
        if(shouldTerminate){
            Connections.endConnection(this.ClientId);
        }
    }
         
	
    public boolean shouldTerminate(){
        return shouldTerminate;
    }

    public Frame ProccesAndBuildFrame(String[] msg){
        String StompCommand = msg[0];
        switch(StompCommand){

            case "CONNECT":   
                return BuildLogIn(msg);
            case "SUBSCRIBE":
                return BuildJoin(msg);
            case "UNSUBSCRIBE":
                return BuildExit(msg);
            case "DISCONNECT":
                return BuildLogOut(msg);
            case "SEND":
                return BuildReport(msg, String.valueOf(Connections.getUserObjById(ClientId).getSubIdByTopicName(msg[1].substring(13))));
        }    
        return new ErrorFrame(null, StompCommand, "Command not valid", "Please use a valid command");// maybe should get as string
    }


    private Frame BuildLogIn(String[] msg){
        shouldTerminate = false;
        ConnectFrame ans = new ConnectFrame(msg);
        // System.out.println("Server received this Frame: \n" + ans.toString() + "\n");
        String Name = ans.getLogIn();
        String Pass = Connections.WasConnected(Name); // returns password by name.
        if(Pass != null){
            int FixedClientId = Connections.getClientIdByName(Name);
            if(Connections.getConnectionHandlerByID(FixedClientId).getConnected()){
                return new ErrorFrame(ans, Pass, "User already logged in", "User tried to log in while user already logged in");
            }
            Connections.swapCHById(ClientId, FixedClientId);
            setId(FixedClientId);
            if(!Pass.equals(ans.getPass())){
                return new ErrorFrame(ans, Pass, "Password missmatch", "”Wrong password”"); //right name wrong password. rturn ErrorFrame
            }
            else{
                Connections.getConnectionHandlerByID(FixedClientId).Connect(); // right name, right password. change connection handler to connected.
            }
        }
        
        else{
            Connections.getUserDataMap().put(Name, ans.getPass()); // No such user. create a new one and add him to the UserMap
            Connections.addUserObj(new User(ClientId));
            Connections.addNameToIdMap(Name, ClientId);
        }
        return new ConnectedFrame();
    }

    private Frame BuildReport(String[] msg, String SubId){
        SendFrame ans = new SendFrame(msg);
        // System.out.println("Server received this Frame: \n" + ans.toString() + "\n");
        if(!Connections.getConnectionHandlerByID(ClientId).getConnected()){
            return new ErrorFrame(ans, "",  "Not connected", "You are not connected to the server");
        }
        String[] Splitter = ans.toString().split("\n");
        return new MessageFrame(Splitter, SubId, String.valueOf(Connections.getAndInceMessageId()));
    }

    private Frame BuildJoin(String[] msg){
        SubscribeFrame ans = new SubscribeFrame(msg);
        // System.out.println("Server received this Frame: \n" + ans.toString() + "\n");
        if(!Connections.getConnectionHandlerByID(ClientId).getConnected()){
            return new ErrorFrame(ans, "", "Not connected", "You are not connected to the server");
        }
        int SubId = Integer.parseInt(ans.getSubId());
        Topic top = Connections.ExistTopic(ans.getDestination());
        if(top != null){ // if topic exists.
            if(!Connections.AlreadySub(ClientId, top, SubId)){ // if not already subbed to it.
                top.addSub(SubId); // topic exists and client isnt subbed. add sub to the topic.
                Connections.getUserObjById(ClientId).addTopicToUser(Integer.parseInt(ans.getSubId()), top); // add to map subID and topic for this user.
            }
            else{
                return new ErrorFrame(ans, "", null, "Already subscribed to this topic"); // client already subscribed to this topic, sends an error frame.
            }
        }
        else{
            Connections.CreateTopic(ans.getDestination(), SubId); // topic doesnt exist. create a new topic.
            Connections.getUserObjById(ClientId).addTopicToUser(Integer.parseInt(ans.getSubId()), new Topic(ans.getDestination(), SubId)); // add to map subID and topic for this user.
        }
        return new ReceiptFrame(ans.getReceiptId());
    }

    private Frame BuildExit(String[] msg){
        UnSubscribeFrame ans = new UnSubscribeFrame(msg);
        // System.out.println("Server received this Frame: \n" + ans.toString() + "\n");
        int SubId = Integer.parseInt(ans.getSubId());
        if(!Connections.getConnectionHandlerByID(ClientId).getConnected()){
            return new ErrorFrame(ans, "", "Not connected", "You are not connected to the server");
        }
        Topic top = Connections.getTopicBySubId(SubId);
        if(top != null){ // if the topic exists.
            if(!Connections.AlreadySub(ClientId, top, Integer.parseInt(ans.getSubId()))){ // topic exists , now check if the client is really subbed to it.
                return new ErrorFrame(ans, "", "Not Subscribed","You can not unsubscribe from a topic that you are not subsribed to");
                                       // topic exists, client is not subbed to it, return error frame.
            }
            else{ // topic exists and the client is subscribed to it.
                Connections.UnSub(top, SubId, ClientId);
                return new ReceiptFrame(ans.getReceiptId());
            }
        }
        else{ // topic doesnt exist.
            return new ErrorFrame(ans,"", "No such topic", "Yoy can not unsubscribe from a topic that does not exist");
        }
    }

    private Frame BuildLogOut(String[] msg){
        DisConnectFrame ans = new DisConnectFrame(msg);
        // System.out.println("Server received this Frame: \n" + ans.toString() + "\n");
        if(!Connections.getConnectionHandlerByID(ClientId).getConnected()){
            return new ErrorFrame(ans, "", "Not connected", "You are not connected to the server");
        }
        shouldTerminate = true;
        return new ReceiptFrame(ans.getReceiptId());
    }

    public void setTerminate(boolean TF){
        shouldTerminate = TF;
    }

    public ConnectionsImpl<String> getConnections(){
        return Connections;
    }

    public void setId(int newId){
        ClientId = newId;
    }

}
