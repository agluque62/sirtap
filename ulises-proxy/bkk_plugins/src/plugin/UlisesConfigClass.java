/**********************************************************************************/
/*Esta clase contiene los datos leidos de la configuracion del Ulises
 * 
 */

package plugin;

import java.util.List;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import es.cd40.*;

public class UlisesConfigClass {
	
	public String VersionConfiguracion;
	public List<AsignacionRecursosGW> asignacion_recursos;
	public List<AsignacionUsuariosTV> asignacion_usuarios;
	public List<DireccionamientoIP> planDireccionamientoIp;
	public List<DireccionamientoSIP> planDireccionamientoSip;
	public List<NumeracionATS> numeracionATS;
	public List<ListaRedes> listaRedes;
	public List<ListaTroncales> listaTroncales;
	public List<String> listaAbonadosATSSectores;
	public List<String> listaTelefonosVoIP;
	public List<Conferencia> conferenciasPreprogramadas;
	
	UlisesConfigClass()
	{
		VersionConfiguracion = "Version";
	}
	
	public boolean TodosLosElementosNonNull()
	{
		if (VersionConfiguracion == null) return false;
		if (asignacion_recursos == null) return false;
		if (asignacion_usuarios == null) return false;
		if (planDireccionamientoIp == null) return false;
		if (planDireccionamientoSip == null) return false;
		if (numeracionATS == null) return false;
		if (listaRedes == null) return false;
		if (listaTroncales == null) return false;
		if (listaAbonadosATSSectores == null) return false;
		if (listaTelefonosVoIP == null) return false;
		if (conferenciasPreprogramadas == null) return false;
		return true;
	}
	
	@Override
    public boolean equals(Object o)
    {
		if (o == this) 
		{
			return true;
		}
		
		if (!(o instanceof UlisesConfigClass)) 
		{
            return false;
        }
		
		UlisesConfigClass cfgobj = (UlisesConfigClass) o;
		
		ObjectMapper objectMapper = new ObjectMapper();
		
		try {
			
			String thisAsString = objectMapper.writeValueAsString(this);
			Utilities.MyLogDeb("thisAsString:  " + thisAsString);
			String cfgobjAsString = objectMapper.writeValueAsString(cfgobj);
			Utilities.MyLogDeb("cfgobjAsString:  " + cfgobjAsString);
			
			boolean ret = thisAsString.equals(cfgobjAsString);
			
			if (!ret)
			{
				Utilities.MyLog("-------- OBJETOS CONFIGURACION DIFERENTES --------");
				Utilities.MyLogDeb("--------> ACTUAL:  ");
				Utilities.MyLogDeb(thisAsString);
				Utilities.MyLogDeb("<-------- ");
				Utilities.MyLogDeb("--------> NUEVO:  ");
				Utilities.MyLogDeb(cfgobjAsString);
				Utilities.MyLogDeb("<-------- ");				
			}
			
			return ret;
			
		} catch (JsonProcessingException e) {
			return false;
		}
    }
}
