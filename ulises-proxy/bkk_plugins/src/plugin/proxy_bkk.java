package  plugin ;

import java.io.*;
import java.net.DatagramSocket;

//import java.util.Iterator;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import plugin.Utilities.*;

import java.util.* ;
import java.util.concurrent.Semaphore;

//Para COMPILAR
//Since the package is "plugin", you need a folder named "plugin". 

//1) mkdir plugin   (Es el nombre del package)

//2) mv sni.* plugin 

//3) javac -cp /usr/local/tomcat7/webapps/pbx/WEB-INF/lib/json_simple-1.1.jar:/usr/local/tomcat7/webapps/pbx/WEB-INF/lib/ondosip.jar plugin/sample.java 

//4) jar -cvf sample.jar plugin/sample.class

// cp sample.jar /usr/local/tomcat7/webapps/pbx/WEB-INF/lib/


public class proxy_bkk
{
	static private String VERSION = "proxy_bkk Version 1.4";
	static public String login_name_PBX = "sa";
	static public String login_pass_PBX = "sa";
	static public DependenciasSet dependenciasSet = new DependenciasSet();
	
	static McastThread runable = null;
	static private Thread hilomcasttifxStatus = null;
	
	/**************************************************************************************************/
	/* Funcion para arrancar el hilo de recepcion de mensajes por multicast	tifx									
	 *
	 */
	/**************************************************************************************************/ 
	static private synchronized void Arrancarmcasttifx()
	{
		try {
			boolean arrancar_hilo = false;
			if (hilomcasttifxStatus == null)
			{
				arrancar_hilo = true;						
			}
			else
			{
				boolean st = hilomcasttifxStatus.isAlive();
				if (!st)
				{
					arrancar_hilo = true;
				}				
			}
			
			if (arrancar_hilo)
			{
				String multicast_add;
				int multicast_port;
				
				multicast_add = Utilities.GetSetting("multicast_status_resources_address");
				if (multicast_add == null) multicast_add = "224.100.10.1";
				String multicast_port_st = Utilities.GetSetting("multicast_status_resources_port");
				if (multicast_port_st == null) multicast_port_st = "1001";
				multicast_port = Integer.parseInt(multicast_port_st);				
				
				runable = new McastThread(multicast_add, multicast_port);
				hilomcasttifxStatus = new Thread( runable, "hiloStatustifx" );
				Utilities.MyLog("hilomcasttifxStatus prioridad: " + hilomcasttifxStatus.getPriority());
				hilomcasttifxStatus.setPriority(Thread.MIN_PRIORITY);
				Utilities.MyLog("hilomcasttifxStatus prioridad: " + hilomcasttifxStatus.getPriority());
				hilomcasttifxStatus.start();
			}
		
		} catch (Exception e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			hilomcasttifxStatus = null;
			Utilities.MyLogError("Arrancarmcasttifx EXEPCION");
		}
	}
	
	static GenericThread GenericThreadrunable = null;
	static private Thread hiloGenerico = null;
	
