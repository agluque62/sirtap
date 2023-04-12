package plugin;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.*;
import java.util.concurrent.Semaphore;

/*Conjunto de dependencias con sus recursos*/
public class DependenciasSet
{	
	/*Definicion de state de los recursos telefonicos
	0. Idle
	1. BusyInterruptionAllow
	2. BusyInterruptionNotAllow
	3. NotAvailable
	*/
	public final int STATE_IDLE = 0;
	public final int STATE_BUSY_INT_ALLOW = 1;
	public final int STATE_BUSY_INT_NO_ALLOW = 2;
	public final int STATE_NOT_AVAIL = 3;	
	
	private Map<String,Dependencia> DependSet;	/*La clave del map es el identificador de la dependencia*/	
	private Date ultima_limpia_date;
	private Semaphore sem;
	
	public DependenciasSet()
	{
		sem = new Semaphore(1);
		ultima_limpia_date = new Date();
		DependSet = new HashMap<String,Dependencia>();		
	}
	
	//Se agrega la dependencia a partir del paquete multicast recibido	
	public void Add(byte[] buf, int buf_length) 		
	{
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError("DependenciasSet.Add. No se puede adquirir semaforo. Excepcion: "+e.getMessage());
			return;
		}
		Dependencia newdep;
		try {
			newdep = new Dependencia(buf, buf_length);
		} catch (Exception e) {
			Utilities.MyLogDeb("DependenciasSet.Add. No se puede crear Dependencia. Mensaje TIFX " + buf.toString() + " que no puede parsear. Excepcion: "+e.getMessage());
			sem.release();
			return;
		}		
		DependSet.put(newdep.GwId, newdep);
		sem.release();
	}	
	
	//Retorna un array con todas las claves del map
	public String[] GetAllKeys() 
	{
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError("DependenciasSet.GetAllKeys. No se puede adquirir semaforo");
			return new String[0];
		}
		String[] ret = new String[DependSet.size()];
		Iterator<String> iterator = DependSet.keySet().iterator();		
		
		int i = 0;
		while(iterator.hasNext())
		{
			String key = (String) iterator.next();
			ret[i] = key;
			i++;
		}
		sem.release();
		return ret;
	}
	
	//Retorna los recursos de una dependencia key. 
	//Retorna null si no la encuentra
	public Dependencia Get(String key) 
	{
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError("DependenciasSet.Get. No se puede adquirir semaforo");
			return null;
		}
		Dependencia dep = DependSet.get(key);
		sem.release();
		return dep;
	}
	
	//Retorna todos los identificadores de recuros para una dependencia 
	public String[] GetAllRs(String Dep)
	{
		String[] ret = null;
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError("DependenciasSet.GetAllRs. No se puede adquirir semaforo");
			return new String[0];
		}
		Dependencia dep = DependSet.get(Dep);
		if (dep == null) 
		{
			ret = new String[0];
			sem.release();
			return ret;
		}
		
		ret = new String[dep.Resources.size()];		
		Iterator<String> iterator = dep.Resources.keySet().iterator();		
		
		int i = 0;
		while(iterator.hasNext())
		{
			String key = (String) iterator.next();
			ret[i] = key;
			i++;
		}
		sem.release();
		return ret;
	}
	
	//Retorna la info de un recurso Rs de una dependencia Dep
	public RsInfo Get(String Dep, String Rs)
	{
		RsInfo rsInfo = null;
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError("DependenciasSet.Get RsInfo. No se puede adquirir semaforo");
			return null;
		}
		Dependencia dep = DependSet.get(Dep);
		if (dep != null)
		{
			rsInfo = dep.Resources.get(Rs);
		}
		sem.release();
		return rsInfo;
	}
	
	//Elimina los elementos que no se han actualizado despues de pasado un tiempo
	public void Limpiar()
	{
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			Utilities.MyLogError("DependenciasSet.Limpiar. No se puede adquirir semaforo");
			return;
		}
		if (DependSet.size() == 0) 
		{
			//Nada que limpiar
			sem.release();
			return;			
		}
		
		Date ahora = new Date();
		int dif_ultima_limpia=(int) ((ahora.getTime()-ultima_limpia_date.getTime())/1000); //Segundos
		
		if (dif_ultima_limpia < 60) 
		{
			//No ha pasado suficiente tiempo para ejectuar la limpieza
			sem.release();
			return;
		}		
		
		String[] elementos_a_limpiar = new String[DependSet.size()];
		int elementos_a_limpiar_count = 0;
		Iterator<String> iterator = DependSet.keySet().iterator();
		while(iterator.hasNext())
		{
			String key = (String) iterator.next();
			Dependencia dep = DependSet.get(key);
			 
			int diferencia=(int) ((ahora.getTime()-dep.ultima_actualizacion_date.getTime())/1000);  //En segundos
			if (diferencia >= 120)
			{
				elementos_a_limpiar[elementos_a_limpiar_count] = key;	
				elementos_a_limpiar_count++;
			}		  
		}
		
		for (int i = 0; i < elementos_a_limpiar_count; i++)
		{
			DependSet.remove(elementos_a_limpiar[i]);			
		}
		sem.release();
		ultima_limpia_date = new Date();
	}
	
	public void dump_dependencies()
	{
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("---------ESTADO RECURSOS RED--------");
		String[] depKeys = GetAllKeys();
		for (int j = 0; j < depKeys.length; j++)
		{
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("DEPENDENCIA:");
			Dependencia dep = Get(depKeys[j]);					
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("Type "+dep.Type+" GwId "+dep.GwId+" NumRs "+dep.NumRs+" Version "+dep.Version+	
					" Ultima actualizacion"+dep.ultima_actualizacion_date);
			
			if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("RECURSOS:");
			String [] rsKeys = GetAllRs(depKeys[j]);					
			for (int k = 0; k < rsKeys.length; k++)
			{
				RsInfo rsinfo = Get(depKeys[j], rsKeys[k]);
				if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("RsId "+rsKeys[k]+" Type "+rsinfo.Type+
						" Version "+rsinfo.Version+" State "+rsinfo.State+
						" Priority "+rsinfo.Priority+" Steps "+rsinfo.Steps+
						" CallBegin "+rsinfo.CallBegin);						
			}					
		}
		if (Utilities.LOGDEBENABLED)  Utilities.MyLogDeb("------------------------------------");
	}
}

