package bgu.spl.net.impl.FramesClients;

public class DisConnectFrame extends Frame {

    private String StompCommand;
    private String ReceiptId;

    public DisConnectFrame(String[] msg){
        super();
        for (String S : msg) {
            setData(S);
        }
    }

    public void setData(String line){
        if(line.contains("DISCONNECT")){
            StompCommand = "DISCONNECT";
        }
        else{
        String[] keyData = line.split(":");
            switch(keyData[0]){
                case "receipt-id":
                    ReceiptId = keyData[1];
            }
        }
    }

    public String getStomp(){
        return StompCommand;
    }

    public String getReceiptId(){
        return ReceiptId;
    }

    public String toString(){
        String ans = (StompCommand + "\n" +
                      "receipt:" + ReceiptId + "\n" +
                      '\u0000');
        return ans;
    }

    public String IsLegalFrame(){
        return "";
    }
    
}
