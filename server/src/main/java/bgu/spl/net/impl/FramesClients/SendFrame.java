package bgu.spl.net.impl.FramesClients;

public class SendFrame extends Frame {

    private String StompCommand;
    private String Destination;
    private String FrameBody;

    public SendFrame(String[] msg){
        super();
        StompCommand = "SEND";
        String[] dest = msg[1].split(":");
        Destination = dest[1].substring(1);
        FrameBody = "";
        for(int i = 2; i< msg.length; i++){
            FrameBody = FrameBody + msg[i] + "\n";
        }
    }

    public String getDestination(){
        return Destination;
    }
    
    public String toString(){
        String ans = (StompCommand + "\n" +
                      "destination:/" +Destination + "\n" + 
                      "\n" +
                      FrameBody+ "\n" +
                      '\u0000');
        return ans;
    }
}