class RsInfo {	
	int Type = 0;	
	int Version = 0;
	int State = 0;
	int Priority = 0;
	int Steps = 0;
	int CallBegin = 0;
	
	public RsInfo()
	{
		Type = 0;	
		Version = 0;
		State = 0;
		Priority = 0;
		Steps = 0;
		CallBegin = 0;		
	}
}

class Dependencia			/*Pasarela, proxy, etc*/
{	
	public int Type;
	public String GwId;
	public int NumRs;
	public int Version;
	public Map<String,RsInfo> Resources;		/*Recursos dentro de la dependencia. La clave es el identificador del recurso*/
	public Date ultima_actualizacion_date;
	
	public Dependencia(byte[] buf, int buf_length) throws Exception 
	{	
		ultima_actualizacion_date = new Date();
		Resources = new HashMap<String,RsInfo>();		
		
		int count = 0;
		
	    Type = getInt(buf, count, buf_length);	
	    count += 4;

	    GwId = getString(buf, count, buf_length);
    	count += 36;
	    
    	NumRs = getInt(buf, count, buf_length);
	    count += 4;
	    
	    Version = getInt(buf, count, buf_length);
	    count += 4;
	    
	    for (int i = 0; i < NumRs; i++)
	    {
	    	RsInfo rs = new RsInfo();

	    	String RsId = getString(buf, count, buf_length);
	    	count += 36;
	    	
	    	rs.Type = getInt(buf, count, buf_length);
		    count += 4;
		    
		    rs.Version = getInt(buf, count, buf_length);
		    count += 4;
	    		
		    rs.State = getInt(buf, count, buf_length);
		    count += 4;
		    
		    rs.Priority = getInt(buf, count, buf_length);
		    count += 4;
		    
		    rs.Steps = getInt(buf, count, buf_length);
		    count += 4;
		    
		    rs.CallBegin = getInt(buf, count, buf_length);
		    count += 4;
		    
		    Resources.put(RsId, rs);
	    }	    
	}
	
	//A partir de la posicion de un byte buffer extrae un int
	private int getInt(byte[] buf, int buf_index, int buf_length) throws Exception 
	{
		if ((buf_index + 4) > buf_length)
		{
			throw new Exception();
		}
		
		byte[] buf_aux = new byte[4];		
		System.arraycopy(buf, buf_index, buf_aux, 0, 4);
		ByteBuffer bb = ByteBuffer.wrap(buf_aux);
	    bb.order(ByteOrder.BIG_ENDIAN);
	    return bb.getInt();		
	}
	
	//A partir de la posicion de un byte buffer extrae un String
	private String getString(byte[] buf, int buf_index, int buf_length) throws Exception
	{
		int i = 0;
		while ((buf[i+buf_index] != 0) && (i < buf_length)) i++;
		if (i == buf_length) 
		{
			throw new Exception();
		}
		
		byte[] buf_aux = new byte[i+1];
		i = 0;
		while (buf[i+buf_index] != 0)
		{
			buf_aux[i] = buf[i+buf_index]; 
			i++;
		}
		buf_aux[i] = 0;
		String ret = new String(buf_aux, 0, i);
		return ret;	
	}
	
}
