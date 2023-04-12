package CheckProxy;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.nio.file.attribute.BasicFileAttributes;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import com.fasterxml.jackson.annotation.JsonAutoDetect.Visibility;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.databind.ObjectMapper;

public class VersionProxy {
	/**************************************************************************************************************/
	/*Esta clase sirve para obtener el fichero de version del proxy para que Ulises lo tome*/
	/**************************************************************************************************************/
	
	static class FileAttr
	{
		String path;
		String size;
		String date;
		String md5;
		
		FileAttr()
		{
			path = "";
			size = "";
			date = "";
			md5 = "";
		}
	}	
		
	static public final String ProxyVersionFileName = "/home/user/SipProxyPBXVersions.json";
	static public final String ProxyVersionFileNameTmp = "/home/user/SipProxyPBXVersionsTmp.json";
	public static boolean version_file_is_generated = false;
	private static int fallos_conectando_Brekeke = 0;
	
	public static boolean GenerateFileOfVersions()
	{				
		boolean ret = false;
		if (version_file_is_generated == true) return ret;
		version_file_is_generated = true;
		
		Main.Milog("GenerateFileOfVersions: entra");
		
		boolean ip_virtual_activada;
		if (Utilities.GetIpVirtual() != null) ip_virtual_activada = true;
		else ip_virtual_activada = false;
		
		ObjectMapper mapper = new ObjectMapper();
		mapper.setVisibility(PropertyAccessor.FIELD, Visibility.ANY);		
		
		File ProxyVersionFileTmp = new File(ProxyVersionFileNameTmp);
		FileOfVersionsClass FileOfVersionsObj_prev = new FileOfVersionsClass();
		if (ProxyVersionFileTmp.exists())
		{			
			try
			{					
				FileOfVersionsObj_prev = mapper.readValue(ProxyVersionFileTmp, FileOfVersionsClass.class);
			} catch (Exception e) {
			}
		}				
		
		FileOfVersionsClass FileOfVersionsObj = new FileOfVersionsClass();
		
		/******** NodeActive *******/
		StringBuffer strbufhostname = Utilities.executeCommand("/bin/hostname");
		String[] lineshostname = strbufhostname.toString().split("\n");
		if (lineshostname.length > 0)
		{
			if (lineshostname[0].contains("node1"))
			{
				FileOfVersionsObj.local_node = "node1";
			}
			else
			{
				FileOfVersionsObj.local_node = "node2";
			}
			if (ip_virtual_activada)
			{
				FileOfVersionsObj.node_status = "active";
			}
			else
			{
				FileOfVersionsObj.node_status = "inactive";
			}
		}		
		
		/******** SipProxyPBXVersion *******/
		if (ip_virtual_activada)
		{		
			FileOfVersionsObj.BKKSipProxyPBXVersion = GetBrekekeVersion();
			if (FileOfVersionsObj.BKKSipProxyPBXVersion == null)
			{
				version_file_is_generated = false;
				Main.MylogError("GenerateFileOfVersions. SoapConfig.GetBrekekeVersion");	
				FileOfVersionsObj.BKKSipProxyPBXVersion = FileOfVersionsObj_prev.BKKSipProxyPBXVersion;
				
				fallos_conectando_Brekeke++;
				Main.Milog("No pude conectarse por websocket: fallos_conectando_Brekeke: " + fallos_conectando_Brekeke);	
				if (fallos_conectando_Brekeke >= 6)
				{
					fallos_conectando_Brekeke = 0;
					ret = true;
				}
			}
			else
			{		
				Main.Milog("GenerateFileOfVersions SipProxyPBXVersion "+ FileOfVersionsObj.BKKSipProxyPBXVersion);
				fallos_conectando_Brekeke = 0;
			}
		}
		else
		{
			//Si no hay ip virtual no intentamos leer la version de la Brekeke por websocket
			//Nos quedamos con la anterior.
			FileOfVersionsObj.BKKSipProxyPBXVersion = FileOfVersionsObj_prev.BKKSipProxyPBXVersion;
			fallos_conectando_Brekeke = 0;
		}
		
		/*******PresenceServer************/
		StringBuffer strbuf = Utilities.executeCommand("/usr/sbin/opensips -V");
		String[] lines = strbuf.toString().split("\n");
		if (lines.length > 0)
		{
			int index_version = lines[0].indexOf("opensips");
			if (index_version < 0)
			{
				version_file_is_generated = false;
				Main.MylogError("GenerateFileOfVersions. No se puede leer la version del servidor de presencia. No exite la linea de version");	
			}
			else
			{
				FileOfVersionsObj.PresenceServer = lines[0].substring(index_version);
			}
		}
		else
		{
			version_file_is_generated = false;
			Main.MylogError("GenerateFileOfVersions. No se puede leer la version del servidor de presencia");	
		}
		
		Main.Milog("GenerateFileOfVersions PresenceServer "+ FileOfVersionsObj.PresenceServer);
		
		/**********CentosRelease************/
		
		try {
			FileOfVersionsObj.CentosRelease = new String ( Files.readAllBytes( Paths.get("/etc/centos-release") ) );
		} catch (IOException e) {
			version_file_is_generated = false;
			Main.MylogError("GenerateFileOfVersions. No puede leer /etc/centos-release");
		}
		
		Main.Milog("GenerateFileOfVersions CentosRelease "+ FileOfVersionsObj.CentosRelease);
		
		FileOfVersionsObj.components.clear();
		
		/*********pbx.war******/
		Main.Milog("GenerateFileOfVersions pbx.war ");
		FileAttr fileattr = GetFileAttr("/usr/local/tomcat7/webapps/pbx.war");
		if (fileattr == null)
		{
			Main.MylogError("GenerateFileOfVersions. GetFileAttr de pbx.war");
			version_file_is_generated = false;
		}
		else
		{		
			FileOfVersionsObj.components.add(fileattr);
		}
		
		/*********proxy_bkk.jar******/
		/*
		Utilities.MyLog("GenerateFileOfVersions proxy_bkk.jar ");
		fileattr = GetFileAttr("/usr/local/tomcat7/webapps/pbx/WEB-INF/lib/proxy_bkk.jar");
		if (fileattr == null)
		{
			version_file_is_generated = false;
			return;
		}
		
		FileOfVersionsObj.components.add(fileattr);
		*/
		
		/*********CheckProxy.jar******/
		Main.Milog("GenerateFileOfVersions CheckProxy.jar ");
		fileattr = GetFileAttr("/root/proxy_ulises/CheckProxy/CheckProxy.jar");
		if (fileattr == null)
		{
			Main.MylogError("GenerateFileOfVersions. GetFileAttr de CheckProxy.jar");
			version_file_is_generated = false;
		}
		else
		{		
			FileOfVersionsObj.components.add(fileattr);
		}
		
		try {
			
			if (mapper.writeValueAsString(FileOfVersionsObj_prev).equals(mapper.writeValueAsString(FileOfVersionsObj)))
			{
				//No cambia nada del fichero
				Main.Milog("GenerateFileOfVersions. Fichero "+ProxyVersionFileNameTmp+" no cambia");
				return ret;
			}
			
			ProxyVersionFileTmp.delete();
			
			mapper.writeValue(ProxyVersionFileTmp, FileOfVersionsObj);			
			Main.Milog("GenerateFileOfVersions. Fichero "+ProxyVersionFileNameTmp+" creado correctamente");
			
			//Copiamos el fichero de form atomica
			Path sourceFile = Paths.get(ProxyVersionFileNameTmp);
		    Path destinationFile = Paths.get(ProxyVersionFileName);
		    Files.copy(sourceFile, destinationFile, StandardCopyOption.REPLACE_EXISTING);
			
		} catch (Exception e) {
			Main.MylogError(e.toString()+" "+e.getMessage());
			Main.MylogError("GenerateFileOfVersions. No se puede crear el fichero ");
			version_file_is_generated = false;
		}	
		
		return ret;
	}
	
