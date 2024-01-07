package bgu.spl.net.impl.FramesClients;

public class ReceiptFrame extends Frame {

    private String StompCommand;
    private String ReceiptId;

    public ReceiptFrame(String _ReceiptId){
        super();
        this.StompCommand = "RECEIPT";
        this.ReceiptId = _ReceiptId;
    }

    public String getStomp(){
        return StompCommand;
    }

    public String getReceiptId(){
        return ReceiptId;
    }

    public String toString(){
        String ans = (StompCommand + "\n" +
                      "receipt-id:" + ReceiptId+ "\n" +
                      '\u0000');
        return ans;
    }
}
