<%@ Application Language="C#" %>
<%@ Import Namespace="log4net" %>
<%@ Import Namespace="log4net.Config" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Net" %>
<%@ Import Namespace="System.Timers" %>
<%@ Import Namespace="System.Configuration" %>
<%@ Import Namespace="System.Web.Configuration" %>
<%@ Import Namespace="System.Runtime.Serialization.Formatters.Binary" %>
<%@ Import Namespace="ServiciosCD40" %>

<script runat="server">


   static IPEndPoint _EndPoint1;
   static IPEndPoint _EndPoint2;
   static System.Timers.Timer _PeriodicTasks;
   static DateTime _LastReceivedNodo1;
   static DateTime _LastReceivedNodo2;
   static byte[] _ActivateMsg;
   static byte[] _AskInfoMsg;
   static bool _Disposed;
   //static ServiciosCD40 ForClusterState;
   static string idioma;
   static string sistema;
   
   //Parámetros leidos del fichero de configuración Web.config
   static string strLocalIp;      //ListenIP: direccion IP local
   static int ilocalPort;        //ListenPort: puerto IP local
   static string strEpNodo1Ip;   //ClusterSrv1Ip
   static int iEpNodo1Port;      //ClusterSrv1Port

   static string strEpNodo2Ip;   //ClusterSrv2Ip
   static int iEpNodo2Port;      //ClusterSrv2Port
   static bool bConexionSocket = false;
   static int iTiempoRefresco = 3000;
   static int iTiempoReconexion = 60000;
   
   private static readonly ILog logDebugView = LogManager.GetLogger("CLUSTER");
   
   void Application_OnStart(object sender, EventArgs e) 
   {
      log4net.Config.XmlConfigurator.Configure();
       string strAux=string.Empty;
       _Disposed = true; //Indica que el objeto _Comm es null y está liberado
       // RQF-30 
       Application["UsersLoggedIn"] = new System.Collections.Generic.List<string>(); 
      try
      {
          //Esta función se ejecuta la primera vez que se accede a la aplicacion a través del navegador, es decir, cuando 
          //se crea el primer objeto httpAplication
          logDebugView.Info("    ");
          logDebugView.Info("(Global.asax-Application_Start): INICIO DE LA APLICACION ...");

          bConexionSocket = false;

          //Se leen los parámetros de configuración del fichero Web.config
          Configuration config = WebConfigurationManager.OpenWebConfiguration("~");

          if (null!=config.AppSettings.Settings["Sistema"])
            sistema = config.AppSettings.Settings["Sistema"].Value;

          idioma = System.Globalization.CultureInfo.InstalledUICulture.TwoLetterISOLanguageName;

          //Si se configura en modo cluster, la entrada Servidor-2 debe tomar el valor SI
          if (null!=config.AppSettings.Settings["Servidor-2"] && string.Compare(config.AppSettings.Settings["Servidor-2"].Value, "SI", true) == 0)
          {
              logDebugView.Info("(Global.asax-Application_Start): Aplicacion configurada en modo Cluster");

              Application["Cnf_ModoCluster"] = true;

              if (bLeerParametrosConfiguracionCluster(ref config))
              {
                  //La primera vez el timer se inicia a los 15 segundos
                  _PeriodicTasks = new System.Timers.Timer(15000);
                  _PeriodicTasks.AutoReset = false;
                  _PeriodicTasks.Elapsed += PeriodicTasks;
                  _PeriodicTasks.Enabled = true;
              }
              else
              {
                  logDebugView.Error("(Global.asax-Application_Start): Se ha producido algún error en la lectura de los parámetros de configuracion. Por favor, revise el fichero " + config.FilePath);
              }
          }
          else
          {
              logDebugView.Info("(Global.asax-Application_Start): Aplicacion configurada en modo Servidor");
              Application["Cnf_ModoCluster"] = false;
          }


          // Al instalarlo en IIS no funciona
          //Application.Add("UserLogin", 0);
      }
      catch (Exception ex)
      {
          logDebugView.Error("(Global.asax-Application_Start): Se ha producido un error ", ex);
      }
   }

   void Session_Start(object sender, EventArgs e) 
	{
		 /* 
			 // Al instalarlo en IIS no funciona
			  //
		 Application.Lock();
		 Application["UserLogin"] = ((int)Application["UserLogin"]) + 1;
		 Application.UnLock();  
		  */
        logDebugView.Info("(Global.asax-Session_Start):    ---- INICIO DE SESION ----\n");
	 }

    void Session_End(object sender, EventArgs e) 
	{
        /*if (userLoggedIn.Length > 0) {
            System.Collections.Generic.List<string> d = Application["UsersLoggedIn"] 
                as System.Collections.Generic.List<string>;
            if (d != null) {
                lock (d) {
                    d.Remove(userLoggedIn);
                }
            }
        }
     */
        logDebugView.Info("(Global.asax-Session_End):    ---- FIN DE SESION ----\n");
	}

    void Application_OnEnd(object sender, EventArgs e) 
    {
        _Disposed = true;

        if (null != _PeriodicTasks)
        {
            _PeriodicTasks.Enabled = false;
            _PeriodicTasks.Close();
            _PeriodicTasks = null;
        }

        logDebugView.Info("(Global.asax-Application_OnEnd):    ---- PARADA DE LA APLICACION ----\n");
   }
     
   void Application_Error(object sender, EventArgs e) 
   { 
     // Code that runs when an unhandled error occurs
   }





   void PeriodicTasks(object sender, ElapsedEventArgs e)
   {
   }

    protected void Application_EndRequest(object sender, EventArgs e)
    {
        //if (HttpContext.Current.IsDebuggingEnabled)
        //    HttpContext.Current.Response.Write(
        //       "<div style='background: red; color: white'>Sitio en depuración</div>"
        //    );
    }

    //Devuelve true si la dirección IP que se le pasa como parámetro tiene un formato válido
    private bool bDireccionIPValida(string ipAddress)
    {
        bool bCorrecto = false;

        if (!string.IsNullOrEmpty(ipAddress))
        {
            try
            {
                IPAddress address = IPAddress.Parse(ipAddress);
                bCorrecto = true;
            }

            catch (ArgumentNullException e)
            {
                logDebugView.Error("Direccion IP no valida (ArgumentNullException) -> "+ e.Message);
            }

            catch (FormatException e)
            {
                logDebugView.Error("Direccion IP no es valida (Error de formato)-> "+ e.Message);
            }
            catch (Exception e)
            {
                logDebugView.Error("La Direccion IP no es valida (excepcion) -> "+ e.Message);
            }
        }
        
        return bCorrecto;
        
    }

    //Lee los parámetros del fichero Web.config, asociados con la monitorización del estado del cluster
    bool bLeerParametrosConfiguracionCluster(ref Configuration config)
    {
        bool bCorrecto=true;
        string strAux=string.Empty;
        const int TIEMPO_RECONEXION_SOCKET = 60000; //Se establece el tiempo de reconexión por defecto a 1 minuto 

        if (null!=config)
        {
            //Se comprueba la dirección IP y el puerto local configurado
            if (null == config.AppSettings.Settings["ListenIp"] || null == config.AppSettings.Settings["ListenPort"])
            {
                logDebugView.Error("(Global.asax-Application_Start): No se han encontrado las entradas ListenIp y/o ListenPort en el fichero de configuración.");
                bCorrecto=false;
            }
            
            if (null == config.AppSettings.Settings["ClusterSrv1Ip"] || null == config.AppSettings.Settings["ClusterSrv1Port"])
            {
                logDebugView.Error("(Global.asax-Application_Start): No se han encontrado las entradas ClusterSrv1Ip y/o ClusterSrv1Port con la dirección IP y el puerto del Nodo 1.");
                 bCorrecto=false;
            }
            
            if (null == config.AppSettings.Settings["ClusterSrv2Ip"] || null == config.AppSettings.Settings["ClusterSrv2Port"])
            {
                logDebugView.Error("(Global.asax-Application_Start): No se han encontrado las entradas ClusterSrv2Ip y/o ClusterSrv2Port con la dirección IP y el puerto del Nodo 2.");
                bCorrecto=false;
            }

            if (bCorrecto)
            {
                  if (null != config.AppSettings.Settings["TimeRefresStateCluster"])
                  {
                      //Se establece el tiempo de refresco configurado
                      try
                      {
                          iTiempoRefresco = Int32.Parse(config.AppSettings.Settings["TimeRefresStateCluster"].Value);

                          if (iTiempoRefresco < 2000)
                          {
                              //El tiempo minimo de refresco se configura a 2000 segundos.
                              iTiempoRefresco = 2000;
                              logDebugView.Error("El parámetro TimeRefresStateCluster debe tomar un valor mayor o igual a 2 s. Por defecto, se toma el valor " + iTiempoRefresco + " ms");
                          }
                      }
                      catch (Exception ex)
                      {
                        iTiempoRefresco=3000; //Se configura con 3 segundos.
                        logDebugView.Error("El parámetro de configuracion TimeRefresStateCluster en milisegundos no tiene un valor correcto. Por defecto, se toma el valor "+iTiempoRefresco+" ms. Error:"+ex.Message.ToString());
                      }
                  }
                  else
                  {
                     iTiempoRefresco=3000; //Se configura con 3 segundos.
                     logDebugView.Error("(Global.asax-Application_Start): No se han encontrado la entrada TimeRefresStateCluster en el fichero de configuracion  Web.config. Por defecto, se configura a 3000 ms.");
                  }
                  
                  //Se comprueban que las direcciones IP configuradas tienen una IP válida
                  //Direccion IP y puerto local
                  strAux=config.AppSettings.Settings["ListenIp"].Value;
                  
                  if (false==bDireccionIPValida(strAux))
                  {
                    logDebugView.Error("(Global.asax-Application_Start): la dirección IP configurada en la la clave ListenIp ("+strAux+") no es válida.");
                    bCorrecto=false;
                  }
                  else
                  {
                    strLocalIp=strAux;
                  }
                
                  //Puerto local
                  strAux=config.AppSettings.Settings["ListenPort"].Value;
                  if (!string.IsNullOrWhiteSpace(strAux))
                  {
                    try
                    {
                        ilocalPort=Int32.Parse(strAux);
                    }
                    catch (Exception ex)
                    {
                        logDebugView.Error("(Global.asax-Application_Start): el parametro ListenPort no es correcto ("+strAux+")."+ex.Message);
                        bCorrecto=false;
                    }
                  }
                  else
                  {
                    logDebugView.Error("(Global.asax-Application_Start): el parametro ListenPort no es valido.");
                    bCorrecto=false;
                  }
                
                  //Direccion IP y puerto del Nodo 1 del cluster
                  strAux=config.AppSettings.Settings["ClusterSrv1Ip"].Value;
                  if (false==bDireccionIPValida(strAux))
                  {
                    logDebugView.Error("(Global.asax-Application_Start): la dirección IP configurada en la la clave ClusterSrv1Ip ("+strAux+") no es válida.");
                    bCorrecto=false;
                  }
                  else
                  {
                    strEpNodo1Ip=strAux;
                  }
                     
                  //Puerto 
                  strAux=config.AppSettings.Settings["ClusterSrv1Port"].Value;
                  if (!string.IsNullOrWhiteSpace(strAux))
                  {
                        try
                        {
                           iEpNodo1Port=Int32.Parse(strAux);
                        }
                        catch (Exception ex)
                        {
                          logDebugView.Error("(Global.asax-Application_Start): el parametro ClusterSrv1Port no es correcto ("+strAux+")."+ex.Message);
                          bCorrecto=false;
                        }
                  }
                  else
                  {
                    logDebugView.Error("(Global.asax-Application_Start): el parametro ClusterSrv1Port no es valido.");
                    bCorrecto=false;
                  }
                  
                  //Direccion IP y puerto del Nodo 2 del cluster
                  strAux=config.AppSettings.Settings["ClusterSrv2Ip"].Value;
                  if (false==bDireccionIPValida(strAux))
                  {
                    logDebugView.Error("(Global.asax-Application_Start): la dirección IP configurada en la la clave ClusterSrv2Ip ("+strAux+") no es válida.");
                    bCorrecto=false;
                  }
                  else
                    strEpNodo2Ip=strAux;
                     
                  strAux=config.AppSettings.Settings["ClusterSrv2Port"].Value;
                  if (!string.IsNullOrWhiteSpace(strAux))
                  {
                        try
                        {
                           iEpNodo2Port=Int32.Parse(strAux);
                        }
                        catch (Exception ex)
                        {
                          logDebugView.Error("(Global.asax-Application_Start): el parametro ClusterSrv2Port no es correcto ("+strAux+")."+ex.Message);
                          bCorrecto=false;
                        }
                  }
                  else
                  {
                    logDebugView.Error("(Global.asax-Application_Start): el parametro ClusterSrv2Port no es valido.");
                    bCorrecto=false;
                  }
                
                  //Se lee el tiempo de reconexión a los servicios de cluster
                  if (null != config.AppSettings.Settings["TimeReconexServCluster"] && (!string.IsNullOrWhiteSpace(config.AppSettings.Settings["TimeReconexServCluster"].Value) ))
                  {
                      //Se establece el tiempo de reconexión a con el servicio de cluster
                      try
                      {
                          iTiempoReconexion = Int32.Parse(config.AppSettings.Settings["TimeReconexServCluster"].Value);

                          if (iTiempoReconexion < 10000)
                          {
                              iTiempoReconexion = 10000;
                              logDebugView.Error("El parámetro de configuracion TimeReconexServCluster debe tomar un valor mayor o igual a 10 s. Por defecto, se toma el valor " + iTiempoReconexion + " ms");
                          }
                      }
                      catch (Exception ex)
                      {
                          iTiempoReconexion = TIEMPO_RECONEXION_SOCKET; //Por defecto, se configura a 60 segundos
                          logDebugView.Error("El parámetro de configuracion TimeReconexServCluster en milisegundos no es valido (" + iTiempoReconexion + "). Error: " + ex.Message.ToString());
                      }
                  }
                  else
                  {
                      iTiempoReconexion = TIEMPO_RECONEXION_SOCKET; //Se configura con 60 segundos.
                  }

                  if (bCorrecto)
                  {
                    StringBuilder sbMsg = new StringBuilder();
                    sbMsg.AppendFormat("Parametros configurados: \n \t\t\t\t TimeRefresStateCluster={0} ms \n \t\t\t\t ListenIp={1}  ListenPort={2} \n \t\t\t\t ", iTiempoRefresco, strLocalIp, ilocalPort);
                    sbMsg.AppendFormat("ClusterSrv1Ip={0}  ClusterSrv1Port={1}\n \t\t \t\t ClusterSrv2Ip={2}  ClusterSrv2Port={3}\n \t\t \t\t TimeReconexServCluster={4} ms", strEpNodo1Ip, iEpNodo1Port, strEpNodo2Ip, iEpNodo2Port, iTiempoReconexion);
                    logDebugView.Info(sbMsg.ToString());
                    sbMsg.Clear();
                  }

               }
        } //config!=null
        else
            bCorrecto=false;
        
        return bCorrecto;
      }

    bool bErrorConexionSocket(System.Net.Sockets.SocketError eSocketError)
    {
        bool bError = false;
        
        switch (eSocketError)
        {
            case System.Net.Sockets.SocketError.AddressNotAvailable:
            case System.Net.Sockets.SocketError.NotConnected:
            case System.Net.Sockets.SocketError.Shutdown:
            case System.Net.Sockets.SocketError.TooManyOpenSockets:
            case System.Net.Sockets.SocketError.NotSocket:
            case System.Net.Sockets.SocketError.SocketError:
            case System.Net.Sockets.SocketError.NetworkDown:
            case System.Net.Sockets.SocketError.ConnectionAborted:
            case System.Net.Sockets.SocketError.SystemNotReady:
                bError = true;
                break;
            default:
                break;
        }

        return bError;
    }

</script>
