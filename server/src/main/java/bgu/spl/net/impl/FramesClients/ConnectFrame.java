package bgu.spl.net.impl.FramesClients;

public class ConnectFrame extends Frame {

    private String StompCommand;
    private String AcceptVersion;
    private String Host;
    private String LogIn;
    private String PassCode;

    public ConnectFrame(String[] msg){
        super();
        for (String S : msg) {
            setData(S);
        }
    }

    public String IsLegalFrame(){
        //check host?
        return "Host adress is not legal";
    }

    public void setData(String line){
        //if(line.contains("CONNECT")||line.contains("accept-version")||line.contains("host")||line.contains("login")||line.contains("passcode")){
            if(line.contains("CONNECT")){
                StompCommand = "CONNECT";
            }
            else{
            String[] keyData = line.split(":");
            switch(keyData[0]){
                //case "CONNECT":
                    // StompCommand = "CONNECT";
                case "Accept-Version":
                    AcceptVersion = keyData[1];
                case "Host":
                    Host = keyData[1];
                case "Login":
                    LogIn = keyData[1];
                case "Passcode":
                    PassCode = keyData[1];
            }
        }
    //}
    }

    public String getStomp(){
        return StompCommand;
    }

    public String getAV(){
        return AcceptVersion;
    }

    public String getHost(){
        return Host;
    }

    public String getLogIn(){
        return LogIn;
    }

    public String getPass(){
        return PassCode;
    }

    public String toString(){
        String ans = (StompCommand + "\n" +
                      "accept-version:" + AcceptVersion + "\n" + 
                      "host:" + Host + "\n" +
                      "login:" + LogIn + "\n" +
                      "passcode:" + PassCode);
        return ans;
    }
}
