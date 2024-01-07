package bgu.spl.net.srv;

import java.io.IOException;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

import bgu.spl.net.impl.stomp.Topic;
import bgu.spl.net.impl.stomp.User;

public class ConnectionsImpl<T> implements Connections<T> {


    private ConcurrentHashMap<Integer,ConnectionHandler<T>> ConnectionHandlersMap;
    private List<Topic> TopicList;
    private ConcurrentHashMap<String,String> UserDataMap;
    private List<User> UserList;
    private AtomicInteger MessageId;
    private ConcurrentHashMap<String, Integer> NameToIdMap;

    public ConnectionsImpl(){
        ConnectionHandlersMap = new ConcurrentHashMap<Integer,ConnectionHandler<T>>();
        TopicList = new LinkedList<Topic>();
        UserDataMap = new ConcurrentHashMap<String,String>();
        UserList = new LinkedList<User>();
        MessageId = new AtomicInteger(0);
        NameToIdMap = new ConcurrentHashMap<String, Integer>();
    }

    public ConcurrentHashMap<String, Integer> getNameToIdMap(){
        return NameToIdMap;
    }

    public Integer getClientIdByName(String Name){ // returns the client id if was connected, -1 if was not.
        return NameToIdMap.get(Name);
    }

    public void addNameToIdMap(String Name, int ClientId){
        NameToIdMap.put(Name, ClientId);
    }

    public ConcurrentHashMap<Integer,ConnectionHandler<T>> getChMap(){
        return ConnectionHandlersMap;
    }

    public void addcHandler(int ClientID, ConnectionHandler<T> CH){
        ConnectionHandlersMap.put(ClientID, CH);
    }

    public void removeConnectionByID(int ClientID){
        ConnectionHandlersMap.remove(ClientID);
    }

    public void removeConnectionByCH(ConnectionHandler<T> CH){
        for (Map.Entry<Integer, ConnectionHandler<T>> entry : ConnectionHandlersMap.entrySet()) {
            if (entry.getValue() == CH) {
              ConnectionHandlersMap.remove(entry.getKey());
              break;
            }
        }
    }

    public List<Topic> getTopicList(){
        return TopicList;
    }

    public void addTopic(Topic _topic){
        TopicList.add(_topic);
    }

    public boolean removeSub(Integer ClientId, String TopicName){
        for (Topic Top : TopicList) {
            if(Top.getName().equals(TopicName)){
                // ByeSub.removeTopic(Top);
                return Top.removeSub(ClientId);
            }
        }
        return false;
    }

    public Topic ExistTopic(String Dest){
        for (Topic Top : TopicList) {
            if(Top.getName().equals(Dest)){
                return Top;
            }
        }
        return null;
    }

    public boolean AlreadySub(int ClientId, Topic top, int SubId){
        Map<Integer, Topic> SubToTopic = getUserObjById(ClientId).getUserToTopic();
        for (Map.Entry<Integer, Topic> set : SubToTopic.entrySet()) {
            if(set.getKey() == SubId && set.getValue().equals(top)){
                return true;
            }
        }
        return false;
    }

    public ConnectionHandler<T> getConnectionHandlerByID(int ClientId){
        return ConnectionHandlersMap.getOrDefault(ClientId, null);
    }

    public void swapCHById(int ClientId, int FixedClientId){
        int newId = FixedClientId;
        ConnectionHandler<T> newCH = ConnectionHandlersMap.get(ClientId);
        newCH.setId(FixedClientId);
        ConnectionHandlersMap.remove(FixedClientId);
        ConnectionHandlersMap.remove(ClientId);
        ConnectionHandlersMap.put(newId, newCH);

    }
    
    public void CreateTopic(String Name, Integer ClientId){
        Topic _topic = new Topic(Name, ClientId);
        TopicList.add(_topic);
    }

    public void UnSub(Topic top, int SubId, int ClientId){
        for (Topic T : TopicList) {
            if(T.getName() == top.getName()){
                top.removeSub(SubId);
            }
        }
        getUserObjById(ClientId).removeSubByTopic(SubId);
    }

    public boolean IsAlreadyConnected(int ClientId){
        return ConnectionHandlersMap.get(ClientId).getConnected();
    }

    public ConcurrentHashMap<String,String> getUserDataMap(){
        return UserDataMap;
    }

    public String getPassByName(String Name){
        return UserDataMap.getOrDefault(Name, null);
    }

    public String getNameByPass(String Pass){
        return UserDataMap.getOrDefault(Pass, null);
    }

    public String WasConnected(String Name){
        return getPassByName(Name);
    }

    public void AddUserData(String Name, String Pass){
        UserDataMap.put(Name, Pass);
    }

    public List<User> getUserList(){
        return UserList;
    }

    public void addUserObj(User NewUser){
        UserList.add(NewUser);
    }

    public void removeUserObj(User DelUser){
        UserList.remove(DelUser);
    }

    public User getUserObjById(int ClientId){
        for (User U : UserList) {
            if(U.getClientId() == ClientId){
                return U;
            }
        }
        return null;
    }

    public Topic getTopicBySubId(int SubId){
        Topic top;
        for (User U : UserList) {
            top = U.getTopicBySubId(SubId);
            if(top != null){
                return top;
            }
        }
        return null;
    }

    public void endConnection(int CId){
        User Bye = getUserObjById(CId);
        if(Bye != null){
            ConcurrentHashMap<Integer,Topic> ByeMap = Bye.getUserToTopic();
            Topic top;
            for (ConcurrentHashMap.Entry<Integer,Topic> set : ByeMap.entrySet()) {
                top = set.getValue();
                top.removeSub(set.getKey());
            }
        }
        try{
            getConnectionHandlerByID(CId).DisConnect();
            getConnectionHandlerByID(CId).close();
        }catch(IOException IO){}
    }

    public boolean send(int connectionId, T msg){
        ConnectionHandlersMap.get(connectionId).send(msg);
        return true;
    }

    public void send(String channel, T msg){
        for (Topic top : TopicList) {
            if(top.getName() == channel){
                List<Integer> SubList = top.getSubList();
                for (Integer Sub : SubList) {
                    send(Sub, msg);
                }
                break;
            }
        }
    }

    public void disconnect(int connectionId){
        ConnectionHandlersMap.remove(connectionId);
        for (Topic top : TopicList) {
            top.removeSub(connectionId);
        }
    }

    public int getMessageId(){
        return MessageId.get();
    }

    public int getAndInceMessageId(){
        return MessageId.getAndIncrement();
    }
}
