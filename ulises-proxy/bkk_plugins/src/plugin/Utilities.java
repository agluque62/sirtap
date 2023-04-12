package plugin;

import java.util.*;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.PropertyConfigurator;
import org.json.simple.*;

import java.io.*;
//import java.net.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Utilities {
	
	//Nombre del fichero de propiedades de configuracion de este plugin
	static public final String ConfigPropertiesFileName = "/root/proxy_ulises/plugin_brekeke/config.properties";
	
	//Nombre del fichero de propiedades de Log de este plugin
	static public final String Log4jPropertiesFileName = "/root/proxy_ulises/plugin_brekeke/log4j.properties";
	
	//nombre del fichero donde se almacena la configuracion del Ulises.
	static public final String UlisesConfigFileName = "/root/proxy_ulises/plugin_brekeke/UlisesConfig.json";
	
	static private final String DefaultJsonProxyFilename = "/home/user/proxy.json";
	static public String JsonProxyFilename = DefaultJsonProxyFilename;
	
	private static Properties appsettings;
	private static FileReader reader;
	private static boolean settings_initizalized = false;	
	
	private static boolean GetLogPropeties_done = false;
	
	static private boolean logger_creado = false;
	static public boolean LOGINFOENABLED = true;   //Si false entonces no se producen logs
	static private boolean LOGERRORENABLED = true;   //Si false entonces no se producen logs
	static public boolean LOGDEBENABLED = true;   //Si false entonces no se producen logs
	private static Log pluglog = LogFactory.getLog(proxy_bkk.class);
	
	/**************************************************************************************************/
	/* Inicializa la lectura del fichero de settings de la aplicacion							  */
	/**************************************************************************************************/
	static public synchronized void StartSettingsApp()
	{	
		if (settings_initizalized) return; 
		String settings_file = Utilities.ConfigPropertiesFileName;
		StarLog();
		File configFile = new File(settings_file);			
		try {
			reader = new FileReader(configFile);
		} catch (FileNotFoundException e) {
			MyLogError("No se puede leer "+ settings_file);
			return;
		}
		appsettings = new Properties();
		try {
			appsettings.load(reader);
		} catch (IOException e) {
			MyLogError("No se pueden cargar los settings de "+ settings_file);
			return;
		}	
		
		JsonProxyFilename = GetSetting("FicheroJsonConfiguracion");
		if (JsonProxyFilename == null) JsonProxyFilename = DefaultJsonProxyFilename;
		
		settings_initizalized = true;
	}
	
	/**************************************************************************************************/
	/* Obtiene un campo del fichero de Settings 
	 * 
	 */
	static public String GetSetting(String setting)
	{
		if (!settings_initizalized) return null;
		try {
			appsettings.load(reader);
		} catch (IOException e) {
			MyLogError("No se pueden cargar los settings");
			return null;
		}
		String ret = appsettings.getProperty(setting);
		if (ret == null)
		{
			MyLogError("No se encuentra la propiedad en el fichero de settings: "+ setting);
		}
		return ret;
	}
	
	/**************************************************************************************************/
	/* Esta funcion sirve para inicia el log. Hay que llamarla al principio de cada funcion del plugin*/
	/**************************************************************************************************/
	static public synchronized void StarLog()
	{	
		if (!logger_creado)
		{
			PropertyConfigurator.configure(Utilities.Log4jPropertiesFileName);
			logger_creado = true;
		}
	}
	
	/**************************************************************************************************/
	/* Esta funcion sirve para agregar una traza en el log											*/
	/**************************************************************************************************/
	static public synchronized void MyLogDeb(String message)
	{
		if (!LOGDEBENABLED) return;
		if (logger_creado) pluglog.debug(message);
	}
	
	/**************************************************************************************************/
	/* Esta funcion sirve para agregar una traza en el log											*/
	/**************************************************************************************************/
	static public synchronized void MyLog(String message)
	{
		if (!LOGINFOENABLED) return;
		if (logger_creado) pluglog.info(message);
	}
	
	/**************************************************************************************************/
	/* Esta funcion sirve para agregar una traza en el log											*/
	/**************************************************************************************************/
	static public synchronized void MyLogError(String message)
	{
		if (!LOGERRORENABLED) return;
		if (logger_creado) pluglog.error(message);
	}
	
	/**************************************************************************************************/
	/* Esta funcion sirve para agregar una traza en el log											*/
	/**************************************************************************************************/
	static public synchronized void MyLogStackTrace(Exception e)
	{
		if (!LOGERRORENABLED) return;
		ByteArrayOutputStream stream = new ByteArrayOutputStream();				
        PrintStream ps = new PrintStream(stream); 
        e.printStackTrace(ps);
        String finalString = new String(stream.toByteArray());
        Utilities.MyLogError(finalString);
	}
	
	/***********************************************************************************/
	/* Ejecuta un comando del sistema operativo
	 * Retorna la salida que se produce
	 */
	public static StringBuffer executeCommand(String command) {

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
	
	public static boolean validateIPV4(final String ip) {
	    String PATTERN = "^((0|1\\d?\\d?|2[0-4]?\\d?|25[0-5]?|[3-9]\\d?)\\.){3}(0|1\\d?\\d?|2[0-4]?\\d?|25[0-5]?|[3-9]\\d?)$";

	    return ip.matches(PATTERN);
	}
	
	/***********************************************************************************/
	/* Retorna la IpVirtual de la máquina virtual Linux.
	 * Si no tiene activa la Ip virtual, entonces retorna null
	 */
	public static String GetIpVirtual()
	{
		/*
		
		String ret = null;
		
		try {
			NetworkInterface nic = NetworkInterface.getByName("eth0");
			Enumeration<NetworkInterface> subinterfaces = nic.getSubInterfaces();  //Buscamos eth0:0, la cual tiene la ip virtual
			
			String net_sip_mirroring_addr = BrekekeProperties.Get_net_sip_mirroring_addr();
			if (net_sip_mirroring_addr != null)
			{
				//La version de Brekeke implementa redundancia
				//Buscamos en las ips del interfaz si tiene la direccion virtual del mirroring
				Enumeration<InetAddress> inet_addresses = nic.getInetAddresses();
				while (inet_addresses.hasMoreElements())
				{
					InetAddress inetaddress = inet_addresses.nextElement();
					if (inetaddress.getHostAddress().equals(net_sip_mirroring_addr))
					{
						ret = net_sip_mirroring_addr;
						break;
					}				
				}
			}
			else
			{
				//La version de Brekeke no implementa redundancia, la cual la implementa Heartbeat
				//En este caso la ip virtual esta en el subinterface eth0:0
				while (subinterfaces.hasMoreElements())
				{
					NetworkInterface interf = subinterfaces.nextElement();
					if (interf.getName().equals("eth0:0"))
					{
						List<InterfaceAddress> int_list = interf.getInterfaceAddresses();
						for (int i = 0; i < int_list.size(); i++)
						{
							ret = int_list.get(i).getAddress().getHostAddress();
						}
					}				
				}
			}
		} catch (Exception e) {
			MyLogError(e.toString()+" "+e.getMessage());
			MyLogError("GetIpVirtual: No se pueden leer la interfaz eth0 ni eth0:0");
			MyLogStackTrace(e);
			ret = null;
		}
		
		if (ret != null)
		{
			if (validateIPV4(ret) == false)
			{
				ret = null;
			}
		}
		
		return ret;
		
		*/
		
		/*Invocando executecommand*/
		String ret = null;
		try
		{
			StringBuffer output = executeCommand("ifconfig eth0:0");
			int ind = output.indexOf("inet addr:");
			
			if (ind != -1)
			{
				//Extraemos la ip virtual
				int indstart = output.indexOf(":", ind);
				indstart += 1;
				
				int indend = output.indexOf(" ", indstart);
				ret = output.substring(indstart, indend);
			}
		}
		catch (Exception e)
		{
			ret = null;
		}
		
		return ret;
	}
	
	/***********************************************************************************/
	public static boolean EsIpPropia(String ip, Properties pr)
	{	
		//Obtenemos la ip que está configurada en el primer interface de la brekeke
		//Para obtener todas las propiedades de pr he pintado pr.toString y asi he obtenido net.net1.interface
		String ip_bkk = pr.getProperty("net.net1.interface");
		if (ip_bkk != null)
		{
			return ip.equals(ip_bkk);			
		}
		else
		{
			return false;			
		}		
		
		/*String ip_virtual = GetIpVirtual();
		if (ip_virtual != null)
		{
			return ip.equals(ip_virtual);			
		}
		else
		{
			return false;
		}*/
		
//		try {
//			NetworkInterface nic = NetworkInterface.getByName("eth0");
//			List<InterfaceAddress> int_list = nic.getInterfaceAddresses();
//			for (int i = 0; i < int_list.size(); i++)
//			{
//				if (ip.equals(int_list.get(i).getAddress().getHostAddress()))
//				{
//					return true;
//				}				
//			}	
//			
//		} catch (SocketException e) {
//			MyLogError(e.toString()+" "+e.getMessage());
//			MyLogError("No se pueden leer la interfaz eth0");
//		}
//		return false;
	}
	
	
	public static class uri_elements
	{
		String user = null;
		String ip = null;
		String port = null;		
	}
	
	/**************************************************************************************************/
	/* Extrae elementos de una uri. Si hay error retorna null										*/
	/**************************************************************************************************/
	static public uri_elements GetUriElements(String uri)
	{
		uri_elements ret = new uri_elements();
		try
		{			
			//Buscamos "sip:"
			String sip = "sip:";
			boolean found = false;
			String uri_aux = uri;
			for (int i = 0; i < (uri.length()-sip.length()); i++)
			{
				uri_aux = uri.substring(i);
				if (uri_aux.startsWith(sip))
				{
					found = true;
					break;		
				}
			}
			if (!found) return null;		
			
			int index_user = uri_aux.indexOf(':');
			if (index_user == -1) return null;
			index_user += 1;
			int index_arroba = uri_aux.indexOf('@');
			int index_ip;
			int index_port;
			if (index_arroba == -1) 
			{
				//La uri no tiene usuario
				ret.user = null;
				index_ip = index_user;
				index_port = uri_aux.indexOf(':', index_ip);
			}
			else
			{
				if (index_user >= index_arroba) return null;
				ret.user = uri_aux.substring(index_user, index_arroba);	//Usuario
				if (ret.user.length() == 0) return null;
				index_ip = index_arroba + 1;
				index_port = uri_aux.indexOf(':', index_arroba);
			}
			
			int index_ip_end = index_ip;
			for (int i = index_ip; i < uri_aux.length(); i++)
			{
				char c = uri_aux.charAt(i); 
				if (c == ' ' || c == ';' || c == ':' || c == '>')
				{
					index_ip_end = i;
					break;
				}
			}
			if (index_ip_end == index_ip) ret.ip = uri_aux.substring(index_ip);
			else ret.ip = uri_aux.substring(index_ip, index_ip_end);
			
			if (index_port != -1)
			{
				//hay puerto
				ret.port = uri_aux.substring(index_port+1);
				//En el string ret.port se busca un posible caracter no numerico
				int ifound = -1;
				for (int i = 0; i < ret.port.length(); i++) {
		            if (!Character.isDigit(ret.port.charAt(i)))
		            {
		            	ifound = i;
		            	break;
		            }
		        }
				if (ifound != -1)
				{
					//El string del puerto es hasta el caracter no numerico
					ret.port = ret.port.substring(0, ifound);
				}
			}
			else
			{				
				ret.port = null;
			}
		}
		catch (IndexOutOfBoundsException e)
		{
			return null;
		}
		return (ret);		
	}
	
	/**************************************************************************************************/
	/* Extrae el prefijo de red del usuario. Si hay error retorna null										*/
	/**************************************************************************************************/
	static public String GetRedPrefixFromUser(String user)
	{
		if (user.length() < 3) 
		{
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Usuario:"+user+" no tiene prefijo de red, la longitud es solo de "+user.length());
			return null;
		}
		
		if (!user.startsWith("0"))
		{
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Usuario:"+user+" no tiene un prefijo de red que empiece por cero");
			return null;			
		}
		
		String red = user.substring(0, 2);
		return red;
	}
	
	/**************************************************************************************************/
	/* Busca un valor en un array json. Retorna null si no lo encuentra 							
	 * Parametros:
	 * obj. Objeto json en el que se encuentra el array
	 * arrayName. Identificador del array.
	 * fielName. En un elemento del array, es el identificador del elemento que queremos buscar. 
	 * 			 Si es null, entonces los elementos del array solo tienen un campo sin identificador.
	 * value2find. String a buscar.
	 * Retorna el objeto JSON que lo contiene. Si fieldName es null entonces retorna el objeto JSON del array.
	 */
	/**************************************************************************************************/
	static public JSONObject BuscaValorJsonArray(JSONObject obj, String arrayName, String fieldName, String value2find)
	{		
		JSONArray jsonarray = (JSONArray) obj.get(arrayName);	
		JSONObject obj_ret = null;	
		JSONObject item = null;

        for (int i = 0; i < jsonarray.size(); i++) {        	
        	String item_value;
        	if (fieldName == null)
        	{
        		item_value = (String) jsonarray.get(i);
        	}
        	else
        	{
        		item = (JSONObject) jsonarray.get(i);
        		item_value = (String) item.get(fieldName);
        	}
        	
        	if (item_value.equals(value2find))
        	{
        		if (fieldName == null) obj_ret = obj;
        		else obj_ret = item;
        		break;
        	}
        }
        
        return (obj_ret);    
	}
	
	/**************************************************************************************************/
	/* Lee los niveles de log del fichero de configuracion del plugin 							
	 */
	/**************************************************************************************************/
	static public void GetLogPropeties()
	{
		if (GetLogPropeties_done == true) return; 
		StartSettingsApp();
		
		String LogErrorEnabled = GetSetting("LogErrorEnabled");
		String LogInfoEnabled = GetSetting("LogInfoEnabled");
		String LogDebugEnabled = GetSetting("LogDebugEnabled");
		
		if (LogErrorEnabled != null && LogErrorEnabled.startsWith("0")) LOGERRORENABLED = false;
		else LOGERRORENABLED = true;
		if (LogInfoEnabled != null && LogInfoEnabled.startsWith("0")) LOGINFOENABLED = false;
		else LOGINFOENABLED = true;
		if (LogDebugEnabled != null && LogDebugEnabled.startsWith("0")) LOGDEBENABLED = false;
		else LOGDEBENABLED = true;
		
		GetLogPropeties_done = true;
	}	
	
	/**************************************************************************************************/
	/* Obtiene el MD5 de un fichero
	 * Retorna null si se produce un error 							
	 */
		
	public static String byteToHex(byte num) {
	    char[] hexDigits = new char[2];
	    hexDigits[0] = Character.forDigit((num >> 4) & 0xF, 16);
	    hexDigits[1] = Character.forDigit((num & 0xF), 16);
	    return new String(hexDigits);
	}
	
	public static String encodeHexString(byte[] byteArray) {
	    StringBuffer hexStringBuffer = new StringBuffer();
	    for (int i = 0; i < byteArray.length; i++) {
	        hexStringBuffer.append(byteToHex(byteArray[i]));
	    }
	    return hexStringBuffer.toString();
	}
	
	public static String GetMD5(String path)
	{
		String checksum = null;
		FileInputStream fis = null;
        try {
            fis = new FileInputStream(path);
            MessageDigest md = MessageDigest.getInstance("MD5");
          
            //Using MessageDigest update() method to provide input
            byte[] buffer = new byte[8192];
            int numOfBytesRead;
            while (true)
            {
            	numOfBytesRead = fis.read(buffer);
            	if (numOfBytesRead > 0)
            		md.update(buffer, 0, numOfBytesRead);
            	else
            		break;
            }
            byte[] hash = md.digest();
            checksum = encodeHexString(hash);
        } catch (IOException ex) {
        	MyLogError(ex.toString()+" "+ex.getMessage());
			MyLogError("GetMD5: No se pueden acceder al fichero "+path);
        } catch (NoSuchAlgorithmException ex) {
        	MyLogError(ex.toString()+" "+ex.getMessage());
			MyLogError("GetMD5: NoSuchAlgorithmException MD5 "+path);
        }
        
        if (fis != null)
        {
        	try {
				fis.close();
			} catch (IOException e) {
			}
        }
          
       return checksum;
    }



}
