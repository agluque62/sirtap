package CheckProxy;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.net.URI;

import javax.websocket.ClientEndpoint;
import javax.websocket.CloseReason;
import javax.websocket.ContainerProvider;
import javax.websocket.OnClose;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.WebSocketContainer;

/**
 * ChatServer Client
 *
 * @author Jiji_Sasidharan
 */
@ClientEndpoint
public class WebsocketClientEndpoint {

    Session userSession = null;
    private MessageHandler messageHandler;
    private Session sess = null;

    public WebsocketClientEndpoint(String st_endpointURI) {
        try {
            WebSocketContainer container = ContainerProvider.getWebSocketContainer();
            sess = container.connectToServer(this, new URI(st_endpointURI));
        } catch (Exception e) {
        	Close();
        	sess = null;        	
        	
        	Main.MylogError("WebsocketClientEndpoint: " + e.toString()+" "+e.getMessage());
        	ByteArrayOutputStream stream = new ByteArrayOutputStream();				
            PrintStream ps = new PrintStream(stream); 
            e.printStackTrace(ps);
            String finalString = new String(stream.toByteArray());
            Main.MylogError(finalString);        	
        	
            throw new RuntimeException(e);
        }
    }
    
    public void Close()
    {
    	if (sess != null)
			try {
				sess.close();
			} catch (IOException e) {
			}    	
    }

    /**
     * Callback hook for Connection open events.
     *
     * @param userSession the userSession which is opened.
     */
    @OnOpen
    public void onOpen(Session userSession) {
        //System.out.println("opening websocket");        
        this.userSession = userSession;
    }

    /**
     * Callback hook for Connection close events.
     *
     * @param userSession the userSession which is getting closed.
     * @param reason the reason for connection close
     */
    @OnClose
    public void onClose(Session userSession, CloseReason reason) {
        this.userSession = null;
    }

    /**
     * Callback hook for Message Events. This method will be invoked when a client send a message.
     *
     * @param message The text message
     */
    @OnMessage
    public void onMessage(String message) {    	
    	if (this.messageHandler != null) {
            this.messageHandler.handleMessage(message);
        }
    }

    /**
     * register message handler
     *
     * @param msgHandler
     */
    public void addMessageHandler(MessageHandler msgHandler) {
        this.messageHandler = msgHandler;
    }
    
    /**
     * Send a message.
     *
     * @param message
     */
    public void sendMessage(String message) {
    	
    	//if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENVIO:"+message);
    	
        this.userSession.getAsyncRemote().sendText(message);
    }

    /**
     * Message handler.
     *
     * @author Jiji_Sasidharan
     */
    public static interface MessageHandler {

        public void handleMessage(String message);
    }
    
}
