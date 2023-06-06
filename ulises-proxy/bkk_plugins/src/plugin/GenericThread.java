package plugin;

public class GenericThread implements Runnable 
{
	private boolean run_th = true;
	
	public void parar()
    {
		run_th = false;
    }
	
	
	public void run() 
	{
		try {
			
			Utilities.MyLog("---------RUN GENERIC THREAD --------");
			
			Thread.sleep(5000);
			
			//Lo primero que hacemos es leer la configuracion del Ulises desde el fichero
			if (SoapConfig.ReadUlisesConfigFromFile() == true)
			{
				SoapConfig.Creados_ExtensionesPBX = SoapConfig.CreaExtensionesPBXPorWebsocket();
				SoapConfig.Creados_UsuariosProxy = SoapConfig.CrearUsuariosProxy();
				SoapConfig.Creadas_SalasConferencia = SoapConfig.CrearConferenciasPreprogramadasPBX();
			}
			
			while (run_th)
			{			 
				Thread.sleep(5000);
				
				Utilities.GetLogPropeties();
				
				try {
					Thread.sleep(100);
				} catch (InterruptedException e1) {
					
				}
				
				SoapConfig.ReadConfig(); 
			}
		} catch (Exception e) {
			Utilities.MyLogError(e.toString()+" "+e.getMessage());
			Utilities.MyLogError("GenericThread. excepcion");
		}
	 }
}
