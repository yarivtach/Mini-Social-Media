package bgu.spl.net.impl.stomp;


import java.util.concurrent.ConcurrentHashMap;


public class User {
    private int ClientId;
    private ConcurrentHashMap<Integer, Topic> UserToTopic;
    
    public User(int _CliendId){
        ClientId = _CliendId;
        UserToTopic = new ConcurrentHashMap<Integer, Topic>();
    }

    public int getClientId(){
        return ClientId;
    }

    public ConcurrentHashMap<Integer,Topic> getUserToTopic(){
        return UserToTopic;
    }

    public void addTopicToUser(int SubId, Topic top){
        UserToTopic.put(SubId, top);
    }

    public Topic getTopicBySubId(int SubId){
        return UserToTopic.get(SubId);
    }

    public Integer getSubIdByTopic(Topic top){
        for (Integer k : UserToTopic.keySet()) {
            if (UserToTopic.get(k).getName() == top.getName()) {
              return k;
            }
          }
        return null;
    }

    public Integer getSubIdByTopicName(String Name){
        for (Integer k : UserToTopic.keySet()) {
            if (UserToTopic.get(k).getName().equals(Name)){
                return k;
            }
        }
        return null;
    }

    public void removeSubByTopic(int SubId){
        UserToTopic.remove(SubId);
    }
}
