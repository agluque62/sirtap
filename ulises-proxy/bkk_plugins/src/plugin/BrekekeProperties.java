package plugin;

import java.util.Properties;
import java.util.concurrent.Semaphore;

public class BrekekeProperties {
	
	//Esta clase contiene propiedades de configuracion de la Brekeke que son leidas cada vez que 
	//run_hilo_gen es llamado
	
	public static String net_sip_mirroring_use = "false";
	public static String net_sip_mirroring_addr = " ";		//Direccion ip que asigna el mirroring	
	public static String net_net1_interface = " ";			//Direccion ip en Interface address 1 (net.net1.interface)
	
	private static Semaphore sem = new Semaphore(1);
	
	
	static public void Set(Properties bkk_properties)
	{
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("BrekekeProperties.Set. No puede adquirir el semaforo ");
			return;
		}
		
		net_sip_mirroring_use = bkk_properties.getProperty("net.sip.mirroring.use");
		net_sip_mirroring_addr = bkk_properties.getProperty("net.sip.mirroring.addr");
		net_net1_interface = bkk_properties.getProperty("net.net1.interface");
		
		sem.release();		
	}
	
	static public String Get_net_sip_mirroring_addr() 
	{
		String ret = null;
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("BrekekeProperties.Set. No puede adquirir el semaforo ");
			return ret;
		}
		
		if (net_sip_mirroring_use.equals("true"))
		{
			//Solamente retornamos la ip del mirroring si el mirroring esta activado
			ret = net_sip_mirroring_addr;
		}
		
		sem.release();
		
		return ret;
	}
	
	static public String Get_net_net1_interface()
	{
		String ret = " ";
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("BrekekeProperties.Set. No puede adquirir el semaforo ");
			return ret;
		}
		
		ret = net_net1_interface;
		
		sem.release();
		
		return ret;
	}

}
