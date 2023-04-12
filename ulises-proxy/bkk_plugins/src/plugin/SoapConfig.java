package plugin;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import javax.xml.namespace.QName;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.DeserializationFeature;

import es.cd40.*;

public class SoapConfig {
	
	private static Semaphore sem = new Semaphore(1);
	private static UlisesConfigClass UlisesConfigObj = null;
	private static InterfazSOAPConfiguracion soap_service = null;
	private static InterfazSOAPConfiguracionSoap soap_port = null;
	public static boolean Creados_ExtensionesPBX = false;
	public static boolean Creados_UsuariosProxy = false;
	
	private static String cfg_url = null;
	private static String id_sistema = null;
	
	/**************************************************************************************************/
	/* Lee la configuracion requerida del servidor de configuración por SOAP
	 * Retorna true si hay una nueva configuracion										
	 */
	/**************************************************************************************************/
	static int IpVirtualActive = 0;
	private static String ReadConfig_previous_exception = "";
	static boolean ReadConfig()
	{	
		boolean ret = true;
		
		String _VersionConfiguracion = null;
		ConfiguracionSistema _configuracion_sistema = null;
		ArrayOfAsignacionRecursosGW _asignacion_recursos = null;
		ArrayOfAsignacionUsuariosTV _asignacion_usuarios = null;
		ArrayOfDireccionamientoIP _planDireccionamientoIp = null;
		ArrayOfDireccionamientoSIP _planDireccionamientoSip = null;
		ArrayOfNumeracionATS _numeracionATS = null;
		ArrayOfListaRedes _listaRedes = null;
		ArrayOfListaTroncales _listaTroncales = null;	
		
		if (Utilities.GetIpVirtual() == null)
		{
			//Esta maquina no tiene activa la ip virtual por tanto no tiene conectividad con 
			//la configuracion
			IpVirtualActive = 0;
			return false;
		}
		else
		{
			if (IpVirtualActive < 2)
			{
				IpVirtualActive++;
				if (IpVirtualActive < 2)
				{
					//Como mínimo tiene que detectarse dos veces la ip virtual para darla por buena
					return false;
				}
			}
		}
		
		try {
			Thread.sleep(20);
		} catch (InterruptedException e1) {
			
		}		
		
		if (cfg_url == null)
		{
			cfg_url = Utilities.GetSetting("U5ki_CfgService");		
			if (cfg_url == null)
			{
				Utilities.MyLogError("No se puede leer U5ki_CfgService del fichero de Settings");
				ret = false;
			}
		}
		
		try {
			Thread.sleep(20);
		} catch (InterruptedException e1) {							
		}
		
		if (ret)
		{		
			if (id_sistema == null)
			{
				id_sistema = Utilities.GetSetting("U5ki_IdSistema");
				if (id_sistema == null)
				{
					Utilities.MyLogError("No se puede leer U5ki_IdSistema del fichero de Settings");
					ret = false;
				}
			}
		}
		
		try {
			Thread.sleep(20);
		} catch (InterruptedException e1) {
			
		}
		
		if (ret)
		{
			try {			
				
				QName SERVICE_NAME = new QName("http://CD40.es/", "InterfazSOAPConfiguracion");
				URL url = new URL(cfg_url);
				
				if (SoapConfig.soap_service == null)
				{
					//No hago el new siempre porque he visto que consume mucha cpu
					SoapConfig.soap_service = new InterfazSOAPConfiguracion(url, SERVICE_NAME);	
					soap_port = soap_service.getInterfazSOAPConfiguracionSoap();
				}
				
				try {
					Thread.sleep(20);
				} catch (InterruptedException e1) {
					
				}
				
				String version = "";
				version = soap_port.getVersionConfiguracion(id_sistema);
				
				int configuraciones_iguales = 0;
				int Max_configuraciones_iguales = 3;
				int Max_intentos = 10;
				
				UlisesConfigClass UlisesConfig_prov = null;
				
				if ((UlisesConfigObj == null) || !version.equals(UlisesConfigObj.VersionConfiguracion))
				{
					//Para dar por buena la configuracion hay que recibir la misma 3 veces seguidas
					while (configuraciones_iguales < Max_configuraciones_iguales)
					{
						//Se ha detectado que ha cambiado la version de configuracion. Sin embargo se ha visto que 
						//si leemos inmediatamente la configuracion completa hay datos que no estan todavia actualizados
						//asi es que esperamos 
						Thread.sleep(3000);
						
						Utilities.MyLog("Lee configuracion por SOAP Max_intentos = " + Max_intentos);
						
						UlisesConfigClass UlisesConfigTmp = new UlisesConfigClass();
						
						_VersionConfiguracion = soap_port.getVersionConfiguracion(id_sistema);
						
						try {
							Thread.sleep(20);
						} catch (InterruptedException e1) {							
						}
						
						_configuracion_sistema = soap_port.getConfigSistema(id_sistema);		//service.getConfigSistema(id_sistema);
						
						try {
							Thread.sleep(20);
						} catch (InterruptedException e1) {							
						}
						
						_numeracionATS = _configuracion_sistema.getPlanNumeracionATS();				
						_planDireccionamientoIp = _configuracion_sistema.getPlanDireccionamientoIP();
						_listaTroncales = _configuracion_sistema.getPlanTroncales();
						_listaRedes = _configuracion_sistema.getPlanRedes();
						_asignacion_usuarios = _configuracion_sistema.getPlanAsignacionUsuarios();
						_asignacion_recursos = _configuracion_sistema.getPlanAsignacionRecursos();
						_planDireccionamientoSip = _configuracion_sistema.getPlanDireccionamientoSIP();
						UlisesConfigTmp.VersionConfiguracion = _VersionConfiguracion;
						
						/*Cada elemento de la configuracion lo creamos vacia si el objeto obtenido por SOAP es null*/
						
						if (_asignacion_recursos == null)
						{
							UlisesConfigTmp.asignacion_recursos = new ArrayList<AsignacionRecursosGW>();
						}
						else
						{
							UlisesConfigTmp.asignacion_recursos = _asignacion_recursos.getAsignacionRecursosGW();
						}
						
						if (_asignacion_usuarios == null)
						{
							UlisesConfigTmp.asignacion_usuarios = new ArrayList<AsignacionUsuariosTV>();
						}
						else
						{
							UlisesConfigTmp.asignacion_usuarios = _asignacion_usuarios.getAsignacionUsuariosTV();
						}
						
						if (_planDireccionamientoIp == null)
						{
							UlisesConfigTmp.planDireccionamientoIp = new ArrayList<DireccionamientoIP>();
						}
						else
						{
							UlisesConfigTmp.planDireccionamientoIp = _planDireccionamientoIp.getDireccionamientoIP();
						}
						
						if (_planDireccionamientoSip == null)
						{
							UlisesConfigTmp.planDireccionamientoSip = new ArrayList<DireccionamientoSIP>();
						}
						else
						{
							UlisesConfigTmp.planDireccionamientoSip = _planDireccionamientoSip.getDireccionamientoSIP();
						}
						
						if (_numeracionATS == null)
						{
							UlisesConfigTmp.numeracionATS = new ArrayList<NumeracionATS>();
						}
						else
						{
							UlisesConfigTmp.numeracionATS = _numeracionATS.getNumeracionATS();
						}
						
						if (_listaRedes == null)
						{
							UlisesConfigTmp.listaRedes = new ArrayList<ListaRedes>();
						}
						else
						{
							UlisesConfigTmp.listaRedes = _listaRedes.getListaRedes();
						}
						
						if (_listaTroncales == null)
						{
							UlisesConfigTmp.listaTroncales = new ArrayList<ListaTroncales>();
						}
						else
						{
							UlisesConfigTmp.listaTroncales = _listaTroncales.getListaTroncales();
						}
						
						UlisesConfigTmp.listaAbonadosATSSectores = GetListaAbonadosATSSectores(UlisesConfigTmp.planDireccionamientoSip);
						UlisesConfigTmp.listaTelefonosVoIP = GetListaTelefonosVoIP(UlisesConfigTmp.asignacion_recursos, UlisesConfigTmp.planDireccionamientoIp);
						
						if (UlisesConfig_prov != null)
						{
							if (UlisesConfig_prov.equals(UlisesConfigTmp))
							{
								configuraciones_iguales++;
							}
							else 
							{
								configuraciones_iguales = 0;								
								UlisesConfig_prov = UlisesConfigTmp;
							}
						}
						else
						{		
							UlisesConfig_prov = UlisesConfigTmp;
						}
						
						if (--Max_intentos <= 0)
						{
							ret = false;
							break;
						}
					}					
				}
				
				try {
					Thread.sleep(20);
				} catch (InterruptedException e1) {							
				}
				
				if (configuraciones_iguales >= Max_configuraciones_iguales)
				{
					//Ya tenemos una nueva configuracion
					Utilities.MyLog("Detectado por SOAP cambio en la Configuracion. Nueva Version: " + version);
					
					sem.acquire();	
					UlisesConfigObj = UlisesConfig_prov;
					sem.release();
					
					Utilities.MyLog("Lista de abonados obtenida por SOAP: ");
					Utilities.MyLog(UlisesConfigObj.listaAbonadosATSSectores.toString());
					Utilities.MyLog(" ");
					
					Utilities.MyLog("Lista de telefonos VoIP obtenida por SOAP: ");
					Utilities.MyLog(UlisesConfigObj.listaTelefonosVoIP.toString());
					Utilities.MyLog(" ");	
						
					WriteUlisesConfigToFile();
					Creados_ExtensionesPBX = false;
					Creados_UsuariosProxy = false;			
				}
				
				try {
					Thread.sleep(20);
				} catch (InterruptedException e1) {							
				}
				
				if (!Creados_ExtensionesPBX)
				{
					Creados_ExtensionesPBX = CreaExtensionesPBXPorWebsocket();
				}
				
				try {
					Thread.sleep(20);
				} catch (InterruptedException e1) {							
				}
				
				if (!Creados_UsuariosProxy)
				{
					Creados_UsuariosProxy = CrearUsuariosProxy();
				}
				
				ReadConfig_previous_exception = "";
				
			} catch (MalformedURLException e) {
				
				Utilities.MyLogError(e.toString()+" "+e.getMessage());
				Utilities.MyLogError("ReadConfig. URL no valida: " + cfg_url);	
				//Utilities.MyLogStackTrace(e);
				ret = false;
			} catch (InterruptedException e) {
				Utilities.MyLogError(e.toString()+" "+e.getMessage());
				Utilities.MyLogError("ReadConfig. No puede adquirir el semaforo ");
				//Utilities.MyLogStackTrace(e);
				ret = false;
			} catch (Exception e) {
				
				String current_e = e.toString()+" "+e.getMessage();
				if (!current_e.equals(ReadConfig_previous_exception))
				{				
					Utilities.MyLogError(current_e);
					Utilities.MyLogError("ReadConfig. Excepcion");
					ReadConfig_previous_exception = current_e;
				}				
				 
				ret = false;
			} 
		}
		
		if (!ret)
		{
			SoapConfig.soap_service = null; //Lo ponemos a null para que vuelva a intentar conectarse
			if (UlisesConfigObj != null)
			{
				UlisesConfigObj.VersionConfiguracion = " ";		//Forzamos a que lea una nueva configuracion
			}
		}
	
		return ret;
	}	
	
