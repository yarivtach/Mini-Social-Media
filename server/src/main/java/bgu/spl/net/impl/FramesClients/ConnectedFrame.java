package bgu.spl.net.impl.FramesClients;

public class ConnectedFrame extends Frame {

    private String StompCommand;
    private String Version;

    public ConnectedFrame(){
        super();
        this.StompCommand = "CONNECTED";
        this.Version = "1.2";
    } 

    public String toString(){
        String ans = (StompCommand + "\n" +
                      "version:" + Version + "\n" + '\u0000');
        return ans;
    }
}