	private static FileAttr GetFileAttr(String path)
	{		
		Path file = Paths.get(path);
        BasicFileAttributes attr;
        
		try {
			attr = Files.readAttributes(file, BasicFileAttributes.class);
		} catch (IOException e) {			
			Main.MylogError("GetFileAttr. No puede leer " + path);
			return null;
		}
		FileAttr file_attr_returned = new FileAttr();
		
		file_attr_returned.path = path;				
		DateFormat df = new SimpleDateFormat("dd/MM/yyyy");
		file_attr_returned.date = df.format(attr.lastModifiedTime().toMillis());
		file_attr_returned.size = Long.toString(attr.size());
		file_attr_returned.md5 = Utilities.GetMD5(path);
		
		return file_attr_returned;
	}	
	
	static private JSONObject resultado_obtenido = null;		//Resultado en la respuesta al websocket
	static private String id_esperado = "";			//Id que se espera al envio del mensaje
	static private int id_message = 1;			//id del mensaje que se envia por websocket
	static private WebsocketClientEndpoint.MessageHandler messhand = new WebsocketClientEndpoint.MessageHandler() {
        
		@Override
		public void handleMessage(String message) {
        	        	
        	JSONParser parser = new JSONParser();
        	JSONObject obj;
			try {
				obj = (JSONObject) parser.parse(message);
			} catch (ParseException e) {				
				Main.Milog("ERROR. Parece que este mensaje recibido por websocket no es JSON. "+message);
				return;
			}

			resultado_obtenido = obj;
			wclenp_sem.release();			
        }
	};
		
