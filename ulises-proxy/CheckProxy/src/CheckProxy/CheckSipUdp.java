package CheckProxy;


import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Random;

public class CheckSipUdp {
	
	private DatagramSocket sipSocket;
	
	public CheckSipUdp() throws Exception
	{
		sipSocket = new DatagramSocket(0);
		sipSocket.setSoTimeout(1000);
	}
	
	public void CloseSocket()
	{
		sipSocket.close();
	}
	
	/*Se envia el metodo options a la brekeke para comprobar que esta vivo
	 * Por ejemplo si la aplicacion esta parada entonces no contestaria al 
	 * metodo options.
	 */
	public boolean check(String ipAddress, int outPort)  {
		
		boolean ret = true;
		
		try
		{	      
	        InetAddress inetIpAddress = InetAddress.getByName(ipAddress);
	        byte [] sendData = new byte[1024];
	        byte [] receiveData = new byte[1024];
	
	        String call_id = generateCallId() + "@" + InetAddress.getLocalHost().getHostAddress();
	        //Message/Method which will be used for checking remote server availability.
	        String method = "OPTIONS sip:" + ipAddress + ":" + outPort + " SIP/2.0\r\n" + 
	        		"Call-ID: " + call_id + "\r\n" + 
	        		"CSeq: 1 OPTIONS\r\n"+
	        		"From: <sip:" + InetAddress.getLocalHost().getHostAddress() + ":" + sipSocket.getLocalPort() + ">;tag=" + new Random().nextInt() + "\r\n" + 
	        		"To: <sip:" + ipAddress + ":" + outPort + ">\r\n" + 
	        		"Via: SIP/2.0/UDP " + InetAddress.getLocalHost().getHostAddress() + ":" + sipSocket.getLocalPort() + ";branch=z9hG4bK-323032-" + generateCallId() + "\r\n" + 
	        		"Max-Forwards: 70\r\n" + 
	        		"Contact: <sip:" + InetAddress.getLocalHost().getHostAddress()+ ":" + sipSocket.getLocalPort() + ">\r\n" + 
	        		"Content-Length: 0\r\n\r\n";
	        
	        sendData = method.getBytes();
	        
	        //System.out.println("REQUEST:" + ipAddress + "\n" + method);       
	
	        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, inetIpAddress, outPort);
	        sipSocket.send(sendPacket);
	
	        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
	        sipSocket.receive(receivePacket);			

	        String response = new String(receivePacket.getData());
	        //System.out.println("RESPONSE:" + receivePacket.getAddress().getHostAddress() + "\n" + response);
	        //sipSocket.close();
	        
	        if (response.toLowerCase().contains(call_id.toLowerCase()))
	        {
	        	//Para comprobar que ha contestado se comprueba que contiene el mismo call id
	        	ret = true;
	        }
	        else
	        {
	        	ret = false;
	        }
	        		
		}
		catch (Exception e)
		{
			ret = false;
		}
		
		return ret;
    }

    //Generating unique callID
    public static String generateCallId()
    {
       Random r = new Random();
       long l1 = r.nextLong() * r.nextLong();
       long l2 = r.nextLong() * r.nextLong();
       return Long.toHexString(l1) + Long.toHexString(l2);
    }

}
