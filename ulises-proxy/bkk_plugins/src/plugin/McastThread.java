package plugin;

import java.net.*;

public class McastThread implements Runnable 
{
	Thread t;
	
	public String multicast_add = "224.100.10.1";
	public int multicast_port = 1001;
	private boolean run_th = true;
	private MulticastSocket socket = null;
	
    public McastThread(String multicast_add, int multicast_port) {
       this.multicast_add = multicast_add;
       this.multicast_port = multicast_port;
    }
    
    public void parar()
    {
    	run_th = false;
    	socket.close();
    }
	
    public void run() 
    {
    	Utilities.MyLog("---------RUN MCAST TIFX--------");
        // Ejecucion del thread una vez creado
    	InetSocketAddress localAdd;
		try {
			localAdd = new InetSocketAddress(InetAddress.getByName(multicast_add), multicast_port);
			socket = new MulticastSocket(multicast_port);
			NetworkInterface nic = NetworkInterface.getByName("eth0");
			socket.joinGroup(localAdd, nic);
			socket.setSoTimeout(60000); //Timeout a recibir de 60 segundos
			
			byte[] buf = new byte[2048];
			DatagramPacket recv = new DatagramPacket(buf, buf.length);
	    	while (run_th)
	    	{  	
	    		try
	    		{
	    			socket.receive(recv);				
	    			proxy_bkk.dependenciasSet.Add(buf, recv.getLength());
	    			proxy_bkk.dependenciasSet.Limpiar();	    			
	    		}
	    		catch (SocketTimeoutException e) {
	    			//Despues del timeout procedemos a limpiar las dependencias si procede
					proxy_bkk.dependenciasSet.Limpiar();	
				}
	    		Thread.yield();
	    	}
		} catch (Exception e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("McastThread. excepcion");
	    }
		
		try {
			if (socket != null) socket.close();
		} catch (Exception e) {
			e.printStackTrace();
	    }	
    }
}
