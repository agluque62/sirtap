<%@ Application Language="C#" %>

<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Net" %>
<%@ Import Namespace="System.Timers" %>
<%@ Import Namespace="System.Configuration" %>
<%@ Import Namespace="System.Web.Configuration" %>
<%@ Import Namespace="System.Data" %>
<%@ Import Namespace="System.Runtime.Serialization.Formatters.Binary" %>

<%@ Import Namespace="ServiciosCD40" %>
<%@ Import Namespace="log4net" %>
<%@ Import Namespace="log4net.Config" %>
<%@ Import Namespace="UtilitiesCD40" %>
<%@ Import Namespace="ClusterLib" %>

<script RunAt="server">

    static UdpSocket _Comm;
    static IPEndPoint _EndPoint1;
    static IPEndPoint _EndPoint2;
    static System.Timers.Timer _PeriodicTasks;
    static System.Timers.Timer _PeriodicTasksTick4Idle;
 
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


    public enum Sesion_Activa { NO_INICIADA = 0, INICIADA };
    public enum Sesion_Estado { FINALIZADA = 0, ACTIVA, SUPERVISADA, CADUCADA, DESCONOCIDA };


    static SessionsControl Sesiones { get; set; }
    static int MaxSessions = 2;
    static HttpTickHandler TickHandler;

    void Application_OnStart(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>("Application_OnStart");

        log4net.Config.XmlConfigurator.Configure();
        string strAux = string.Empty;
        _Disposed = true; //Indica que el objeto _Comm es null y está liberado
        _Comm = null;
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

            if (null != config.AppSettings.Settings["Sistema"])
                sistema = config.AppSettings.Settings["Sistema"].Value;

            idioma = System.Globalization.CultureInfo.InstalledUICulture.TwoLetterISOLanguageName;

            //Si se configura en modo cluster, la entrada Servidor-2 debe tomar el valor SI
            if (null != config.AppSettings.Settings["Servidor-2"] && string.Compare(config.AppSettings.Settings["Servidor-2"].Value, "SI", true) == 0)
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
            
            // Se lanza la gestión de sesiones de operador
            ControlSession_Start(sender, e);
            
            // Se lanza el control de sesiones de operador
            // El timer se inicia a  segundos
            _PeriodicTasksTick4Idle = new System.Timers.Timer(15000);
            _PeriodicTasksTick4Idle.AutoReset = true;
            _PeriodicTasksTick4Idle.Elapsed += PeriodicTasksTick4Idle;
            _PeriodicTasksTick4Idle.Enabled = true;
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Global.asax-Application_Start): Se ha producido un error ", ex);
        }
    }
    void ControlSession_Start(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>("Application_Start");
        Sesiones = new SessionsControl(MaxSessions);
        Sesiones.InitializeVar();
        Application["SessionsControl"] = Sesiones;
        TickHandler = new HttpTickHandler();
    }

    void Application_OnEnd(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>("Application_OnEnd");
        _Disposed = true;

        if (null != _PeriodicTasks)
        {
            _PeriodicTasks.Enabled = false;
            _PeriodicTasks.Close();
            _PeriodicTasks = null;
        }
        if (null != _PeriodicTasks)
        {
            _PeriodicTasksTick4Idle.Enabled = false;
            _PeriodicTasksTick4Idle.Close();
            _PeriodicTasksTick4Idle = null;
        }

        if (null != _Comm)
        {
            _Comm.Dispose();
            _Comm = null;
        }
        logDebugView.Info("(Global.asax-Application_OnEnd):    ---- PARADA DE LA APLICACION ----\n");
    }
    void Application_End(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>("Application_End");
    }

    void Session_Start(object sender, EventArgs e)
    {
         NLogLogger.Trace<Object>(String.Format("{0}, {1}", Request.RawUrl, Session.SessionID));
    }
    void Session_End(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>(String.Format("{0}, {1}", "", Session.SessionID));
    }

    void Application_Disposed(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>("Application_Disposed");

    }
    void Application_Error(object sender, EventArgs e)
    {
        // Code that runs when an unhandled error occurs
        NLogLogger.Trace<Object>("Application_Error");
    }

    void Application_BeginRequest(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>(String.Format("{0} {1} {2}", Request.HttpMethod, Request.Url, Request.IsAuthenticated));
    }
    void Application_AuthenticateRequest(object sender, EventArgs e)
    {
        // This method is called just before authentication is performed. This is time and place where we can write our own authentication codes.
       NLogLogger.Trace<Object>(String.Format("{0} {1} {2}", Request.HttpMethod, Request.Url, Request.IsAuthenticated));
    }
    
    void Application_AuthorizeRequest(object sender, EventArgs e)
    {
        // After the user is authenticated (identified), it's time to determine the user's permissions. Here we can assign user with special privileges
        NLogLogger.Trace<Object>(String.Format("{0} {1} {2}", Request.HttpMethod, Request.Url, Request.IsAuthenticated));
        if (Request.IsAuthenticated && Request.Url.ToString().Contains("tick") == true || Request.IsAuthenticated && Request.Url.ToString().Contains("PingSrv") == true)
        {
            HttpContext contextTick = HttpContext.Current;
            TickHandler.ProcessRequest(contextTick);
        }
        else
        {
            Sesiones.Renew(Request, Response);
        }
    }
    
    void Application_ResolveRequestCache(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>("Application_ResolveRequestCache");

    }
    
    void Application_AcquireRequestState(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>("Application_RequestState");

    }
    
    void Application_PreRequestHandlerExecute(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>(String.Format("Application_PreRequestHandleExecute => {0}", Request.Url));

    }
    
    void Application_PostRequestHandlerExecute(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>("Application_PostRequestHandlerExecute");

    }
    
    void Application_ReleaseRequestState(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>("Application_ReleaseRequestState");

    }
    
    void Application_UpdateRequestCache(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>("Application_UpdateRequestCache");

    }
    
    void Application_EndRequest(object sender, EventArgs e)
    {
        NLogLogger.Trace<Object>(String.Format("{0} {1} {2}", Request.HttpMethod, Request.Url, Request.IsAuthenticated));
    }

    bool GetDataReplicationState(DataReplicacionState estadoNode1, DataReplicacionState estadoNode2)
    {
        //Se comprueba si se han obtenido datos de la replicación, antes de realizar la comparación.
        //Para que devuelta el estado correcto, en el caso de no estar configurada
        bool bEstadoReplicacionDatos = false;
        //
        //       if (null != estadoNode1.Master.File && estadoNode1.Master.File == estadoNode2.Slave.Master_Log_File &&
        //           null !=estadoNode1.Master.Position && estadoNode1.Master.Position == estadoNode2.Slave.Read_Master_Log_Pos &&
        //           estadoNode2.Master.File == estadoNode1.Slave.Master_Log_File &&
        //           estadoNode2.Master.Position == estadoNode1.Slave.Read_Master_Log_Pos &&
        // 20210419 #4784 
        // No se implementa el control de posiciones, ante una actualización masiva de incidencias, es posible que en un determinado
        // momento Master y Slave no coincidan en el tiempo, se produzca error aunque replicación funcione correctamante.

        if (null != estadoNode1.Master.File &&
            null != estadoNode1.Master.Position &&
            null != estadoNode2.Master.File &&
            null != estadoNode2.Master.Position &&
                Convert.ToInt32(estadoNode1.Slave.Last_Errno) == 0 &&
                Convert.ToInt32(estadoNode1.Slave.Last_IO_Errno) == 0 &&
                Convert.ToInt32(estadoNode1.Slave.Last_SQL_Errno) == 0 &&
                Convert.ToInt32(estadoNode2.Slave.Last_Errno) == 0 &&
                Convert.ToInt32(estadoNode2.Slave.Last_IO_Errno) == 0 &&
                Convert.ToInt32(estadoNode2.Slave.Last_SQL_Errno) == 0)
        {
            bEstadoReplicacionDatos = true;
        }

        return bEstadoReplicacionDatos;
    }

    void OnNewData(object sender, DataGram dg)
    {
        bool bAppLock = false;

        try
        {
            BinaryFormatter bf = new BinaryFormatter();
            MemoryStream ms = new MemoryStream(dg.Data);
            ClusterState state = bf.Deserialize(ms) as ClusterState;

            if (state != null)
            {
                Application.Lock();
                bAppLock = true;

                if (dg.Client.ToString() == _EndPoint1.ToString())
                {
                    //Si se recibe información del nodo 1
                    Application["ClusterWeb_Node1"] = state.LocalNode;
                    Application["ClusterWeb_Node2"] = state.RemoteNode;
                    Application["Data_Replication_Node1"] = state.DataReplication;

                    //Se elimina la actualización de la tabla estadocluster
                    //Ya que el estado no empieza a actualizarse hasta que no se accede por primera vez a la aplicación.
                    //ForClusterState.SetEstadoNodes(state.LocalNode.Name, (int)state.LocalNode.State, state.RemoteNode.Name, (int)state.RemoteNode.State, idioma, sistema, localIp);
                    bool dataReplication = GetDataReplicationState((ClusterLib.DataReplicacionState)Application["Data_Replication_Node1"], (ClusterLib.DataReplicacionState)Application["Data_Replication_Node2"]);
                    Application["Data_Replication_State"] = dataReplication;
                    logDebugView.Debug(String.Format("(Global.asax-OnNewData): Recepcion nodo 1 --> EstadoNodoLocal= {0} EstadoNodoRemoto= {1}", state.LocalNode.State, state.RemoteNode.State));
                    _LastReceivedNodo1 = DateTime.Now;
                }
                else
                {
                    //Si se recibe información del nodo 2
                    Application["ClusterWeb_Node1"] = state.RemoteNode;
                    Application["ClusterWeb_Node2"] = state.LocalNode;
                    Application["Data_Replication_Node2"] = state.DataReplication;

                    //Se elimina la actualización de la tabla estadocluster.
                    //ForClusterState.SetEstadoNodes(state.RemoteNode.Name, (int)state.RemoteNode.State, state.LocalNode.Name, (int)state.LocalNode.State, idioma, sistema, localIp);
                    bool dataReplication = GetDataReplicationState((ClusterLib.DataReplicacionState)Application["Data_Replication_Node1"], (ClusterLib.DataReplicacionState)Application["Data_Replication_Node2"]);
                    Application["Data_Replication_State"] = dataReplication;

                    logDebugView.Debug(String.Format("(Global.asax-OnNewData): Recepcion nodo 2 --> EstadoNodoLocal= {0} EstadoNodoRemoto= {1}", state.LocalNode.State, state.RemoteNode.State));
                    _LastReceivedNodo2 = DateTime.Now;
                }

                Application.UnLock();
                bAppLock = false;
            }
        }
        catch (Exception ex)
        {

            logDebugView.Error("(Global.asax-OnNewData) Error on data received from " + dg.Client, ex);
            if (bAppLock)
            {
                // Si previamente, se ha bloqueado la aplicación, se elimina el bloqueo
                Application.UnLock();
                bAppLock = false;
            }
        }
    }

    void PeriodicTasks(object sender, ElapsedEventArgs e)
    {
        bool bAppLock = false;
        bool bActualizarEstadoRep = false;
        StringBuilder strMsgError = new StringBuilder();
        
        try
        {
            if (false == bConexionSocket)
            {
                try
                {
                    if (null != _Comm)
                    {
                        //Se libera el objeto, antes de volver a crearlo
                        _Comm.Dispose();
                        _Comm = null;
                        _Disposed = true;

                        logDebugView.Error("(Global.asax-PeriodicTasks): Se ha perdido la conexion con el socket.");
                        //Se limpian las estructuras
                        Application.Lock();
                        bAppLock = true;

                        //Se  resetean las estructuras.
                        //Para que se actualice el estado
                        Application["ClusterWeb_Node1"] = new NodeInfo();
                        Application["Data_Replication_Node1"] = new DataReplicacionState();
                        Application["ClusterWeb_Node2"] = new NodeInfo();
                        Application["Data_Replication_Node2"] = new DataReplicacionState();
                        Application["Data_Replication_State"] = false;
                        Application.UnLock();
                        bAppLock = false;
                    }

                    _Comm = new UdpSocket(strLocalIp, ilocalPort);

                    if (null != _Comm)
                    {
                        _Disposed = false;
                    }
                }
                catch (System.Net.Sockets.SocketException exp)
                {
                    logDebugView.Info("(Global.asax-PeriodicTasks): No se ha conseguido establecer la conexión a la dirección " + strLocalIp + ":" + ilocalPort + " (ListenIp:ListenPort)");
                    strMsgError.AppendFormat("(Global.asax-PeriodicTasks): Error al crear el socket --> {0}({1}): {2}", exp.SocketErrorCode, exp.ErrorCode, exp.Message.ToString());
                    logDebugView.Error(strMsgError.ToString());
                }

                if (null != _Comm && false == _Disposed)
                {
                    _Comm.NewDataEvent += OnNewData;

                    // Se establece el  endpoint remoto con el nodo1  con el nodo1 para el socket
                    if (null == _EndPoint1)
                    {
                        _EndPoint1 = new IPEndPoint(IPAddress.Parse(strEpNodo1Ip), iEpNodo1Port);
                    }

                    // Se establece el  endpoint remoto con el nodo2  para el socket
                    if (null == _EndPoint2)
                    {
                        _EndPoint2 = new IPEndPoint(IPAddress.Parse(strEpNodo2Ip), iEpNodo2Port);
                    }

                    //ForClusterState = new ServiciosCD40();

                    MsgType type = MsgType.Activate;
                    MemoryStream ms = new MemoryStream();
                    BinaryFormatter bf = new BinaryFormatter();
                    bf.Serialize(ms, type);
                    _ActivateMsg = ms.ToArray();

                    type = MsgType.GetState;
                    ms.Position = 0;
                    bf.Serialize(ms, type);
                    _AskInfoMsg = ms.ToArray();

                    Application["ClusterWeb_Comm"] = _Comm;
                    Application["ClusterWeb_EP1"] = _EndPoint1;
                    Application["ClusterWeb_EP2"] = _EndPoint2;
                    Application["ClusterWeb_ActivateMsg"] = _ActivateMsg;
                    Application["ClusterWeb_Node1"] = new NodeInfo();
                    Application["ClusterWeb_Node2"] = new NodeInfo();
                    Application["Data_Replication_Node1"] = new DataReplicacionState();
                    Application["Data_Replication_Node2"] = new DataReplicacionState();
                    Application["Data_Replication_State"] = false;

                    //Se conecta con el nodo remoto
                    _Comm.BeginReceive();
                    bConexionSocket = true;
                    logDebugView.Info("(Global.asax-PeriodicTasks): Conexión establecida a " + strLocalIp + ":" + ilocalPort);
                }

            }
            else
            {
                Application.Lock();
                bAppLock = true;

                if ((DateTime.Now - _LastReceivedNodo1).TotalMilliseconds > 5000)
                {
                    //Si no se ha recibido información del nodo 1, en un tiempo superior a 5 segundos, se resetean las estructuras.
                    //Para que se actualice el estado
                    Application["ClusterWeb_Node1"] = new NodeInfo();
                    Application["Data_Replication_Node1"] = new DataReplicacionState();
                    bActualizarEstadoRep = true;
                }

                if ((DateTime.Now - _LastReceivedNodo2).TotalMilliseconds > 5000)
                {
                    //Si no se ha recibido información del nodo 2, en un tiempo superior a 5 segundos, se resetean las estructuras.
                    //Para que se actualice el estado del nodo 2
                    Application["ClusterWeb_Node2"] = new NodeInfo();
                    Application["Data_Replication_Node2"] = new DataReplicacionState();
                    bActualizarEstadoRep = true;
                }

                if (bActualizarEstadoRep)
                    Application["Data_Replication_State"] = false;

                Application.UnLock();
                bAppLock = false;

                try
                {
                    _Comm.Send(_EndPoint1, _AskInfoMsg);
                }
                catch (System.Net.Sockets.SocketException objSocketEx)
                {
                    strMsgError.AppendFormat("(Global.asax-PeriodicTasks): Error al solicitar informacion del Estado del Nodo 1 (_Comm.Send {0})--> {1} ({2}): {3}", (_EndPoint1 != null) ? _EndPoint1.ToString() : "null", objSocketEx.SocketErrorCode, objSocketEx.ErrorCode, objSocketEx.Message.ToString());
                    logDebugView.Error(strMsgError.ToString());

                    if (bErrorConexionSocket(objSocketEx.SocketErrorCode))
                        bConexionSocket = false;
                }

                try
                {
                    _Comm.Send(_EndPoint2, _AskInfoMsg);
                }
                catch (System.Net.Sockets.SocketException objSocketEx)
                {
                    if (strMsgError.Length > 0)
                        strMsgError.Clear();

                    strMsgError.AppendFormat("(Global.asax-PeriodicTasks): Error al solicitar informacion del Estado del Nodo 2 (_Comm.Send {0})--> {1} ({2}): {3}", (_EndPoint2 != null) ? _EndPoint2.ToString() : "null", objSocketEx.SocketErrorCode, objSocketEx.ErrorCode, objSocketEx.Message.ToString());
                    logDebugView.Error(strMsgError.ToString());
                    if (bErrorConexionSocket(objSocketEx.SocketErrorCode))
                        bConexionSocket = false;
                }
            }
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Global.asax-PeriodicTasks) Error en la funcion PeriodicTasks:", ex);

            if (bAppLock)
            {
                //Si previamente, se ha producido un bloqueo, se desbloquea
                Application.UnLock();
                bAppLock = false;
            }
        }
        finally
        {

            //Si no está conectado, se establece el timer de reconexión
            if (!bConexionSocket)
            {
                if (iTiempoReconexion != _PeriodicTasks.Interval)
                    _PeriodicTasks.Interval = iTiempoReconexion;
            }
            else
                if (iTiempoRefresco != _PeriodicTasks.Interval)
                _PeriodicTasks.Interval = iTiempoRefresco;

            _PeriodicTasks.Enabled = true;


            if (strMsgError.Length > 0)
                strMsgError.Clear();
        }
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
                logDebugView.Error("Direccion IP no valida (ArgumentNullException) -> " + e.Message);
            }

            catch (FormatException e)
            {
                logDebugView.Error("Direccion IP no es valida (Error de formato)-> " + e.Message);
            }
            catch (Exception e)
            {
                logDebugView.Error("La Direccion IP no es valida (excepcion) -> " + e.Message);
            }
        }

        return bCorrecto;

    }

    //Lee los parámetros del fichero Web.config, asociados con la monitorización del estado del cluster
    bool bLeerParametrosConfiguracionCluster(ref Configuration config)
    {
        bool bCorrecto = true;
        string strAux = string.Empty;
        const int TIEMPO_RECONEXION_SOCKET = 60000; //Se establece el tiempo de reconexión por defecto a 1 minuto 

        if (null != config)
        {
            //Se comprueba la dirección IP y el puerto local configurado
            if (null == config.AppSettings.Settings["ListenIp"] || null == config.AppSettings.Settings["ListenPort"])
            {
                logDebugView.Error("(Global.asax-Application_Start): No se han encontrado las entradas ListenIp y/o ListenPort en el fichero de configuración.");
                bCorrecto = false;
            }

            if (null == config.AppSettings.Settings["ClusterSrv1Ip"] || null == config.AppSettings.Settings["ClusterSrv1Port"])
            {
                logDebugView.Error("(Global.asax-Application_Start): No se han encontrado las entradas ClusterSrv1Ip y/o ClusterSrv1Port con la dirección IP y el puerto del Nodo 1.");
                bCorrecto = false;
            }

            if (null == config.AppSettings.Settings["ClusterSrv2Ip"] || null == config.AppSettings.Settings["ClusterSrv2Port"])
            {
                logDebugView.Error("(Global.asax-Application_Start): No se han encontrado las entradas ClusterSrv2Ip y/o ClusterSrv2Port con la dirección IP y el puerto del Nodo 2.");
                bCorrecto = false;
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
                        iTiempoRefresco = 3000; //Se configura con 3 segundos.
                        logDebugView.Error("El parámetro de configuracion TimeRefresStateCluster en milisegundos no tiene un valor correcto. Por defecto, se toma el valor " + iTiempoRefresco + " ms. Error:" + ex.Message.ToString());
                    }
                }
                else
                {
                    iTiempoRefresco = 3000; //Se configura con 3 segundos.
                    logDebugView.Error("(Global.asax-Application_Start): No se han encontrado la entrada TimeRefresStateCluster en el fichero de configuracion  Web.config. Por defecto, se configura a 3000 ms.");
                }

                //Se comprueban que las direcciones IP configuradas tienen una IP válida
                //Direccion IP y puerto local
                strAux = config.AppSettings.Settings["ListenIp"].Value;

                if (false == bDireccionIPValida(strAux))
                {
                    logDebugView.Error("(Global.asax-Application_Start): la dirección IP configurada en la la clave ListenIp (" + strAux + ") no es válida.");
                    bCorrecto = false;
                }
                else
                {
                    strLocalIp = strAux;
                }

                //Puerto local
                strAux = config.AppSettings.Settings["ListenPort"].Value;
                if (!string.IsNullOrWhiteSpace(strAux))
                {
                    try
                    {
                        ilocalPort = Int32.Parse(strAux);
                    }
                    catch (Exception ex)
                    {
                        logDebugView.Error("(Global.asax-Application_Start): el parametro ListenPort no es correcto (" + strAux + ")." + ex.Message);
                        bCorrecto = false;
                    }
                }
                else
                {
                    logDebugView.Error("(Global.asax-Application_Start): el parametro ListenPort no es valido.");
                    bCorrecto = false;
                }

                //Direccion IP y puerto del Nodo 1 del cluster
                strAux = config.AppSettings.Settings["ClusterSrv1Ip"].Value;
                if (false == bDireccionIPValida(strAux))
                {
                    logDebugView.Error("(Global.asax-Application_Start): la dirección IP configurada en la la clave ClusterSrv1Ip (" + strAux + ") no es válida.");
                    bCorrecto = false;
                }
                else
                {
                    strEpNodo1Ip = strAux;
                }

                //Puerto 
                strAux = config.AppSettings.Settings["ClusterSrv1Port"].Value;
                if (!string.IsNullOrWhiteSpace(strAux))
                {
                    try
                    {
                        iEpNodo1Port = Int32.Parse(strAux);
                    }
                    catch (Exception ex)
                    {
                        logDebugView.Error("(Global.asax-Application_Start): el parametro ClusterSrv1Port no es correcto (" + strAux + ")." + ex.Message);
                        bCorrecto = false;
                    }
                }
                else
                {
                    logDebugView.Error("(Global.asax-Application_Start): el parametro ClusterSrv1Port no es valido.");
                    bCorrecto = false;
                }

                //Direccion IP y puerto del Nodo 2 del cluster
                strAux = config.AppSettings.Settings["ClusterSrv2Ip"].Value;
                if (false == bDireccionIPValida(strAux))
                {
                    logDebugView.Error("(Global.asax-Application_Start): la dirección IP configurada en la la clave ClusterSrv2Ip (" + strAux + ") no es válida.");
                    bCorrecto = false;
                }
                else
                    strEpNodo2Ip = strAux;

                strAux = config.AppSettings.Settings["ClusterSrv2Port"].Value;
                if (!string.IsNullOrWhiteSpace(strAux))
                {
                    try
                    {
                        iEpNodo2Port = Int32.Parse(strAux);
                    }
                    catch (Exception ex)
                    {
                        logDebugView.Error("(Global.asax-Application_Start): el parametro ClusterSrv2Port no es correcto (" + strAux + ")." + ex.Message);
                        bCorrecto = false;
                    }
                }
                else
                {
                    logDebugView.Error("(Global.asax-Application_Start): el parametro ClusterSrv2Port no es valido.");
                    bCorrecto = false;
                }

                //Se lee el tiempo de reconexión a los servicios de cluster
                if (null != config.AppSettings.Settings["TimeReconexServCluster"] && (!string.IsNullOrWhiteSpace(config.AppSettings.Settings["TimeReconexServCluster"].Value)))
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
            bCorrecto = false;

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
    
    // Control de sesiones de operadores
    void PeriodicTasksTick4Idle(object sender, ElapsedEventArgs e)
    {
        Sesiones.Tick4Idle();
        _PeriodicTasksTick4Idle.Enabled = true;
    }

</script>
