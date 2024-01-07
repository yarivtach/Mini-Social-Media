package bgu.spl.net.api;

import bgu.spl.net.srv.ConnectionsImpl;

public interface StompMessagingProtocol<T> {
	/**
	 * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
	**/
    void start(int connectionId);

    
    void process(T message, int ClientId);
	
	/**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();

    ConnectionsImpl<T> getConnections();
}
