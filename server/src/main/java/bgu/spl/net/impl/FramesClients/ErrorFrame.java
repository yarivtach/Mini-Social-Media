package bgu.spl.net.impl.FramesClients;

public class ErrorFrame extends Frame {

    private String StompCommand;
    private String errLine;
    private String errMessage;
    private Frame F;
    private String errDescription;

    public ErrorFrame(Frame _F, String _errLine, String _errMessage, String _errDescription){
        super();
        this.StompCommand = "ERROR";
        this.errLine = _errLine;
        this.F = _F;
        this.errMessage = _errMessage;
        this.errDescription = _errDescription + "\n" + "~@";
    }
    
    public String toString(){
        String ans;
        if(F == null){
            ans = (StompCommand + "\n" +
                      "Receipt-id:" + errLine + "\n" + 
                      "Message:" + errMessage + "\n" +
                      "\n" +
                      "The Message:" + "\n" +
                      "---------------------" + "\n" +
                      "\n" +
                      "---------------------" + "\n" +
                      errDescription+ "\n" +
                      '\u0000');
        }
        else{
            ans = (StompCommand + "\n" +
            "Receipt-id:" + errLine + "\n" + 
            "Message:" + errMessage + "\n" +
            "\n" +
            "The Message:" + "\n" +
            "---------------------" + "\n" +
            F.toString() + "\n" +
            "---------------------" + "\n" +
            errDescription+ "\n" +
            '\u0000');
        }
        return ans;
    }
}

