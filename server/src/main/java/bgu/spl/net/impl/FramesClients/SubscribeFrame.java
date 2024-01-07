package bgu.spl.net.impl.FramesClients;

public class SubscribeFrame extends Frame {

    private String StompCommand;
    private String Destination;
    private String SubId;
    private String ReceiptId;

    public SubscribeFrame(String[] msg){
        super();
        for (String S : msg) {
            setData(S);
        }
    }

    public void setData(String line){
        if(line.contains("SUBSCRIBE")){
            StompCommand = "SUBSCRIBE";
        }
        else{
            String[] keyData = line.split(":");
            switch(keyData[0]){
                case "destination":
                    Destination = keyData[1].substring(1);
                case "id":
                    SubId = keyData[1]; // subId = keyData[1]
                case "receipt":
                    ReceiptId = keyData[1];
            }
        }
    }

    public String getStomp(){
        return StompCommand;
    }

    public String getDestination(){
        return Destination;
    }

    public String getSubId(){
        return SubId;
    }

    public String getReceiptId(){
        return ReceiptId;
    }

    public String toString(){
        String ans = (StompCommand + "\n" +
                      "destination:" + Destination + "\n" + 
                      "id:" + SubId+ "\n" +
                      "receipt:" + ReceiptId + "\n" +
                      '\u0000');
        return ans;
    }
}
