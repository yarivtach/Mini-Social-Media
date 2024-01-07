package bgu.spl.net.impl.FramesClients;

public class MessageFrame extends Frame {

    private String StompCommand;
    private String Subsription;
    private String MessageId;
    private String Destination;
    private String FrameBody;
    

    public MessageFrame(String[] msg, String SubId, String _MessageId){
        super();
        this.StompCommand = "MESSAGE";
        this.Subsription = SubId;
        this.MessageId = _MessageId;
        this.Destination = msg[1].substring(13);
        String Content = "";
        for (int i = 2; i < msg.length; i++) {
            Content = Content + msg[i] + "\n";
        }
        this.FrameBody = Content;
    } 

    public String getDestination(){
        return Destination;
    }
    
    public String toString(){
        String ans = (StompCommand + "\n" +
                      "subscription:" + Subsription + "\n" + 
                      "Message-id:" + MessageId + "\n" +
                      "destination:" + Destination + "\n" + 
                      "\n" +
                      "Hello " + Destination + "\n" +
                      FrameBody);
        return ans;
    }
}