	/**************************************************************************************************/
	/* Funcion para arrancar el hilo Generico									
	 *
	 */
	/**************************************************************************************************/ 
	static private synchronized void ArrancarHiloGenerico()
	{
		try {
			boolean arrancar_hilo = false;
			if (hiloGenerico == null)
			{
				arrancar_hilo = true;						
			}
			else
			{
				boolean st = hiloGenerico.isAlive();
				if (!st)
				{
					arrancar_hilo = true;
				}
			}
			
			if (arrancar_hilo)
			{
				login_name_PBX = Utilities.GetSetting("login_name_PBX");
				login_pass_PBX = Utilities.GetSetting("login_pass_PBX");
				if (login_name_PBX == null || login_pass_PBX == null)
				{
					Utilities.MyLogError("No se puede arrancar el hilo generico");
					hiloGenerico = null;
					return;
				}				
				
				Utilities.MyLog(VERSION);
				GenericThreadrunable = new GenericThread();
				hiloGenerico = new Thread( GenericThreadrunable, "hiloGenerico" );				
				Utilities.MyLog("hiloGenerico prioridad: " + hiloGenerico.getPriority());
				hiloGenerico.setPriority(Thread.MIN_PRIORITY);
				Utilities.MyLog("hiloGenerico prioridad: " + hiloGenerico.getPriority());
				hiloGenerico.start();
			}
		
		} catch (Exception e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			hiloGenerico = null;
			Utilities.MyLogError("ArrancarHiloGenerico EXEPCION");
		}
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke
	 * Funcion para arrancar un hilo generico con diferentes funciones. Por ejemplo realizar ciertas tareas de
	 * configuracion.
	 * Parametros de entrada:	 
	 * arg[0]. login name para conectarse por websocket a la PBX
	 * arg[1]. login password para conectarse por websocket a la PBX										
	 * Retorna true. 
	 */
	/**************************************************************************************************/
	static public boolean run_hilo_gen( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{		
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		//if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  runhilogen --------------");		
		
		ArrancarHiloGenerico();
		
		BrekekeProperties.Set(pr);
		
		//if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("SALE --------------  runhilogen --------------");
		return true;
		
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke
	 * Funcion para arrancar el hilo de recepcion de mensajes por multicast	tifx	
	 * Parametros de entrada. 
	 * arg[0]. IP multicast. 
	 * arg[1]. Puerto.		
	 * Si los parametros anteriores no estan presentes entonces se utiliza por defecto: 224.100.10.1:1001	 * 							
	 * Retorna true. 
	 */
	/**************************************************************************************************/
	static public boolean run_tifx( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();	
		Utilities.StartSettingsApp();
		//if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  arrtifx --------------");
				
		Arrancarmcasttifx();
		//if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("SALE --------------  arrtifx --------------");
		return true;
		
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Retorna true si el To contiene usuario y partenece a nuestro dominio. Es decir que el host (ip) es nuestro
	 */
	/**************************************************************************************************/
	static public boolean is_our_domain( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  is_our_domain --------------");
		
		uri_elements urie = Utilities.GetUriElements(sippacket.getValue("To"));
		if (urie == null)
		{
			return false;
		}
		
		if (urie.user == null)
		{
			return false;
		}
		
		if (Utilities.EsIpPropia(urie.ip, pr))
		{
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("IsOurDomain: To="+sippacket.getValue("To")+" va dirigido a nuestro dominio");
			return true;
		}
		
		return false;	
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Retorna true si el host del To partenece a nuestro dominio. Es decir que el host (ip) es nuestro
	 */
	/**************************************************************************************************/
	static public boolean is_host_our_domain( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  is_host_our_domain --------------");
		
		uri_elements urie = Utilities.GetUriElements(sippacket.getValue("To"));
		if (urie == null)
		{
			return false;
		}
		
		if (Utilities.EsIpPropia(urie.ip, pr))
		{
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("IsOurDomain: To="+sippacket.getValue("To")+" va dirigido a nuestro dominio");
			return true;
		}
		
		return false;	
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Retorna true si el host del To partenece a nuestro dominio. Es decir que el host (ip) es nuestro
	 */
	/**************************************************************************************************/
	static public String our_domain_addr( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  our_domain_addr --------------");
		
		String ip_bkk = pr.getProperty("net.net1.interface");
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("our_domain_addr="+ip_bkk);
		return ip_bkk;
	}
	
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Se llama cuando se quiere encaminar un usuario, por ejemplo en transito.
	 * Retorna una lista saparada por espacios de todas las uris por la que se encamina
	 */
	/**************************************************************************************************/
	static public String encaminar( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  encaminar --------------");
		
		uri_elements urie = Utilities.GetUriElements(sippacket.getValue("To"));
		if (urie == null)
		{
			return null;
		}
				
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Encaminar: user "+urie.user);
		ArrayList<String> enc = SoapConfig.GetEnc(urie.user);
		
		if (enc != null && !enc.isEmpty())
		{
			String ret = "";
			for (int i = 0; i < enc.size(); i++)
			{
				ret += enc.get(i)+" ";
			}
			
			Utilities.MyLog("encaminar: Encamina "+urie.user+" hacia "+ret);
			
			return ret;
		}
		return null;
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Extrae la primera uri obtenida en Encaminar para mandarla con el To
	 * Hay que pasar como parametro la salida obtenida por Encaminar
	 */
	/**************************************************************************************************/
	static public String get_to_from_encaminar( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  get_to_from_encaminar --------------");
		
		if (arg.length == 0) return null;
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Resultado encamina: "+arg[0]);
		
		String ret;
		//Busca la primera uri
		int endIndex = arg[0].indexOf(" ");
		if (endIndex > 0) ret = arg[0].substring(0, endIndex);
		else ret = arg[0];
		
		Utilities.MyLog("get_to_from_encaminar: parametro entrada: "+ arg[0]+" To: "+ret);
		
		return ret;
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Extrae las uris menos la primera de las obtenidas en Encaminar para el FailOver
	 * Hay que pasar como parametro la salida obtenida por Encaminar
	 */
	/**************************************************************************************************/
	static public String get_failover_from_encaminar( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  get_failover_from_encaminar --------------");
		
		if (arg.length == 0) return null;
		
		String ret;
		//Busca la primera uri
		int endIndex = arg[0].indexOf(" ");
		if (endIndex > 0) ret = arg[0].substring(endIndex);
		else
		{
			ret = " ";
			return ret;
		}
		
		if (ret.length() < "sip:".length())
		{
			ret = " ";
			return ret;
		}
		
		//Saltamos el espacio en blanco
		ret = ret.substring(1);
		
		//En los argumentos del failover convertimos las uris para enviarlas a localhost
		//Ponemos toda la uri en el user convirtiendo los caracteres especiales a caracteres no especiales.
		//Esto lo hacemos porque el failover quita los uri-parameter. 
		//Como se ha visto que cuando se envia con el To esto si funciona entonces, lo que hacemos es enviarlo asi al localhost y
		//en otra regla (recover_uri_from_fileover) volveremos a convertir la uri a una correcta y lo enviamos con el To.
		//Convertimos loa caracteres reservados a cadenas
		//Estos son los caracteres reservados segun la rfc3986
		/*
		 * ":" / "/" / "?" / "#" / "[" / "]" / "@"

      		"!" / "$" / "&" / "'" / "(" / ")"
                  / "*" / "+" / "," / ";" / "="
		 * 
		 */
		
		ret = ret.replace(">", "_%3E-");
		ret = ret.replace("<", "_%3C-");
		ret = ret.replace(":", "_%3A-");
		ret = ret.replace("/", "_%2F-");
		ret = ret.replace("?", "_%3F-");
		ret = ret.replace("#", "_%23-");
		ret = ret.replace("[", "_%5B-");
		ret = ret.replace("]", "_%5D-");
		ret = ret.replace("@", "_%40-");
		ret = ret.replace("!", "_%21-");
		ret = ret.replace("$", "_%24-");
		ret = ret.replace("&", "_%26-");
		ret = ret.replace("'", "_%27-");
		ret = ret.replace("(", "_%28-");
		ret = ret.replace(")", "_%29-");
		ret = ret.replace("*", "_%2A-");
		ret = ret.replace("+", "_%2B-");
		ret = ret.replace(",", "_%2C-");
		ret = ret.replace(";", "_%3B-");
		ret = ret.replace("=", "_%3D-");
		
		String[] separadas = ret.split(" ");
		
		//Obtenemos la ip que esta configurada en el primer interface de la brekeke
		//Para obtener todas las propiedades de pr he pintado pt.toString y asi he obtenido net.net1.interface
		String ip_bkk = pr.getProperty("net.net1.interface");
		if (ip_bkk == null)
		{
			ip_bkk = "127.0.0.1"; 		
		}
		
		ret = "";
		for (int i = 0; i < separadas.length; i++)
		{			
			//ret += "sip:failover_" + separadas[i] + "@localhost ";
			ret += "sip:failover_" + separadas[i] + "@" + ip_bkk + " ";
		}				
		
		Utilities.MyLog("get_failover_from_encaminar: parametro entrada: "+ arg[0]+" failover: "+ret);
		return ret;
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Convierte la uri generada en get_failover_from_encaminar por la correcta 
	 */
	/**************************************************************************************************/
	static public String recover_uri_from_fileover( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  recover_uri_from_fileover --------------");
		
		uri_elements urie = Utilities.GetUriElements(sippacket.getValue("To"));
		if (urie == null)
		{
			return null;
		}
		
		String useruri = urie.user;
		useruri = useruri.replaceFirst("failover_", "");
		
		useruri = useruri.replace("_%3E-", ">");
		useruri = useruri.replace("_%3C-", "<");
		useruri = useruri.replace("_%3A-", ":");
		useruri = useruri.replace("_%2F-", "/");
		useruri = useruri.replace("_%3F-", "?");
		useruri = useruri.replace("_%23-", "#");
		useruri = useruri.replace("_%5B-", "[");
		useruri = useruri.replace("_%5D-", "]");
		useruri = useruri.replace("_%40-", "@");
		useruri = useruri.replace("_%21-", "!");
		useruri = useruri.replace("_%24-", "$");
		useruri = useruri.replace("_%26-", "&");
		useruri = useruri.replace("_%27-", "'");
		useruri = useruri.replace("_%28-", "(");
		useruri = useruri.replace("_%29-", ")");
		useruri = useruri.replace("_%2A-", "*");
		useruri = useruri.replace("_%2B-", "+");
		useruri = useruri.replace("_%2C-", ",");
		useruri = useruri.replace("_%3B-", ";");
		useruri = useruri.replace("_%3D-", "=");
		
		Utilities.MyLog("recover_uri_from_fileover: To entrante: "+sippacket.getValue("To") +" salida: "+useruri);
		
		return useruri;
	}	
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Retorna true. Si el destino u origen es telefono de seguridad 
	 */
	/**************************************************************************************************/
	static public boolean is_tlf_sec( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  istlfsec --------------");
		
		boolean ret = false;		
		String valueFrom = sippacket.getValue( "From" );
		uri_elements urie = Utilities.GetUriElements(valueFrom);
		if (urie == null) return false;
		String from_user = urie.user;
		if (from_user == null) from_user = "";
		
		String valueTo = sippacket.getValue( "To" );
		urie = Utilities.GetUriElements(valueTo);
		if (urie == null) return false;
		String to_user_ = urie.user;
		if (to_user_ == null) to_user_ = "";
		String to_user;
		
		/*A to_user le quitamos un posible prefijo 0*, que indica que se quiere realizar una conferencia*/
		/*Esto se utiliza en la intrusion*/
		/*Nos quedamos con el substring de detras del asterisco */ 
		int index_ast = to_user_.indexOf("*");
		if ((index_ast != -1) && (to_user_.length() > 1) && (index_ast < (to_user_.length()-1)))
		{
			to_user = to_user_.substring(index_ast+1);			
		}
		else
		{
			to_user = to_user_;			
		}
		
		JSONParser parser = new JSONParser();
		try {
			 
			Object obj = parser.parse(new FileReader(Utilities.JsonProxyFilename));	 
			JSONObject jsonObject = (JSONObject) obj;
			
			JSONArray tel_sec_array = (JSONArray) jsonObject.get("telefonos_seguridad");

	        for (int i = 0; i < tel_sec_array.size(); i++) 
	        {        	
	        	JSONArray rango_tlf_sec = (JSONArray) tel_sec_array.get(i);
	        	if (rango_tlf_sec.size() == 2)
        		{
        			//Es un rango
        			String menor = (String) rango_tlf_sec.get(0);
        			String mayor = (String) rango_tlf_sec.get(1);
        			
        			if 	((to_user.compareTo(menor) >= 0) &&  //to_user >= menor
        				 (to_user.compareTo(mayor) <= 0))	//to_user <= mayor
        			{
        				//El usuario del To esta en el rango
        				ret = true;
        				break;
        			} 
        			if 	((from_user.compareTo(menor) >= 0) &&  //to_user >= menor
           				 (from_user.compareTo(mayor) <= 0))	//to_user <= mayor
           			{
        				//El usuario del from esta en el rango
        				ret = true;
        				break;
           			}
        		}
        		else if (rango_tlf_sec.size() == 1)
        		{
        			//Es un numero sumple
        			String num = (String) rango_tlf_sec.get(0);
        			if (to_user.equals(num) || from_user.equals(num))
        			{
        				ret = true;
        				break;
        			}
        		} 
	        }
		} catch (Exception e) {
			Utilities.MyLogError("istlfsec excepcion "+e.getMessage());
			return false;
		}
		
		if (ret)
		{ 
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("istlfsec: "+from_user+" o "+ to_user_+" son telefonos de seguridad");
			return true;
		}
		else
		{
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("istlfsec: Ni "+from_user+" ni "+ to_user_+" son telefonos de seguridad");
			return false;
		}		
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Crea uri from con parametro cd40rs desde el from cuyo usuario empieza por cd40rs###
	 * 
	 */
	/**************************************************************************************************/
	static public String set_from_cd40rs( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		String valueFrom = sippacket.getValue("From");
		String ret = valueFrom;
		uri_elements urie_from = Utilities.GetUriElements(valueFrom);
		
		if (urie_from.user.contains("cd40rs###"))
		{
			int index1 = urie_from.user.indexOf("cd40rs###");
			index1 += 9;
			int index2 = urie_from.user.indexOf("###", index1);
			String rec = urie_from.user.substring(index1, index2);
			int index3 = index2 + 3;
			String from_user = urie_from.user.substring(index3);
			ret = "<sip:"+from_user+"@"+urie_from.ip;
			if (urie_from.port != null) ret += ":"+urie_from.port;
			ret += ";"+"cd40rs="+rec+">";
		}
		
		
		return ret;
	}
		
	static String[] Uris_no_registradas = null;
	static final int TOTAL_Uris_no_registradas = 2048;
	static int Index_Uris_no_registradas = 0;
	static Semaphore mutex_Uris_no_registradas = null;
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Almacena en un buffer circular las uris que no estan registradas
	 */
	/**************************************************************************************************/
	static public boolean save_non_registered( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		if (Uris_no_registradas == null)
		{
			Uris_no_registradas = new String[TOTAL_Uris_no_registradas];
			Index_Uris_no_registradas = 0;
			mutex_Uris_no_registradas = new Semaphore(1);
		}
		
		String uri_no_registrada = null;
		if (arg[0].equals("From"))
		{
			//El que no esta registrado es el from		
			uri_no_registrada = sippacket.getValue("From");
		}
		else if (arg[0].equals("To"))
		{
			//El que no esta registrado es el To
			uri_no_registrada = sippacket.getValue("To");
		}
		
		if (uri_no_registrada != null)
		{
			uri_elements urie = Utilities.GetUriElements(uri_no_registrada);
			if (urie.user == null)
			{
				uri_no_registrada = null;
			}
			else
			{
				if (uri_no_registrada.contains("tag="))
				{
					int ind = uri_no_registrada.indexOf(";tag=");
					uri_no_registrada = uri_no_registrada.substring(0, ind);
				}
			}
		}
		
		if (uri_no_registrada != null)
		{
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ADD URI NO REGISTRADA "+uri_no_registrada);
			mutex_Uris_no_registradas.acquire();
			Uris_no_registradas[Index_Uris_no_registradas] = uri_no_registrada;
			Index_Uris_no_registradas++;
			if (Index_Uris_no_registradas == TOTAL_Uris_no_registradas) 
			{
				Index_Uris_no_registradas = 0;
			}
			mutex_Uris_no_registradas.release();
		}		
		
		return true;
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Busca en la lista de uris no registradas la que coincida el usuario
	 * La uri se pasa como argumento.
	 * REtorna la nueva uri
	 */
	/**************************************************************************************************/
	static public String get_non_registered( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		String ret = arg[0];	
		if (Uris_no_registradas == null)
		{
			return ret;
		}
		
		uri_elements urie = Utilities.GetUriElements(arg[0]);
		String user = urie.user;
		if (user != null)
		{
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("BUSCA URI NO REGISTRADA DEL USUARIO "+user);
			//REcorremos la lista de la posicion mas reciente hacia atras 
			mutex_Uris_no_registradas.acquire();
			
			int index = Index_Uris_no_registradas;
			boolean found = false;
			for (int i = 0; (i < Index_Uris_no_registradas) && !found; i++)
			{
				if (index == 0) index = TOTAL_Uris_no_registradas - 1;
				else index--;
				
				if ((Uris_no_registradas[index] != null) && (Uris_no_registradas[index].length() > 0))
				{
					uri_elements uri = Utilities.GetUriElements(Uris_no_registradas[index]);
					if (user.equals(uri.user))
					{
						ret = Uris_no_registradas[index];
						found = true;
					}
				}
			}	
			
			mutex_Uris_no_registradas.release();
			if (found)
			{
				if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENCUENTRA URI NO REGISTRADA DE "+user+" "+ret);
			}
		}		
		
		return ret;
	}
	
	/**************************************************************************************************/
	/* Funcion pluging de brekeke											
	 * Retorna true si el servidor de presencia esta presente.
	 * Se pasa como argumento el puerto del servidor de presencia
	 * 
	 */
	/**************************************************************************************************/
	static public boolean is_pres_srv_alive( String[] arg, com.brekeke.net.sip.SIPpacket  sippacket, Properties pr ) throws Exception
	{
		Utilities.StarLog();
		Utilities.StartSettingsApp();
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("ENTRA --------------  is_pres_srv_alive --------------");
		
		boolean ret = true;
		
		if (arg == null)
		{						
			Utilities.MyLogError("is_pres_srv_alive: El argumento no es valido. Tiene que ser valor numerico correspondente a un puerto");
			return false;
		}
		
		if (arg.length == 0)
		{			
			
			Utilities.MyLogError("is_pres_srv_alive: El argumento no es valido. Tiene que ser valor numerico correspondente a un puerto");
			return false;
		}
		
		int puerto;
		
		try
		{
			puerto = Integer.parseInt(arg[0]);
		}
		catch (Exception e)
		{
			Utilities.MyLogError("is_pres_srv_alive: El argumento no es valido. Tiene que ser valor numerico correspondente a un puerto");
			return false;			
		}
		
		//Comprobamos si podemos abrir el puerto. Si no entonces es que el pres_srv esta vivo.		
		DatagramSocket s = null;
	    try {
	        s = new DatagramSocket(puerto);

	        // If the code makes it this far without an exception it means
	        // something is using the port and has responded.
	        Utilities.MyLogError("pres_srv esta muerto. Puerto "+ puerto);
	        s.close();
	        ret = false;
	    } catch (Exception e) {
	    	if (Utilities.LOGDEBENABLED) Utilities.MyLogDeb("pres_srv esta vivo. Puerto " + puerto);
	    		
	    }
	    
	    return ret;
	}
	
}
