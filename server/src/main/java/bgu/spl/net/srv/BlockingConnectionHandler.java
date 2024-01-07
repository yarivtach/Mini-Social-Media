package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    private final StompMessagingProtocol<T> protocol;
    private final MessageEncoderDecoder<T> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    private int Clientid;

    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, StompMessagingProtocol<T> protocol, int Clientid) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        this.Clientid = Clientid;
    }

    @Override
    public void run() {
        try (Socket sock = this.sock) { //just for automatic closing
            int read;
            protocol.getConnections().addcHandler(Clientid, this);
            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                T nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    protocol.process(nextMessage, Clientid);
                //     if (response != null) {
                //         out.write(encdec.encode(response));
                //         out.flush();
                //     } 
                // }
                }
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    public int getId(){
        return Clientid;
    }

    public void setId(int _ClientId){
        Clientid = _ClientId;
    }

    public void DisConnect(){
        connected = false;
    }

    public void Connect(){
        connected = true;
    }

    public boolean getConnected(){
        return connected;
    }

    public void startProtocol(int ClientId){
        protocol.start(ClientId);
    }

    @Override
    public synchronized void send(T msg) {
        //IMPLEMENT IF NEEDED
        try{
            if (msg != null) {
                out.write(encdec.encode(msg));
                out.flush();
            }
        }catch(IOException ex) {
            ex.printStackTrace();
        }
    }
}
