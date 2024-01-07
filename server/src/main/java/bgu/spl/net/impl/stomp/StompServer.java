package bgu.spl.net.impl.stomp;

import java.io.IOError;
import java.io.IOException;
import java.net.SocketPermission;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.BaseServer;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        ConnectionsImpl<String> connections = new ConnectionsImpl<String>();
       if(args.length == 2){
            if(args[1].equals("tpc")){
                System.out.println("TPC");
                Server.threadPerClient(
                // 7777,
                Integer.valueOf(args[0]),
                () -> new STOMPProtocol(connections),
                STOMPEncoderDecoder::new 
                ).serve();
            }else{
                System.out.println("REACTOR");
                Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    // 7777,
                    Integer.valueOf(args[0]),
                    () -> new STOMPProtocol(connections),
                    STOMPEncoderDecoder::new 
                ).serve();
            }
       }
        
    }
}