	/**************************************************************************************************/
	/* Copia la configuracion en el fichero json									
	 */
	/**************************************************************************************************/
	static public void WriteUlisesConfigToFile()
	{	
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("WriteUlisesConfigToFile. No puede adquirir el semaforo ");
			return;
		}
		
		try {
			ObjectMapper mapper = new ObjectMapper();	
			
			mapper.writeValue(new File(Utilities.UlisesConfigFileName), UlisesConfigObj);
			
			Utilities.MyLog("WriteUlisesConfigToFile. Fichero de configuracion "+Utilities.UlisesConfigFileName+" creado correctamente");
			
		} catch (Exception e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("WriteUlisesConfigToFile. No se puede crear el fichero ");
			Utilities.MyLogStackTrace(e);
		}	
		
		sem.release();
		
	}
	
	/**************************************************************************************************/
	/* Lee Configuracion Ulises desde el fichero									
	 */
	/**************************************************************************************************/
	static public boolean ReadUlisesConfigFromFile()
	{			
		boolean ret = false;
		
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("ReadUlisesConfigFromFile. No puede adquirir el semaforo ");
			return ret;
		}
		
		try {
			ObjectMapper mapper = new ObjectMapper();	
			
			mapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
			UlisesConfigClass UConfigObj = mapper.readValue(new File(Utilities.UlisesConfigFileName), UlisesConfigClass.class);
			
			if (UConfigObj.TodosLosElementosNonNull() == false)
			{
				Utilities.MyLog("ReadUlisesConfigFromFile. Fichero de configuracion "+Utilities.UlisesConfigFileName+" no esta completo");
				ret = false;
			}
			else
			{			
				UlisesConfigObj = UConfigObj;
				
				Utilities.MyLog("ReadUlisesConfigFromFile. Fichero de configuracion "+Utilities.UlisesConfigFileName+" leido correctamente");
				ret = true;
			}
			
		} catch (Exception e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("ReadUlisesConfigFromFile. No se puede leer el fichero de configuracion json");
			Utilities.MyLogStackTrace(e);			
		}	
		
		sem.release();
		
		return ret;
	}
	
	/**************************************************************************************************/
	/* Busca la central a la que pertenece el user.		
	 * Retorna el contenido de esa central. Numeracion ATS
	 */
	/**************************************************************************************************/
	static public NumeracionATS GetCentralEnc(String user)
	{
		NumeracionATS numATScentral = null;
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Entra GetCentralEnc. user="+user);
		
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("GetCentralEnc. No puede adquirir el semaforo ");
			return null;
		}
		
		if (UlisesConfigObj != null) 
		{
			try
			{
				for (int i = 0; i < UlisesConfigObj.numeracionATS.size() && numATScentral == null; i++)
				{
					//Probamos con rango de operador
					ArrayOfRangosSCV rangos = UlisesConfigObj.numeracionATS.get(i).getRangosOperador();
					for (int j = 0; j < rangos.getRangosSCV().size() && numATScentral == null; j++)
					{
						if ((rangos.getRangosSCV().get(j).getInicial().compareTo(user) <= 0) && (rangos.getRangosSCV().get(j).getFinal().compareTo(user) >= 0))
						{
							numATScentral = UlisesConfigObj.numeracionATS.get(i);					
						}				
					}		
					//Probamos con rango de privilegiado
					rangos = UlisesConfigObj.numeracionATS.get(i).getRangosPrivilegiados();
					for (int j = 0; j < rangos.getRangosSCV().size() && numATScentral == null; j++)
					{
						if ((rangos.getRangosSCV().get(j).getInicial().compareTo(user) <= 0) && (rangos.getRangosSCV().get(j).getFinal().compareTo(user) >= 0))
						{
							numATScentral = UlisesConfigObj.numeracionATS.get(i);					
						}				
					}
				}
			} catch (Exception e) {
				Utilities.MyLogError(e.toString()+" "+e.getMessage());
				Utilities.MyLogError("GetCentralEnc. Excepcion");
				Utilities.MyLogStackTrace(e);
				numATScentral = null;			
			}
		}
		
		sem.release();
		
		return numATScentral;
	}	
	
	/**************************************************************************************************/
	/* Busca el direccionamientoIP de una central	
	 * Retorna el contenido. null si no lo encuentra
	 */
	/**************************************************************************************************/
	static public DireccionamientoIP GetDireccionamientoIP(String central)
	{
		DireccionamientoIP ret = null;
		
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("GetDireccionamientoIP. No puede adquirir el semaforo ");
			return null;
		}
		
		if (UlisesConfigObj != null)
		{		
			try
			{
				for (int i = 0; i < UlisesConfigObj.planDireccionamientoIp.size(); i++)
				{
					if (UlisesConfigObj.planDireccionamientoIp.get(i).getIdHost().equals(central))
					{
						ret = UlisesConfigObj.planDireccionamientoIp.get(i);
						break;
					}				
				}	
			} catch (Exception e) {
				Utilities.MyLogError(e.toString()+" "+e.getMessage());
				Utilities.MyLogError("GetDireccionamientoIP. Excepcion");
				Utilities.MyLogStackTrace(e);
				ret = null;			
			}
		}
		
		sem.release();
		
		return ret;
	}
	
	/**************************************************************************************************/
	/* Obtiene la lista de recursos para un troncal
	 * Retorna el contenido. null si no lo encuentra
	 */
	/**************************************************************************************************/
	static public ArrayOfPlanRecursos GetListaRecursosTroncal(String troncal)
	{
		ArrayOfPlanRecursos ret = null;
		
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("GetListaRecursosTroncal. No puede adquirir el semaforo ");
			return null;
		}
		
		if (UlisesConfigObj != null) 
		{		
			try
			{
				for (int i = 0; i < UlisesConfigObj.listaTroncales.size(); i++)
				{
					if (UlisesConfigObj.listaTroncales.get(i).getIdTroncal().equals(troncal))
					{
						ret = UlisesConfigObj.listaTroncales.get(i).getListaRecursos();		
						break;
					}			
				}		
			} catch (Exception e) {
				Utilities.MyLogError(e.toString()+" "+e.getMessage());
				Utilities.MyLogError("GetListaRecursosTroncal. Excepcion");
				Utilities.MyLogStackTrace(e);
				ret = null;			
			}
		}
		
		sem.release();
		return ret;
	}
	
	/**************************************************************************************************/
	/* Para un recurso, obtenemos el identificador de la pasarela y el puerto
	 * Retorna el contenido. null si no lo encuentra
	 */
	/**************************************************************************************************/
	static public AsignacionRecursosGW GetAsignacionGW(String recurso)
	{
		AsignacionRecursosGW ret = null;
		
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("GetListaRecursosTroncal. No puede adquirir el semaforo ");
			return null;
		}
		
		if (UlisesConfigObj != null) 
		{		
			try
			{
				for (int i = 0; i < UlisesConfigObj.asignacion_recursos.size(); i++)
				{
					if (UlisesConfigObj.asignacion_recursos.get(i).getIdRecurso().equals(recurso))
					{
						ret = UlisesConfigObj.asignacion_recursos.get(i);
						break;
					}				
				}
				
			} catch (Exception e) {
				Utilities.MyLogError(e.toString()+" "+e.getMessage());
				Utilities.MyLogError("GetAsignacionGW. Excepcion");
				Utilities.MyLogStackTrace(e);
				ret = null;			
			}
		}
		
		sem.release();
		return ret;	
	}
	
	/**************************************************************************************************/
	/* Con el usuario, se busca el encaminamiento.		
	 * Retorna un array con todas las uris posible para el encaminamiento.
	 * Retorna null si no encuentra ruta
	 * Esta uri será la que se pasa como parametro en el failover del dialplan.							
	 */
	/**************************************************************************************************/
	static public ArrayList<String> GetEnc(String user)
	{
		ArrayList<String> ret = null;
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Entra GetEnc. user="+user);
		
		//Buscamos la central que comprende a este user en su rango
		
		NumeracionATS numATScentral = GetCentralEnc(user);
		if (numATScentral == null)
		{
			Utilities.MyLog("Al encaminar user "+user+" No se encuentra en ningun rango de ninguna central");
			return ret;
		}
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" pertenece a central "+numATScentral.getCentral());
		
		//Si la central obtenida es la propia entonces no vale.
		if (numATScentral.isCentralPropia()) 
		{
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Al encaminar user "+user+" Es un usuario de la propia central");
			return ret;
		}
		
		Utilities.MyLog("Encaminando "+user+" pertenece a central "+numATScentral.getCentral()+". No es la propia");
		
		ArrayList<String> proxies_uris = new ArrayList<String>();  //Contiene las uris de los proxies en caso de la que central sea ip
		
		String central = numATScentral.getCentral();
		DireccionamientoIP dirIP = GetDireccionamientoIP(central);
		if (dirIP == null)
		{
			if (Utilities.LOGDEBENABLED)  
				Utilities.MyLogDeb("GetEnc user "+user+" pertenece a central "+numATScentral.getCentral()+" No es central IP");
		}
		else
		{
			//La central es IP.
			//Buscamos los proxies de esa central y los agregamos a la lista de uris hacia proxies			
			String ip = dirIP.getIpRed1();
			if (ip != null && ip.length() > 0)
			{
				if (ip.indexOf(":") < 0)
				{
					//Si no lleva el puerto se lo ponemos, porque lo requiere el tifx
					ip = ip.concat(":5060");
				}
				if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" pertenece a central "+numATScentral.getCentral()+" proxy principal "+ip);
				
				//if (Utilities.LOGDEBENABLED) proxy_bkk.dependenciasSet.dump_dependencies();
				
				//Comprobamos que esta disponible en el tifx
				RsInfo rs = proxy_bkk.dependenciasSet.Get("PROXIES", ip);
				if (rs != null && rs.State == proxy_bkk.dependenciasSet.STATE_IDLE)
				{
					//Como esta disponible lo usamos
					String uri = "<sip:"+user+"@"+ip+">";
					proxies_uris.add(uri);
					if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" uri principal "+uri);
				}		
				else
				{
					if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" proxy principal "+ip+" no disponible");
				}
			}
			ip = dirIP.getIpRed2();
			if (ip != null && ip.length() > 0)
			{
				if (ip.indexOf(":") < 0)
				{
					//Si no lleva el puerto se lo ponemos, porque lo requiere el tifx
					ip = ip.concat(":5060");
				}
				if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" pertenece a central "+numATScentral.getCentral()+" proxy alternativo 1 "+ip);
				//Comprobamos que esta disponible en el tifx
				RsInfo rs = proxy_bkk.dependenciasSet.Get("PROXIES", ip);
				if (rs != null && rs.State == proxy_bkk.dependenciasSet.STATE_IDLE)
				{		
					//Como esta disponible lo usamos
					String uri = "<sip:"+user+"@"+ip+">";
					proxies_uris.add(uri);
					if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" uri alternativo 1 "+uri);
				}
				else
				{
					if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" proxy alternativo 1 "+ip+" no disponible");
				}
			}
			ip = dirIP.getIpRed3();
			if (ip != null && ip.length() > 0)
			{
				if (ip.indexOf(":") < 0)
				{
					//Si no lleva el puerto se lo ponemos, porque lo requiere el tifx
					ip = ip.concat(":5060");
				}
				if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" pertenece a central "+numATScentral.getCentral()+" proxy alternativo 2 "+ip);
				//Comprobamos que esta disponible en el tifx
				RsInfo rs = proxy_bkk.dependenciasSet.Get("PROXIES", ip);
				if (rs != null && rs.State == proxy_bkk.dependenciasSet.STATE_IDLE)
				{		
					//Como esta disponible lo usamos
					String uri = "<sip:"+user+"@"+ip+">";
					proxies_uris.add(uri);
					if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" uri alternativo 2 "+uri);
				}
				else
				{
					if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" proxy alternativo 2 "+ip+" no disponible");
				}
			}
		}
		
		//A continuacion se obtiene una lista de recursos de las rutas.
		ArrayList<String> recursos_rutas = new ArrayList<String>();
		
		//Buscamos las rutas de esa central.
		ArrayOfPlanRutas planrutas = numATScentral.getListaRutas();
		if (planrutas != null)
		{
			//Para cada ruta obtenemos la lista de troncales
			for (int i = 0; i < planrutas.getPlanRutas().size(); i++)
			{
				ArrayOfString lista_troncales = planrutas.getPlanRutas().get(i).getListaTroncales();
				
				//Para cada troncal obtenemos la lista de recursos y los agregamos a recursos_rutas
				for (int j = 0; j < lista_troncales.getString().size(); j++)
				{
					ArrayOfPlanRecursos plan_recursos = GetListaRecursosTroncal(lista_troncales.getString().get(j));
					if (plan_recursos != null)
					{
						if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" troncal "+lista_troncales.getString().get(j));
						for (int k = 0; k < plan_recursos.getPlanRecursos().size(); k++)
						{
							recursos_rutas.add(plan_recursos.getPlanRecursos().get(k).getIdRecurso());
							if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" recurso de rutas "+plan_recursos.getPlanRecursos().get(k).getIdRecurso());
						}		
					}
				}				
			}	
		}
		
		//Ahora buscamos las direcciones ip de los recursos. Y formamos las uris
		ArrayList<String> uris_recursos_rutas = new ArrayList<String>();
		
		for (int i = 0; i < recursos_rutas.size(); i++)
		{
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" recurso de rutas "+recursos_rutas.get(i));
			//Buscamos la pasarela del recurso o el equipo externo
			AsignacionRecursosGW asig_rec = GetAsignacionGW(recursos_rutas.get(i));
			if (asig_rec != null)
			{
				DireccionamientoIP dirip = GetDireccionamientoIP(asig_rec.getIdHost());
				if (dirip != null)
				{
					RsInfo rs = null;
					if (dirip.getTipoHost() == TipoElementoHW.TEH_EXTERNO_TELEFONIA)
					{
						//El recurso es un equipo externo de telefonia. Por tanto no se comprueba la disponibilidad
						if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" recurso de rutas "+recursos_rutas.get(i)+ " eq ext telf "+asig_rec.getIdHost());					
					}
					else
					{	
						//Es un recurso de telefonia interno, pasarela
						if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" recurso de rutas "+recursos_rutas.get(i)+ " GW "+asig_rec.getIdHost());
						//Comprobamos que el recurso de la pasarela esta disponible
						rs = proxy_bkk.dependenciasSet.Get(asig_rec.getIdHost(), recursos_rutas.get(i));
					}
					
					if ((dirip.getTipoHost() == TipoElementoHW.TEH_EXTERNO_TELEFONIA) || (rs != null && rs.State == proxy_bkk.dependenciasSet.STATE_IDLE))
					{				
						//Buscamos la ip de la pasarela	o equipo externo				
						String ip1 = dirip.getIpRed1();		//He visto que la ip de comunicaciones de la pasarela es la primera
						
						//Creo las uris
						String uri;
						if (ip1 != null && ip1.length() > 0)
						{
							uri = "<sip:"+user+"@"+ip1;
							
							if (uri.indexOf(":") != -1)
							{
								//IpRed1 ya incluye el puerto								
							}
							else
							{
								//Le ponemos el puerto a la uri
								uri += ":"+asig_rec.getSipPort();
							}
							
							if (dirip.getTipoHost() == TipoElementoHW.TEH_EXTERNO_TELEFONIA)
							{
								uri += ";user="+asig_rec.getIdRecurso()+">";
							}
							else
							{
								uri += ";cd40rs="+asig_rec.getIdRecurso()+">";
							}				
							
							uris_recursos_rutas.add(uri);	
							
							if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" recurso disponible "+uri);
						}
					}
				}
				else
				{
					if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetEnc user "+user+" central "+numATScentral.getCentral()+" recurso no disponible ");
				}
			}	
		}
			
		//Creamos el string con todas las uris separadas por espacios
		if (proxies_uris.size() > 0 || uris_recursos_rutas.size() > 0)
		{
			ret = new ArrayList<String>();
		}
		for (int i = 0; i < proxies_uris.size(); i++)
		{
			ret.add(proxies_uris.get(i));			
		}
		for (int i = 0; i < uris_recursos_rutas.size(); i++)
		{
			ret.add(uris_recursos_rutas.get(i));			
		}	
		
		if (ret != null)
		{
			Utilities.MyLog("Encaminando "+user+" hacia "+ ret.toString());			
		}
		else
		{
			Utilities.MyLog("No hay recursos disponibles para encaminar "+user);
		}
		
		return ret;
	}
	
	static private JSONObject resultado_obtenido = null;		//Resultado en la respuesta al websocket
	static private String id_esperado = "";			//Id que se espera al envio del mensaje
	static private WebsocketClientEndpoint.MessageHandler messhand = new WebsocketClientEndpoint.MessageHandler() {
        
		@Override
		public void handleMessage(String message) {
        	
        	if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("RECIBO:"+message);
        	
        	JSONParser parser = new JSONParser();
        	JSONObject obj;
			try {
				obj = (JSONObject) parser.parse(message);
			} catch (ParseException e) {				
				Utilities.MyLogError("ERROR. Parece que este mensaje recibido por websocket no es JSON. "+message);
				return;
			}

			resultado_obtenido = obj;
			wclenp_sem.release();			
        }
	};

	/*Websocket para conectarnos a la PBX*/
	static private WebsocketClientEndpoint wclenp = null;
	static private Semaphore wclenp_sem = new Semaphore(0);
	
	/*Abbre una conexion por websocket con la PBX si no esta ya abierta*/
	/*Utiliza el manejador de cliente de wclenp*/
	static public void Get_WebsocketClientEndpoint()
	{
		
		/*No aseguramos de que está levantada la ip virtual de la maquina virtual*/
		String ip_virtual = Utilities.GetIpVirtual();
		if (ip_virtual == null)
		{
			if (wclenp != null) wclenp.Close();
			wclenp = null;
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Get_WebsocketClientEndpoint aun no tiene activa la ip virtual");
			IpVirtualActive = 0;
			return;
		}
		else
		{
			if (IpVirtualActive < 2)
			{
				IpVirtualActive++;
				if (IpVirtualActive < 2)
				{
					//Como mínimo tiene que detectarse dos veces la ip virtual para darla por buena
					if (wclenp != null) wclenp.Close();
					wclenp = null;
					if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Get_WebsocketClientEndpoint aun no tiene activa la ip virtual");
					return;
				}
			}
		}		
		
		String uri = "ws://";
		uri += ip_virtual + ":8080/pbx/ws?login_user=";
		uri += proxy_bkk.login_name_PBX+"&login_password=";
		uri += proxy_bkk.login_pass_PBX;
		
		try
		{
			if (wclenp != null && wclenp.userSession.isOpen())
			{
				//El websocket ya esta abierto
				if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Get_WebsocketClientEndpoint websocket ya abierto sale");
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
				if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Get_WebsocketClientEndpoint wclenp es null");
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
				Utilities.MyLogError("ERROR El web socket no se abre "+uri);
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
				Utilities.MyLogError("ERROR. Al intentar conectar por Websocket a "+uri+" el servidor no parece estar activo");
				if (wclenp != null) wclenp.Close();
				wclenp = null;
				return;
			}
		}
		catch (Exception e)
		{
			Utilities.MyLogError("ERROR conectando Websocket a "+uri);
			Utilities.MyLogError("ERROR "+ e.getMessage());
			
			if (wclenp != null) wclenp.Close();
			wclenp = null;
			return;
		}		
	}
	
	/*Crea las extensiones en la pbx, a partir de los usuarios de los sectores*/	
	/*Retorna true si ha tenido exito*/
	static public boolean CreaExtensionesPBXPorWebsocket()
	{
		boolean ret = true;		
		
		String crear_extensiones_y_usuarios = Utilities.GetSetting("CrearExtensionesYUsuarios");
		if (crear_extensiones_y_usuarios != null && crear_extensiones_y_usuarios.equals("0"))
		{
			//Lee la propiedad CrearExtensionesYUsuarios. La cual si tiene el valor 0
			//quiere decir que no queremos que se creen las extensiones y usuarios en la brekeke
			//desde la configuracion
			//Entonces se sale de esta funcion con valor true
			return ret;
		}	
		
		Get_WebsocketClientEndpoint();
		if (wclenp == null)
		{
			return false;			
		}
		
		ret = CrearExtensionesPBX();		
		if (ret == true)
		{
			Utilities.MyLog("CreaExtensionesPBXYUsuariosProxy sale con exito");			
		}
		
		return ret;
	}
	
	/***********************************************************************************************************/
	/*Se crean las extensiones de los puestos y de los telefonos de seguridad en la PBX
	 * 
	 */
	static public boolean CrearExtensionesPBX()
	{
		boolean ret = true;

		//Se obtiene el array con todas las extensiones tipo user
		JSONArray ext_array = null;
		
		ext_array = GetExtensiones("user");
		if (ext_array == null)
		{
			Utilities.MyLogError("ERROR CrearExtensionesPBX. No se puede obtener lista de extensiones tipo user ");	
			return false;
		}		
		
		//Se borran de la BPX las extensiones que no estan en la configuracion.		
        for (int i = 0; i < ext_array.size(); i++) {        	
       		String ext = (String) ext_array.get(i);
       		
       		if (!EstaEnListaAbonadosATSSectores(ext) && !EstaEnListaTelefonosVoIP(ext))
       		{
       			//Borra la extension de la pbx
       			if (BorrarExtensionPBX(ext) == false)
       			{
					Utilities.MyLogError("No se puede borrar la extension"+(String) ext_array.get(i)+" por websocket");
					ret = false;
	       		}
       		}
        }
        
        //Obtenemos la nueva lista de las extensiones
        ext_array = GetExtensiones("user");
		if (ext_array == null)
		{
			Utilities.MyLogError("ERROR CrearExtensionesPBX. No se puede obtener lista de extensiones tipo user  despues de borrar los que no se encuentran en la configuracion");	
			return false;
		}
        
        Utilities.MyLog("Extensiones en PBX despues de borrar: ");
        Utilities.MyLog(ext_array.toJSONString());
        Utilities.MyLog("--- ");
        
        try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("CrearExtensionesPBX. No puede adquirir el semaforo sem");
			return false;
		}
                
        try {
        	
        	//Se crean las extensiones en la PBX dando prioridad a las de los telefonos VoIP, ya que lo requieren para la gestion de su presencia
        
        	Utilities.MyLog(" -------------- CREA EXTENSIONES DE LOS TELEFONOS VOIP --------------------- ");
	        
	        //Se crean las extensiones de los telefonos VoIP del SCV en la PBX
	        if (UlisesConfigObj != null && UlisesConfigObj.listaTelefonosVoIP != null)
	        {        	
	        	for (int i = 0; i < UlisesConfigObj.listaTelefonosVoIP.size(); i++)
	        	{
	        		String idrecurso = UlisesConfigObj.listaTelefonosVoIP.get(i);
	        		
	        		if (!ext_array.contains(idrecurso))
	    			{
	        			//Creamos la extension si no esta creada
	        			int result = CrearUserExtensionPBX(idrecurso);
	    	        	if (result == MAXIMO_NUMERO_EXTENSIONES_PBX_SOBREPASADA)
	    	        	{	    	        		
	    	        		//Del array de extensiones de la PBX obtenido, borramos una que no sea del tipo telefono VoIP, tanto del array como de la PBX
	    	        		boolean extension_sustituida = false;
	    	        		for (int j = 0; j < ext_array.size(); j++) {        	
	    	               		String ext = (String) ext_array.get(j);
	    	               		if (!UlisesConfigObj.listaTelefonosVoIP.contains(ext))
	    	               		{
		    	               		//Borra la extension de la pbx
	    	               			if (BorrarExtensionPBX(ext) == false)
	    	               			{
	    	        					Utilities.MyLogError("No se puede borrar la extension que no es del tipo tlf VoIP"+(String) ext_array.get(i)+" por websocket");
	    	        	       		}
	    	               			else
	    	               			{
	    	               				//Una vez borrada de la PBX, la borramos del array
	    	               				ext_array.remove(ext);
	    	               				//Se vuelve a intentar crear la extension del telefono VoIP
	    	               				result = CrearUserExtensionPBX(idrecurso);
	    	               				if (result == MAXIMO_NUMERO_EXTENSIONES_PBX_SOBREPASADA)
	    	               				{
	    	               					//Si se sobrepasa el numero de usuarios no retornamos false para que no se vuelva a intentar
	    	               				}
	    	               				else if (result != CREADA_EXTENSION_PBX)
	    	               				{
	    	               					Utilities.MyLogError("ERROR. No se ha podido agregar en la PBX la extension telf VoIP despues de borrar otra que no es telf VoIP" + idrecurso);
	    	               					ret = false;
	    	               				}
	    	               				else
	    	               				{
	    	               					Utilities.MyLog("Se ha borrado la extension "+ext+" y se ha agregado la extesion de tlf VoIP "+idrecurso);
	    	               					extension_sustituida = true;
	    	               				}
	    	               				break;
	    	               			}
	    	               		}	    	               		
	    	                }
	    	        		if (!extension_sustituida)
	    	        		{
	    	        			Utilities.MyLogError("La extension de tlf VoIP "+idrecurso+" definitivamente no ha podido ser agregada a la PBX por limite de licencia");
	    	        		}
	    	        	}
	    	        	else if (result != CREADA_EXTENSION_PBX)
	    	        	{
		    	        	Utilities.MyLogError("ERROR. No se ha podido agregar en la PBX la extension " + idrecurso);
		    	        	ret = false;
		    	        }
	    			}
	        	}
	        } 
        	
        	Utilities.MyLog(" --------------- CREA EXTENSIONES DE LOS SECTORES -------------------");
	        
	        //Se crean las extensiones de los sectores del scv en la PBX
	        if (UlisesConfigObj != null && UlisesConfigObj.listaAbonadosATSSectores != null)
	        {
	        	for (int i = 0; i < UlisesConfigObj.listaAbonadosATSSectores.size(); i++)
	        	{
	        		String num_abonado = UlisesConfigObj.listaAbonadosATSSectores.get(i);
	        		
	        		if (!ext_array.contains(num_abonado))
	    			{
	    				//Creamos la extension si no esta creada
	        			int result = CrearUserExtensionPBX(num_abonado);
	        			if (result == MAXIMO_NUMERO_EXTENSIONES_PBX_SOBREPASADA)
           				{
           					//Si se sobrepasa el numero de usuarios no retornamos false para que no se vuelva a intentar
           				}
	        			else if (result != CREADA_EXTENSION_PBX)
        	        	{
	        	        	Utilities.MyLogError("ERROR. No se ha podido agregar en la PBX la extension " + num_abonado);
	        	        	ret = false;
	        	        }
	    			}
	        	}
	        }
	        
	             
		} catch (Exception e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("CrearExtensionesPBX. Error creando o borrando extensiones en PBX");
			ret = false;
		}
        
        sem.release();  
        
        if (!ret) return ret;
        
        //Obtenemos la definitiva lista de las extensiones
		ext_array = GetExtensiones("user");
		if (ext_array == null) 
		{
			Utilities.MyLogError("ERROR CrearExtensionesPBX. No se puede obtener lista definitiva de extensiones tipo user");	
			ret = false;
		}
        else
        {
	        Utilities.MyLog("Lista definitiva de extensiones en la PBX: ");
	        Utilities.MyLog(ext_array.toString());
	        Utilities.MyLog("---");
        }
        
		return ret;	    
	}
	
	/***********************************************************************************************************/
	/*
	 * Se crean los usuarios en la base de datos que luego utiliza "user authetication" en el SIP proxy
	 */
	static public boolean CrearUsuariosProxy()
	{
		boolean ret = true;		
		
		String crear_extensiones_y_usuarios = Utilities.GetSetting("CrearExtensionesYUsuarios");
		if (crear_extensiones_y_usuarios != null && crear_extensiones_y_usuarios.equals("0"))
		{
			//Lee la propiedad CrearExtensionesYUsuarios. La cual si tiene el valor 0
			//quiere decir que no queremos que se creen las extensiones y usuarios en la brekeke
			//desde la configuracion
			//Entonces se sale de esta funcion con valor true
			return ret;
		}
		
		Connection conex = ConnectToDB(Utilities.GetSetting("base_datos_user_authetication"), 
				Utilities.GetSetting("usuario_base_datos_user_authetication"), 
				Utilities.GetSetting("password_base_datos_user_authetication"));
		
		if (conex == null) return false;
		
		List<String> listaUsuariosEnBaseDatos = null; 
		listaUsuariosEnBaseDatos = GetListaUsuariosBaseDatos(conex, Utilities.GetSetting("tabla_base_datos_user_authetication"));
			
		Utilities.MyLog("Usuarios en la base de datos antes de borrar: ");
		Utilities.MyLog(listaUsuariosEnBaseDatos.toString());
		Utilities.MyLog("--- ");
		
		//Se borran de la base de datos los usuarios que no estan en la configuracion.
        for (int i = 0; i < listaUsuariosEnBaseDatos.size(); i++)
        {        	
        	String user = listaUsuariosEnBaseDatos.get(i);
        	if (!EstaEnListaAbonadosATSSectores(user) && !EstaEnListaTelefonosVoIP(user))
       		{
       			//Borra el usuario del sip proxy
       			if (conex != null)
       			{       				
       				ret = BorrarUsuarioDeTablaBaseDatos(conex, Utilities.GetSetting("tabla_base_datos_user_authetication"), user);
       			}
       		}
        }
        
        //Obtenemos la nueva lista de los usuarios
		listaUsuariosEnBaseDatos = GetListaUsuariosBaseDatos(conex, Utilities.GetSetting("tabla_base_datos_user_authetication"));
			
		Utilities.MyLog("Usuarios en base de datos despues de borrar:");
		Utilities.MyLog(listaUsuariosEnBaseDatos.toString());
        Utilities.MyLog("--- ");
        
        try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("CrearExtensionesPBX. No puede adquirir el semaforo sem");
			return false;
		}
        
        try
        {
        
	        Utilities.MyLog(" --------------- CREA USUARIOS DE LOS SECTORES -------------------");
	        //Se crean las extensiones de los sectores del scv en la PBX
	        if (UlisesConfigObj != null && UlisesConfigObj.listaAbonadosATSSectores != null)
	        {
	        	for (int i = 0; i < UlisesConfigObj.listaAbonadosATSSectores.size(); i++)
	        	{
	        		String num_abonado = UlisesConfigObj.listaAbonadosATSSectores.get(i);
	        		
	        		if ((conex != null) && (!listaUsuariosEnBaseDatos.contains(num_abonado)))
	    	        {
	    	        	//Se agrega a la base de datos si no esta incluida
	    	        	ret = AgregarUsuarioATablaBaseDatos(conex, Utilities.GetSetting("tabla_base_datos_user_authetication"), num_abonado);
	    	        }
	        	}
	        }
	        
	        Utilities.MyLog(" -------------- CREA USUARIOS DE LOS TELEFONOS VOIP --------------------- ");
	        
	        //Se crean las extensiones de los telefonos VoIP del SCV en la PBX
	        if (UlisesConfigObj != null && UlisesConfigObj.listaTelefonosVoIP != null)
	        {        	
	        	for (int i = 0; i < UlisesConfigObj.listaTelefonosVoIP.size(); i++)
	        	{
	        		String idrecurso = UlisesConfigObj.listaTelefonosVoIP.get(i);
	        		
	        		if ((conex != null) && (!listaUsuariosEnBaseDatos.contains(idrecurso)))
	    	        {
	    	        	//Se agrega a la base de datos si no esta incluida
	    	        	ret = AgregarUsuarioATablaBaseDatos(conex, Utilities.GetSetting("tabla_base_datos_user_authetication"), idrecurso);
	    	        }
	        	}
	        }      
        } catch (Exception e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("CrearUsuariosProxy. Error creando o borrando usuarios en Proxy");
			ret = false;
		}
        
        sem.release(); 
        
        //Obtenemos la nueva lista de los usuarios definitiva en la base de datos
		listaUsuariosEnBaseDatos = GetListaUsuariosBaseDatos(conex, Utilities.GetSetting("tabla_base_datos_user_authetication"));
			
		Utilities.MyLog("Lista definitiva de usuarios en base de datos: ");
		Utilities.MyLog(listaUsuariosEnBaseDatos.toString());
        Utilities.MyLog("--- ");
		
		CloseConnection(conex);
		
		return ret;		
	}
	
	/***********************************************************************************************************/
	/*Obtiene y guarda en una lista de los numeros de abonados de los sectores
	 */
	static private List<String> GetListaAbonadosATSSectores(List<DireccionamientoSIP> planDireccionamientoSip)
	{	
		Utilities.MyLogDeb("Entra en GetListaAbonadosSectores");
		
		List<String> listaAbonadosSectores = new ArrayList<String>();
		
		for (int i = 0; i < planDireccionamientoSip.size(); i++)
    	{
    		int num_abonados_que_atiende = planDireccionamientoSip.get(i).getNumerosAbonadoQueAtiende().getStrNumeroAbonado().size();
    		for (int j = 0; j < num_abonados_que_atiende; j++)
    		{
    			long prefijo = planDireccionamientoSip.get(i).getNumerosAbonadoQueAtiende().getStrNumeroAbonado().get(j).getPrefijo();
    			if (prefijo == 3)
    			{
    				//Solamente tomamos los abonados del tipo ATS
	    			String num_abonado = planDireccionamientoSip.get(i).getNumerosAbonadoQueAtiende().getStrNumeroAbonado().get(j).getNumeroAbonado();
	    			listaAbonadosSectores.add(num_abonado);
    			}
    		}
    	}		
		
		Utilities.MyLogDeb("Lista de abonados obtenida por SOAP: ");
		Utilities.MyLogDeb(listaAbonadosSectores.toString());
		Utilities.MyLogDeb(" ");
		
		return listaAbonadosSectores;		
	}
	
	/***********************************************************************************************************/
	/*Return true si el abonado exite en la lista de Abonados de los sectores 
	 */
	static private boolean EstaEnListaAbonadosATSSectores(String ab)
	{
		boolean ret = false;
		
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("EsTelefonoVoIP. No puede adquirir el semaforo sem");
			return ret;
		}
		
		if (UlisesConfigObj == null) 
		{
			sem.release();
			return ret;
		}
		
		ret = UlisesConfigObj.listaAbonadosATSSectores.contains(ab);
		
		sem.release();
		
		return ret;
	}
	
	/***********************************************************************************************************/
	/*Obtiene y guarda en una lista de los telefonos VoIP
	 */
	static private List<String> GetListaTelefonosVoIP(List<AsignacionRecursosGW> asignacion_recursos, List<DireccionamientoIP> planDireccionamientoIp)
	{
		Utilities.MyLogDeb("Entra en GetListaTelefonosVoIP");
		
		List<String> listaTelefonosVoIP = new ArrayList<String>();
		
		for (int i = 0; i < asignacion_recursos.size(); i++)
    	{
    		String idrecurso = asignacion_recursos.get(i).getIdRecurso();
    		String idhost = asignacion_recursos.get(i).getIdHost();
		
			for (int j = 0; j < planDireccionamientoIp.size(); j++)
			{
				if ((planDireccionamientoIp.get(j).getIdHost().equals(idhost)) &&
					(planDireccionamientoIp.get(j).getTipoHost() == TipoElementoHW.TEH_EXTERNO_TELEFONIA) &&
					(planDireccionamientoIp.get(j).isInterno() == true) &&
					(planDireccionamientoIp.get(j).isEsCentralIP() == true))
				{
					//Es un telefono VoIP
					listaTelefonosVoIP.add(idrecurso);
					Utilities.MyLogDeb("EsTelefonoVoIP "+ idrecurso);
					break;
				}		
			}
    	}
		
		Utilities.MyLogDeb("Lista de telefonos VoIP obtenida por SOAP: ");
		Utilities.MyLogDeb(listaTelefonosVoIP.toString());
		Utilities.MyLogDeb(" ");
		
		return listaTelefonosVoIP;		
	}
	
	/***********************************************************************************************************/
	/*Return true si el abonado exite en la lista de telefonos VoIP
	 */
	static private boolean EstaEnListaTelefonosVoIP(String ab)
	{
		boolean ret = false;
		
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("EsTelefonoVoIP. No puede adquirir el semaforo sem");
			return ret;
		}
		
		if (UlisesConfigObj == null) 
		{
			sem.release();
			return ret;
		}
		
		ret = UlisesConfigObj.listaTelefonosVoIP.contains(ab);
		sem.release();
		
		return ret;
	}
	
	static private int id_message = 1;			//id del mensaje que se envia por websocket
	
	/***********************************************************************************/
	/* Retorna un JSONArray con una lista de extensiones
	 * Parametro:
	 * type. Tipo de extension: user (extension simple), ringgroup (Grupo de extensiones).
	 * Retorna un array vacio si el websocket retorna error
	 * Retorna null si no se han podido tomar las extensiones
	 */
	@SuppressWarnings("unchecked")
	static private JSONArray GetExtensiones(String type)
	{
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Entra en GetExtensiones");
		
		if (wclenp == null) return null;
		
		int tries = 3;		
		while (tries > 0)
		{		
			wclenp_sem.drainPermits();
			resultado_obtenido = null;
			
			/*Enviamos methodo getExtensions. para obtener la lista de grupos de extensiones actuales en la PBX*/
			JSONObject obj = new JSONObject();
	        obj.put("jsonrpc", "2.0");
			obj.put("method", "getExtensions");
			id_message++;
			if (id_message == 50000) id_message = 1;
			id_esperado = Integer.toString(id_message);
			obj.put("id", id_esperado);
			
			JSONObject obj_param = new JSONObject();
			obj_param.put("pattern", ".+");	
			obj_param.put("limit", "-1");
			obj_param.put("type", type);
			obj.put("params", obj_param);		
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
				Utilities.MyLogError("ERROR Sin respuesta al mensaje por websocket "+message);				
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
		
		JSONArray result = (JSONArray) resultado_obtenido.get("result");
		if (result == null) return null;		
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Lista de extensiones leida correctamente por websocket type: "+type + " result: " + result.toString());
		
		return result;
	}
	
	private static int ERROR_AL_CREAR_EXTENSION_PBX = 0;
	private static int CREADA_EXTENSION_PBX = 1;
	private static int MAXIMO_NUMERO_EXTENSIONES_PBX_SOBREPASADA = 2;
	@SuppressWarnings("unchecked")
	static private int CrearUserExtensionPBX(String ext)
	{
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Entra en CrearUserExtensionPBX");
		
		if (wclenp == null) return ERROR_AL_CREAR_EXTENSION_PBX;
		
		int tries = 3;
		while (tries > 0)
		{
			wclenp_sem.drainPermits();
			resultado_obtenido = null;
			
			/*Enviamos methodo createExtension. para crear la extension*/
			JSONObject obj = new JSONObject();
	        obj.put("jsonrpc", "2.0");
			obj.put("method", "createExtension");
			id_message++;
			if (id_message == 50000) id_message = 1;
			id_esperado = Integer.toString(id_message);
			obj.put("id", id_esperado);
			
			JSONObject obj_param = new JSONObject();
			obj_param.put("extension", ext);
			obj_param.put("password", ext);
			obj_param.put("login_password", ext);
			obj_param.put("type", "user");
			obj.put("params", obj_param);		
			String message = obj.toString();
			wclenp.sendMessage(message);
			
			boolean semr = false;
			try
			{
				semr = wclenp_sem.tryAcquire(15, TimeUnit.SECONDS);
			}
			catch (Exception e)
			{
				semr = false;
			}
			if (semr == false)
			{
				//No ha habido respuesta
				Utilities.MyLogError("ERROR Sin respuesta al mensaje por websocket "+message);
			}
			else if (resultado_obtenido != null)
			{
				//Ha habido respuesta
				break;
			}
			tries--;
		}
		
		if (tries <= 0)
		{
			//No ha habido respuesta despues de varios intentos
			//Cerramos el websocket para que vuelva a intentar abrirlo
			if (wclenp != null) wclenp.Close();
			wclenp = null;
			return ERROR_AL_CREAR_EXTENSION_PBX;
		}
		
		String id = (String) resultado_obtenido.get("id");
		if (id == null) return ERROR_AL_CREAR_EXTENSION_PBX;		
		if (id.equals(id_esperado) == false) return ERROR_AL_CREAR_EXTENSION_PBX;
		
		String result = (String) resultado_obtenido.get("result");
		if (result == null)
		{
			JSONObject error = (JSONObject) resultado_obtenido.get("error");
			if (error != null) 
			{
				String message = (String) error.get("message");
				if (message != null)
				{
					if (message.startsWith("Maximum Users exceeded"))
					{
						//No se puede agregar mas extensiones. La licencia no lo permite
						Utilities.MyLogError("ERROR Extension " + ext + " no se puede crear. Maximo numero de extensiones sobrepasado");
						return MAXIMO_NUMERO_EXTENSIONES_PBX_SOBREPASADA;
					}
				}
			}
			return ERROR_AL_CREAR_EXTENSION_PBX;
		}
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Resultado de crear extension "+ext+" por WebSocket result: " + result.toString());
		
		if (result.equals("true") == true)
		{
			if (Utilities.LOGDEBENABLED) Utilities.MyLogDeb("Extension creada con exito "+ext);
		}
		else
		{
			if (Utilities.LOGDEBENABLED) Utilities.MyLogError("Extension no puede ser creada "+ext);
			return ERROR_AL_CREAR_EXTENSION_PBX;			
		}
		
		/*Creamos las propiedades de la extension*/
		
		tries = 3;		
		while (tries > 0)
		{
			wclenp_sem.drainPermits();
			resultado_obtenido = null;
			
			JSONObject obj = new JSONObject();
	        obj.put("jsonrpc", "2.0");
			obj.put("method", "setExtensionProperties");
			id_message++;
			if (id_message == 50000) id_message = 1;
			id_esperado = Integer.toString(id_message);
			obj.put("id", id_esperado);
			
			JSONObject obj_param = new JSONObject();
			obj_param.put("extension", ext);		
			
			JSONObject obj_param_properties = new JSONObject();	
			obj_param_properties.put("type", "user");
			obj_param_properties.put("desc", "descripcion");
			obj_param_properties.put("busyforward.voicemail", "false");
			obj_param_properties.put("noanswerforward.voicemail", "false");
			obj_param_properties.put("pln1_d_busyforward.voicemail", "false");
			obj_param_properties.put("pln1_d_noanswerforward.voicemail", "false");
			obj_param_properties.put("pln1_d_ringertime", "300");
			obj_param.put("properties", obj_param_properties);
			
			obj.put("params", obj_param);
			
			String message = obj.toString();
			wclenp.sendMessage(message);
			
			boolean semr = false;
			try
			{
				semr = wclenp_sem.tryAcquire(15, TimeUnit.SECONDS);
			}
			catch (Exception e)
			{
				semr = false;
			}
			if (semr == false)
			{
				//No ha habido respuesta
				Utilities.MyLogError("ERROR Sin respuesta al mensaje por websocket "+message);
			}
			else if (resultado_obtenido != null)
			{
				//Ha habido respuesta
				break;
			}
			tries--;
		}
		
		if (tries <= 0)
		{
			//No ha habido respuesta despues de varios intentos
			//Cerramos el websocket para que vuelva a intentar abrirlo
			if (wclenp != null) wclenp.Close();
			wclenp = null;
			return ERROR_AL_CREAR_EXTENSION_PBX;
		}
		
		id = (String) resultado_obtenido.get("id");
		if (id == null) return ERROR_AL_CREAR_EXTENSION_PBX;		
		if (id.equals(id_esperado) == false) return ERROR_AL_CREAR_EXTENSION_PBX;
		
		result = (String) resultado_obtenido.get("result");
		if (result == null) return ERROR_AL_CREAR_EXTENSION_PBX;		
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Resultado de setExtensionProperties "+ext+" por WebSocket result: " + result.toString());
		
		if (result.equals("OK") == true)
		{
			if (Utilities.LOGDEBENABLED) Utilities.MyLogDeb("Propiedades de Extension creada con exito "+ext);
			return CREADA_EXTENSION_PBX;
		}
		else
		{
			if (Utilities.LOGDEBENABLED) Utilities.MyLogError("Propiedades de Extension no pueden ser creadas "+ext);
			return ERROR_AL_CREAR_EXTENSION_PBX;			
		}
	}
	
	/* Se conecta con una base de datos mysql*/
	/* DBcon. Base de datos, del tipo "jdbc:mysql://localhost/prueba"
	 * user. Usuario para acceder a la base de datos
	 * pass. password.
	 * Retorna la Connection, o null si no tiene exito
	 */
	static private Connection ConnectToDB(String DBcon, String user, String pass)
	{
		Connection conexion = null;
		try
		{
		   Class.forName("com.mysql.jdbc.Driver");
		} catch (Exception e)
		{
		   Utilities.MyLogError("El driver com.mysql.jdbc.Driver no se puede registrar");
		   return null;
		}
		
		try {
			conexion = DriverManager.getConnection (DBcon, user, pass);
		} catch (Exception e) {
			Utilities.MyLogError("No se puede connectar a la base de datos "+DBcon);
			return null;
		}
		return conexion;
	}
	
	static private void CloseConnection(Connection conexion)
	{
		try {
			conexion.close();
		} catch (Exception e) {
			Utilities.MyLogError("No se puede cerrar la conexion con la base de datos");
		}		
	}
	
	/*
	static private void BorrarTablaDeBaseDatos(Connection conexion, String tabla)
	{
		Statement s;
		try {
			s = (Statement) conexion.createStatement();	
			((java.sql.Statement) s).executeUpdate ("DELETE FROM "+tabla);
		} catch (Exception e) {
			Utilities.MyLogError("No se puede borrar el contenido de la tabla "+tabla);
		}				
	}
	*/
	
	static private List<String> GetListaUsuariosBaseDatos(Connection conexion, String tabla)
	{
		List<String> ret = new ArrayList<String>();
		try {
			Statement st = conexion.createStatement();
		    ResultSet res = st.executeQuery("SELECT * FROM " + tabla);
		    while (res.next()) {
		      String user = res.getString("username");
		      ret.add(user);
		    }
		} catch (Exception e) {
			Utilities.MyLogError("GetListaUsuariosBaseDatos. tabla "+tabla);			
		}	
		return ret;
	}
	
	static private boolean AgregarUsuarioATablaBaseDatos(Connection conexion, String tabla, String usuario)
	{
		String st = null;
		int uid = 0;
		Statement s;		
		try {
			s = (Statement) conexion.createStatement();				
			ResultSet res = s.executeQuery("SELECT MAX(uid) FROM " + tabla);
			if (res.next()) {
				uid = res.getInt(1);
			}
			uid += 1;
			st = "INSERT INTO "+tabla+" VALUES ("+uid+", \""+usuario+"\", \""+usuario+"\", \""+usuario+"\", \"\",\""+usuario+"\", 0, 0,"+uid+")";
			((java.sql.Statement) s).executeUpdate (st);
			Utilities.MyLogDeb("Agregado a la base de datos con exito usuario "+usuario);
		} catch (SQLException e) {
			Utilities.MyLogError("No se puede ejecutar el estamento "+st);
			return false;
		}
		return true;
	}
	
	static private boolean BorrarUsuarioDeTablaBaseDatos(Connection conexion, String tabla, String usuario)
	{
		Statement s;
		
		String st = "DELETE FROM "+tabla+" WHERE username = \"" + usuario + "\"";
		try {
			s = (Statement) conexion.createStatement();	
			((java.sql.Statement) s).executeUpdate (st);
		} catch (SQLException e) {
			Utilities.MyLogError("No se puede ejecutar el estamento "+st);
			return false;
		}
		
		Utilities.MyLogDeb("Borrado de la base de datos con exito usuario "+usuario);
		
		return true;
	}
	
	@SuppressWarnings("unchecked")
	static private boolean BorrarExtensionPBX(String ext)
	{
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Entra en BorrarExtensionPBX");
		
		if (wclenp == null) return false;
		
		int tries = 3;
		while (tries > 0)
		{
			wclenp_sem.drainPermits();
			resultado_obtenido = null;
			
			JSONObject obj = new JSONObject();
	        obj.put("jsonrpc", "2.0");
			obj.put("method", "deleteExtension");
			id_message++;
			if (id_message == 50000) id_message = 1;
			id_esperado = Integer.toString(id_message);
			obj.put("id", id_esperado);
			
			JSONObject obj_param = new JSONObject();
			obj_param.put("extension", ext);
			obj.put("params", obj_param);
			
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
				Utilities.MyLogError("ERROR Sin respuesta al mensaje por websocket "+message);				
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
			return false;
		}
		
		String id = (String) resultado_obtenido.get("id");
		if (id == null) return false;		
		if (id.equals(id_esperado) == false) return false;
		
		String result = (String) resultado_obtenido.get("result");
		if (result == null) return false;		
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Resultado de borrar extension "+ext+" por WebSocket result: " + result.toString());
		
		if (result.equals("true"))
		{
			Utilities.MyLogDeb("Extension borrada con exito "+ext);
			return true;
		}
		else
		{
			Utilities.MyLogError("Extension no puede ser borrada "+ext);
			return false;			
		}		
	}
	
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
		
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Entra GetBrekekeVersion");
		
		Get_WebsocketClientEndpoint();
		if (wclenp == null)
		{
			Utilities.MyLogError("ERROR: GetBrekekeVersion. No puede abrirse el websocket ");
			return null;
		}
			
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
				Utilities.MyLogError("ERROR: GetBrekekeVersion. Sin respuesta al mensaje por websocket "+message);				
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
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("GetBrekekeVersion result: " + result.toString());
		
		version_returned = (String) result.get("version");
		Utilities.MyLog("GetBrekekeVersion version_returned: " + version_returned);
		
		return version_returned;
	}
	
	
}


