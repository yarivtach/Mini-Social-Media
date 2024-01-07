package bgu.spl.net.impl.FramesClients;

public class UnSubscribeFrame extends Frame {

    private String StompCommand;
    private String SubId;
    private String ReceiptId;

    public UnSubscribeFrame(String[] msg){
        super();
        for (String S : msg) {
            setData(S);
        }
    }

    public void setData(String line){
        if(line.contains("UNSUBSCRIBE")){
            StompCommand = "UNSUBSCRIBE";
        }
        else{
            String[] keyData = line.split(":");
            switch(keyData[0]){
                case "id":
                    SubId = keyData[1];
                case "receipt":
                    ReceiptId = keyData[1];
            }
        }
    }

    public String getStomp(){
        return StompCommand;
    }

    public String getSubId(){
        return SubId;
    }

    public String getReceiptId(){
        return ReceiptId;
    }

    public String toString(){
        String ans = (StompCommand + "\n" +
                      "id:" + SubId + "\n" +
                      "\n" +
                      '\u0000');
        return ans;
    }
}
