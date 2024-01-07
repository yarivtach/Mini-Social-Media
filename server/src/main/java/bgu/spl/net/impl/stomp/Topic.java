package bgu.spl.net.impl.stomp;

import java.util.LinkedList;
import java.util.List;

public class Topic {

    private final String TopicName;
    private List<Integer> SubList;
    
    public Topic(String _Name, Integer _FirstSub){
        this.TopicName = _Name;
        this.SubList = new LinkedList<Integer>();
        SubList.add(_FirstSub);
    }

    public final String getName(){
        return TopicName;
    }

    public List<Integer> getSubList(){
        return SubList;
    }

    public void addSub(Integer NewSub){
        SubList.add(NewSub);
    }

    public boolean removeSub(Integer ByeSub){
        return SubList.remove(ByeSub);
    }

    public boolean IsSubscribed(Integer SubID){
        return SubList.contains(SubID);
    }
}