	@SuppressWarnings("unchecked")
	public static String GetBrekekeVersion()
	{
		/* Se utiliza el metodo de websocket getProductInfo, que no esta documentado.
		 * Pero este es un ejemplo del resultado
		 */
		/*
		{"jsonrpc": "2.0", 
			"result": {
				"sip.tcp.port":"5060",
				"ms.port":"5056",
				"users":20,
				"license_status":"License is activated.",
				"product_id":"DN80CA0100000019",
				"concurrent":20,
				"sip.udp.port":"5060",
				"pal.notify_registered":"false",
				"pbx.port":"5052",
				"options":"xye",
				"version":"3.8.4.9"}, 
			"id": "55"}
		*/
		
		String version_returned = null;
		
		Get_WebsocketClientEndpoint();
		if (wclenp == null)
		{
			Main.Milog("ERROR: GetBrekekeVersion. No puede abrirse el websocket ");
			return null;
		}
		
		try
		{			
			int tries = 3;			
			while (tries > 0)
			{		
				wclenp_sem.drainPermits();
				resultado_obtenido = null;
				
				/*Enviamos methodo getExtensions. para obtener la lista de grupos de extensiones actuales en la PBX*/
				JSONObject obj = new JSONObject();
		        obj.put("jsonrpc", "2.0");
				obj.put("method", "getProductInfo");
				id_message++;
				if (id_message == 50000) id_message = 1;
				id_esperado = Integer.toString(id_message);
				obj.put("id", id_esperado);	
				
				String message = obj.toString();
				wclenp.sendMessage(message);
				
				boolean semr = false;
				try
				{
					semr = wclenp_sem.tryAcquire(25, TimeUnit.SECONDS);
				}
				catch (Exception e)
				{
					semr = false;
				}
				
				if (semr == false)
				{
					//No ha habido respuesta
					Main.Milog("ERROR: GetBrekekeVersion. Sin respuesta al mensaje por websocket "+message+ "tries "+tries);				
				}
				else 
				{
					if (resultado_obtenido != null)
					{
						//Ha habido respuesta
						break;
					}
				}	
				tries--;
			}
			
			if (tries <= 0)
			{
				//No ha habido respuesta despues de varios intentos
				//Cerramos el websocket para que vuelva a intentar abrirlo
				if (wclenp != null) wclenp.Close();
				wclenp = null;
				return null;
			}
			
			String id = (String) resultado_obtenido.get("id");
			if (id == null) return null;
			if (id.equals(id_esperado) == false) return null;
			
			JSONObject result = (JSONObject) resultado_obtenido.get("result");
			if (result == null) return null;
			Main.Milog("GetBrekekeVersion result: " + result.toString());
			
			version_returned = (String) result.get("version");
			Main.Milog("GetBrekekeVersion version_returned: " + version_returned);
		}
		catch (Exception e)
		{
			Main.Milog("ERROR GetBrekekeVersion: ");
			Main.Milog("ERROR "+ e.getMessage());
			try
			{
				if (wclenp != null) wclenp.Close();
			}
			catch (Exception e1) {}			
			wclenp = null;
			return null;
		}
		
		return version_returned;
	}
	
