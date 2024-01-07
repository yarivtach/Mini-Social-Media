package bgu.spl.net.api;

import bgu.spl.net.srv.ConnectionsImpl;

public interface MessagingProtocol<T> {
 
    /**
     * process the given message 
     * @param msg the received message
     * @return the response to send or null if no response is expected by the client
     */
    T process(T msg, int id);
 
    /**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();

    void start(int ClientId, ConnectionsImpl<String> connections);
 
}