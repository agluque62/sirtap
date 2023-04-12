package CheckProxy;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.PropertyConfigurator;

/*****************************************************************************************/
/* Checkea si el proxy esta vivo enviando OPTIONS
 * 
 */
/*****************************************************************************************/

public class Main {
	
	private static String VERSION = "CheckProxy Version 1.1";
	
	private static Log pluglog = LogFactory.getLog(Main.class);
	
	/**************************************************************************************************/
	/* Esta funcion sirve para inicia el log. Hay que llamarla al principio de cada funcion del plugin*/
	/**************************************************************************************************/
	static private synchronized void StarLog()
	{	
		PropertyConfigurator.configure("log4j.properties");
	}
	
	/**************************************************************************************************/
	/* Esta funcion sirve para agregar una traza en el log											*/
	/**************************************************************************************************/
	static public synchronized void Milog(String message)
	{
		pluglog.info(message);
	}
	
	/**************************************************************************************************/
	/* Esta funcion sirve para agregar una traza en el log											*/
	/**************************************************************************************************/
	static public synchronized void MylogError(String message)
	{
		pluglog.error(message);
	}
	
	/***********************************************************************************/
	/* Ejecuta un comando del sistema operativo
	 * Retorna la salida que se produce
	 */
	private static StringBuffer executeCommand(String command) {

		StringBuffer output = new StringBuffer();

		Process p;
		try {
			p = Runtime.getRuntime().exec(command);
			p.waitFor();
			BufferedReader reader =
                            new BufferedReader(new InputStreamReader(p.getInputStream()));

                        String line = "";
			while ((line = reader.readLine())!= null) {
				output.append(line + "\n");
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		return output;
	}	
	
	public static Properties config;
	private static int cuenta_de_fallos = 0;
	private static String ip_Virtual = null;

	public static void main(String[] args) {
		
		FileReader reader;
		StarLog();		
		
		Milog("--------------------- CheckProxy esta corriendo --------------");
		Milog(VERSION);
		
		//Leemos nuestro fichero de propiedades
		File configFile = new File("config.properties");			
		try {
			reader = new FileReader(configFile);
			config = new Properties();
			config.load(reader);
		} catch (Exception e1) {
			MylogError("ERROR: Leyendo \"config.properties\". ");
		}		
		
		int espera_inicial_count = 15;		//Esperamos a que arranque la Brekeke antes de chequear si está viva la Brekeke
		int max_cuenta_fallos = 4;			//Al principio esperamos a que arranque la brekeke para dar fallo
		
		cuenta_de_fallos = 0;
		while (true)
		{
			String ipvirt = Utilities.GetIpVirtual();	
			if (ipvirt == null && ip_Virtual != null)
			{
				Milog("Cambio en el estado de la IP virtual");
				VersionProxy.version_file_is_generated = false;
				ip_Virtual = ipvirt;
			}
			else if (ipvirt != null && ip_Virtual == null)
			{
				Milog("Cambio en el estado de la IP virtual");
				VersionProxy.version_file_is_generated = false;
				ip_Virtual = ipvirt;
			}
			else if (ipvirt != null && ip_Virtual != null)
			{
				if (!ipvirt.equals(ip_Virtual))
				{
					Milog("Cambio en el estado de la IP virtual. ipvirt: " + ipvirt + " ip_Virtual: "+ ip_Virtual);
					VersionProxy.version_file_is_generated = false;
					ip_Virtual = ipvirt;
				}	
			}
			
			boolean reiniciar_brekeke = VersionProxy.GenerateFileOfVersions();
			if (reiniciar_brekeke)
			{
				MylogError("Se ha excedido el numero de intentos para leer la version de la Brekeke por WebSocket");
				Milog("Se reinician los servicios heartbeat y tomcat");
				
				cuenta_de_fallos = 0;
				max_cuenta_fallos = 4;
				
				Reiniciar_Brekeke();
				
				//Esperamos a que la brekeke se arranque
				VersionProxy.version_file_is_generated = false;	
				espera_inicial_count = 15;
				continue;
			}
			
			try {
				Thread.sleep(5000);
			} catch (InterruptedException e) {
				e.printStackTrace();
				return;
			}
			
			if (espera_inicial_count > 0)
			{
				espera_inicial_count--;
				if (espera_inicial_count == 0)
				{
					Milog("---- Comienza chequeo del proxy ---");
				}
				continue;
			}
			
			CheckSipUdp check_brekeke = null;
			try {
				check_brekeke = new CheckSipUdp();
			} catch (Exception e) {
				MylogError("ERROR: No se puede crear CheckSipUdp "+ e.getMessage());
				continue;
			}
			
			//Leemos del fichero de propiedades de la brekeke el puerto SIP con el que esta configurada
			String sv_properties = config.getProperty("sv.properties");		//Tomamos el nombre del fichero que esta en nuestro fichero de propiedades
			if (sv_properties == null)
			{
				MylogError("ERROR: No se puede leer la propiedad sv_properties de config.properties ");
				continue;				
			}
			
			//Tomamos el puerto SIP que esta utilizando el SIP-proxy de brekeke de su fichero de propiedades
			Properties bkk_properties = null;
			File bkk_properties_file = new File(sv_properties);			
			try {
				FileReader bkk_reader = new FileReader(bkk_properties_file);
				bkk_properties = new Properties();
				bkk_properties.load(bkk_reader);
			} catch (Exception e1) {
				MylogError("ERROR: Leyendo " + sv_properties);
				continue;
			}
			
			String st_sip_bkk_port = bkk_properties.getProperty("net.sip.port.local");
			if (st_sip_bkk_port == null)
			{
				MylogError("ERROR: No se encuentra net.sip.port.local en " + bkk_properties_file);
				continue;				
			}
			
			int sip_bkk_port;
			try {
				sip_bkk_port = Integer.parseInt(st_sip_bkk_port);
			} catch (Exception e) {
				sip_bkk_port = 5060;
				Milog("ERROR: Valor no valido de net.sip.port.local en " + bkk_properties_file);
				continue;
			}
			
			if (!check_brekeke.check("127.0.0.1", sip_bkk_port))
			{							
				MylogError("El SIP proxy Brekeke no contesta a OPTIONS en el puerto " + sip_bkk_port);
				cuenta_de_fallos++;
				
				if ((cuenta_de_fallos == max_cuenta_fallos))
				{
					cuenta_de_fallos = 0;
					max_cuenta_fallos = 4;
					
					MylogError("Se ha excedido el numero de intentos cuenta_de_fallos="+cuenta_de_fallos);
					Milog("Se reinician los servicios heartbeat y tomcat");
					
					Reiniciar_Brekeke();
					
					//Esperamos a que la brekeke se arranque
					VersionProxy.version_file_is_generated = false;	
					espera_inicial_count = 15;
					continue;
				}
			}
			else
			{
				cuenta_de_fallos = 0;
				max_cuenta_fallos = 4;
			}
			
		}	
	}
	
	private static void Reiniciar_Brekeke()
	{
		executeCommand("/etc/init.d/tomcat stop");
		executeCommand("/etc/init.d/heartbeat restart");
		//Cuando para el servicio tomcat tarda unos 10 segundos hasta que se cierran sus aplicaciones
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			MylogError("Error al ejecutar sleep");
		}					
		
		VersionProxy.version_file_is_generated = false;
		VersionProxy.GenerateFileOfVersions();	//Generamos nuevo fichero para actualizar el estado del nodo
		
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			MylogError("Error al ejecutar sleep");
		}					
		
		VersionProxy.version_file_is_generated = false;
		VersionProxy.GenerateFileOfVersions();	//Generamos nuevo fichero para actualizar el estado del nodo
		
		try {
			Thread.sleep(30000);
		} catch (InterruptedException e) {
			MylogError("Error al ejecutar sleep");
		}
		executeCommand("/etc/init.d/tomcat start");
	}
	
}