	/*Websocket para conectarnos a la PBX*/
	static private WebsocketClientEndpoint wclenp = null;
	static private Semaphore wclenp_sem = new Semaphore(0);
	
	/*Abbre una conexion por websocket con la PBX si no esta ya abierta*/
	/*Utiliza el manejador de cliente de wclenp*/
	static public void Get_WebsocketClientEndpoint()
	{
		/*No aseguramos de que está levantada la ip virtual de la maquina virtual*/
		String ip_virtual = "1.1.1.1";
		for (int i = 0; i < 4; i++)
		{
			try {
				Thread.sleep(250);
			} catch (InterruptedException e) {
				Main.MylogError("Error al ejecutar sleep");
			}
			
			ip_virtual = Utilities.GetIpVirtual();
			if (ip_virtual == null)
			{
				if (wclenp != null) wclenp.Close();
				wclenp = null;
				Main.Milog("Get_WebsocketClientEndpoint aun no tiene activa la ip virtual");
				return;
			}
		}
		
		String login_name_PBX = Main.config.getProperty("login_name_PBX");		
		if (login_name_PBX == null)
		{
			login_name_PBX = "sa";				
		}
		String login_pass_PBX = Main.config.getProperty("login_pass_PBX");		
		if (login_pass_PBX == null)
		{
			login_pass_PBX = "sa";				
		}
		
		String uri = "ws://";
		uri += ip_virtual + ":8080/pbx/ws?login_user=";
		uri += login_name_PBX+"&login_password=";
		uri += login_pass_PBX;
		
		try
		{
			if (wclenp != null && wclenp.userSession.isOpen())
			{
				//El websocket ya esta abierto
				Main.Milog("Get_WebsocketClientEndpoint websocket ya abierto sale");
				return;
			}
			
			if (wclenp != null) wclenp.Close();
			wclenp = null;
			wclenp_sem.drainPermits();
			resultado_obtenido = null;
			id_esperado = "";
			
			wclenp = new WebsocketClientEndpoint(uri);
			
			// add listener
			if (wclenp != null) wclenp.addMessageHandler(messhand);
			else
			{
				Main.Milog("Get_WebsocketClientEndpoint wclenp es null");
				return;
			}
			
			/*Esperamos a que este abierto el socket*/
			boolean semr = false;
			try
			{
				semr = wclenp_sem.tryAcquire(5, TimeUnit.SECONDS);
			}
			catch (Exception e)
			{
				semr = false;
			}
			
			if (semr == false)
			{
				Main.Milog("ERROR El web socket no se abre "+uri);
				if (wclenp != null) wclenp.Close();
				wclenp = null;
				return;
			}	
			
			boolean webserver_active = false;
			
			if (resultado_obtenido != null && wclenp.userSession != null)
			{
				//El websocket ha sido abierto. Comprobamos que se ha recibido una notificacion
				
				String resultobj = (String) resultado_obtenido.get("method");
				if (resultobj != null && resultobj.equals("notify_serverstatus"))
				{
					JSONObject params = (JSONObject) resultado_obtenido.get("params");
					String status = (String) params.get("status");
					
					if (status.equals("active"))
					{
						webserver_active = true;
					}
				}
			}
			
			if (!webserver_active)
			{
				Main.Milog("ERROR. Al intentar conectar por Websocket a "+uri+" el servidor no parece estar activo");
				if (wclenp != null) wclenp.Close();
				wclenp = null;
				return;
			}
		}
		catch (Exception e)
		{
			Main.Milog("ERROR conectando Websocket a "+uri);
			Main.Milog("ERROR "+ e.getMessage());
			
			if (wclenp != null) wclenp.Close();
			wclenp = null;
			return;
		}		
	}


}
