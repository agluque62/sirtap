using System;
using System.IO;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Diagnostics;
using System.Net;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Web.Configuration;
using System.Collections.Generic;
using log4net;
using log4net.Config;
using System.Runtime.InteropServices;


public partial class ConfiguracionesOffline : PageBaseCD40.PageCD40
{
    private static ServiciosCD40.Tablas[] datosConfOffline;
    private static ServiciosCD40.Tablas[] datosLogProcesos;

    private enum Accion { Nueva, Importar, Editar, Eliminar, Exportar, Copiar, Activar, Replicar };
    private string literalVersion = "-- Ulises V5000i - CFG V";

    [DllImport("user32.dll")]
    static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

    private static int maxConfOffline;
    private static string dbUser, dbPassword;
    private static string IPLocal, IPRemoto;

    // Control de procesos .bat
    private static string accionProceso, scriptProceso, argProceso, descProceso, fichProceso;  // Para cuando la ejecución de un proceso necesite confirmación: "EjecutarProceso"
    private static int refrescoLogProcesos;


    private Mensajes.msgBox cMsg;
    private static ILog _logDebugView;
    public static ILog logDebugView
    {
        get
        {
            if (_logDebugView == null)
            {
                log4net.Config.XmlConfigurator.Configure();
                _logDebugView = LogManager.GetLogger("CONFIGURACION");
            }
            return _logDebugView;
        }
    }

    static ServiciosCD40.ServiciosCD40 Servicio = new ServiciosCD40.ServiciosCD40();

    protected void Page_Load(object sender, EventArgs e)
    {
        base.Page_Load(sender, e);
        
        cMsg = (Mensajes.msgBox)this.Master.FindControl("MsgBox1");
        if (Context.Request.IsAuthenticated)
        {
            // retrieve user's identity from httpcontext user 
            FormsIdentity ident = (FormsIdentity)Context.User.Identity;
            string perfil = ident.Ticket.UserData;
            if (perfil != "3")
            {
                Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO", false);
                return;
            }
        }
        if (!IsPostBack)
        {
            //Se lee la variable de sesión idsistema
            Configuration webConfig = WebConfigurationManager.OpenWebConfiguration("~");

            Application["CFGVersion"] = webConfig.AppSettings.Settings["CFGVersion"].Value;

            if (null != webConfig.AppSettings.Settings["Servidor-2"] && string.Compare(webConfig.AppSettings.Settings["Servidor-2"].Value, "SI", true) == 0)
            {
                logDebugView.Info("(ConfiguracionesOffline-Page_Load): Sistema configurado en modo Cluster");
                Application["Cnf_ModoCluster"] = true;
                Application["Servidor1"] = webConfig.AppSettings.Settings["ClusterSrv1Ip"].Value;
                Application["Servidor2"] = webConfig.AppSettings.Settings["ClusterSrv2Ip"].Value;
            }
            else
            {
                logDebugView.Info("(ConfiguracionesOffline-Application_Start): Sistema configurado en modo Servidor");
                Application["Cnf_ModoCluster"] = false;
                Application["Servidor1"] = webConfig.AppSettings.Settings["ClusterSrv1Ip"].Value;
                Application["Servidor2"] = string.Empty;
            }

            IPLocal = IPRemoto = "";
            string HostNameLocal = Dns.GetHostName();
            IPHostEntry ip = Dns.GetHostEntry(HostNameLocal);
            IPAddress[] addr = ip.AddressList;
            for (int i = 0; i < addr.Length; i++)
            {
                if (Application["Servidor1"].ToString() == addr[i].ToString())
                {
                    IPLocal = Application["Servidor1"].ToString();
                    break;
                }
            }

            if (string.IsNullOrEmpty(IPLocal))
            {
                for (int i = 0; i < addr.Length; i++)
                {
                    if (Application["Servidor2"].ToString() == addr[i].ToString())
                    {
                        IPLocal = Application["Servidor2"].ToString();
                        break;
                    }
                }
            }

            if (string.IsNullOrEmpty(IPLocal))
            {
                logDebugView.Error("(ConfiguracionesOffline-Page_Load): Imposible identificar la IP del servidor Local entre los servidores configurados. 1:" + Application["Servidor1"].ToString() + " 2:" + Application["Servidor2"].ToString());
            }
            else
            {
                if (IPLocal == Application["Servidor1"].ToString())
                    IPRemoto = Application["Servidor2"].ToString();
                else
                    IPRemoto = Application["Servidor1"].ToString();
            }

            Application["idsistema"] = webConfig.AppSettings.Settings["Sistema"].Value;
            dbUser = webConfig.AppSettings.Settings["DBAccesU"].Value;
            dbPassword = webConfig.AppSettings.Settings["DBAccesP"].Value;

            Application["ConfOfflineEditarURL"] = webConfig.AppSettings.Settings["ConfOfflineEditarURL"].Value;
            Application["ConfOfflineEditarWebConfig"] = webConfig.AppSettings.Settings["ConfOfflineEditarWebConfig"].Value;
            Application["ConfOfflineEditarWebConfig_BD"] = webConfig.AppSettings.Settings["ConfOfflineEditarWebConfig_BD"].Value;
            Application["ConfOfflineEditarWebConfig_BD_Trans"] = webConfig.AppSettings.Settings["ConfOfflineEditarWebConfig_BD_Trans"].Value;

            Application["PathConfOffline"] = webConfig.AppSettings.Settings["PathConfOffline"].Value;
            Application["PathScripts"] = webConfig.AppSettings.Settings["PathScripts"].Value;
            Application["ScriptBorrar"] = webConfig.AppSettings.Settings["ScriptBorrar"].Value;
            Application["ScriptCopiar"] = webConfig.AppSettings.Settings["ScriptCopiar"].Value;
            Application["ScriptExportar"] = webConfig.AppSettings.Settings["ScriptExportar"].Value;
            Application["ScriptImportar"] = webConfig.AppSettings.Settings["ScriptImportar"].Value;
            Application["ScriptNueva"] = webConfig.AppSettings.Settings["ScriptNueva"].Value;
            Application["ScriptActivar"] = webConfig.AppSettings.Settings["ScriptActivar"].Value;
            Application["ScriptReplicar"] = webConfig.AppSettings.Settings["ScriptReplicar"].Value;
            Application["IncidenciaConfiguracionesOffline"] = webConfig.AppSettings.Settings["IncidenciaConfiguracionesOffline"].Value;

            if (!Int32.TryParse(webConfig.AppSettings.Settings["RefrecoLogProceso"].Value, out refrescoLogProcesos))
                refrescoLogProcesos = 5000; // Por defecto 5 segundos

            timerRefrescoLogProcesos.Interval = refrescoLogProcesos;

            if (!Int32.TryParse(webConfig.AppSettings.Settings["MaxConfiguracionesOffline"].Value, out maxConfOffline))
                maxConfOffline = 32;

            MuestraDatos();
        }
        else
        {
            if (datosConfOffline == null)
                DameDatosConfOffline();
            if (datosLogProcesos == null)
                DameDatosLogProcesos();

            if (!string.IsNullOrEmpty(accionProceso))
            {
                if (scriptProceso == "" || argProceso == "" || descProceso == "")
                    cMsg.alert("No se ha definido el proceso a ejecutar");

                switch ((Accion)Enum.Parse(typeof(Accion), accionProceso, true))
                {
                    case Accion.Exportar:
                        if (Request.Form["EjecutarProceso"] == "1")   //El usuario elige continuar con el export
                        {
                            Request.Form["EjecutarProceso"].Replace("1", "0");
                            LanzarScript(scriptProceso, argProceso, descProceso);
                        }
                        break;
                    case Accion.Replicar:
                        if (Request.Form["EjecutarProceso"] == "1")   //El usuario elige continuar con el export
                        {
                            Request.Form["EjecutarProceso"].Replace("1", "0");
                            LanzarScript(scriptProceso, argProceso, descProceso);
                        }
                        else
                        {
                            logDebugView.Debug("(ConfiguracionesOffline-Page_Load): EjecutarProceso: " + accionProceso + " sin borrar los esquemas en el servidor remoto");
                            argProceso = argProceso + " 0";  // Último parámetro del script Borrar=0. El script por defecto borra los esquemas en el otro servidor
                            LanzarScript(scriptProceso, argProceso, descProceso);
                            //MuestraLogProcesos(DameDatosLogProcesos());
                        }
                        break;
                    default:
                        break;
                }

                accionProceso = "";
                scriptProceso = "";
                argProceso = "";
                descProceso = "";
                fichProceso = "";
                MuestraDatos();
            }

            if (timerRefrescoLogProcesos.Enabled)
            {
                MuestraLogProcesos(DameDatosLogProcesos());
            }
        }

    }

    private void DameDatos()
    { 
        DameDatosConfOffline();
        DameDatosLogProcesos();
    }

    private ServiciosCD40.Tablas[] DameDatosConfOffline()
    {
        try
        {
            ServiciosCD40.Configuraciones t = new ServiciosCD40.Configuraciones();
            ServiciosCD40.Tablas[] d = Servicio.ListSelectSQL(t);
            datosConfOffline = d;
            return d;
        }
        catch (Exception e)
        {
            logDebugView.Error("(ConfiguracionesOffline-DameDatosConfOffline):", e);
        }

        return null;
    }

    private ServiciosCD40.Tablas[] DameDatosLogProcesos()
    {
        try
        {
            ServiciosCD40.LogProcesos t = new ServiciosCD40.LogProcesos();
            ServiciosCD40.Tablas[] d = Servicio.ListSelectSQL(t);
            datosLogProcesos = d;
            return d;
        }
        catch (Exception e)
        {
            logDebugView.Error("(ConfiguracionesOffline-DameDatosLogProcesos):", e);
        }
    
        return null;
    }

    private void MuestraDatos()
    {
        MuestraConfOffline(DameDatosConfOffline());
        MuestraLogProcesos(DameDatosLogProcesos());
    }

    private void MuestraConfOffline(ServiciosCD40.Tablas[] datos)
    {
        lnkReplicar.Visible = (bool)Application["Cnf_ModoCluster"];

        try
        {
            gvConfiguraciones.Columns[1].Visible = true;
            gvConfiguraciones.Columns[2].Visible = true;

            if (datos != null)
            {
                gvConfiguraciones.DataSource = datos;
                gvConfiguraciones.DataBind();
            }

            gvConfiguraciones.Columns[1].Visible = false;
            gvConfiguraciones.Columns[2].Visible = false;
        }
        catch (Exception ex)
        {
            logDebugView.Error("(ConfiguracionesOffline-MuestraConfOffline) " + ex.ToString());
            Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO", false);
            return;
        }
    }

    private void MuestraLogProcesos(ServiciosCD40.Tablas[] datos)
    {
        try
        {
            gvLogProcesos.Columns[0].Visible = true;
            gvLogProcesos.Columns[1].Visible = true;
            gvLogProcesos.Columns[2].Visible = true;

            if (datos != null)
            {
                gvLogProcesos.DataSource = datos;
                gvLogProcesos.DataBind();

                // Si hay algún proceso Activo, comprobamos si existe.
                // Si no existe, modificamos los datos  volvemos a cargar la tabla
                bool refrescar, procesoActivo;
                refrescar = false;
                procesoActivo = false;

                foreach (ServiciosCD40.LogProcesos logProceso in datos)
                {
                    if (logProceso.Activo)
                    {
                        if (ExisteProceso(logProceso.PID))
                        {
                            procesoActivo = true;
                        }
                        else
                        {
                            logProceso.Descripcion = null;
                            logProceso.Activo = false;
                            logProceso.IdSistema = Application["IdSistema"].ToString(); // Se le da valor en el proceso de inserción. Las siguientes veces provocan el error "Cannot add or update a child row"
                            int ret = Servicio.UpdateSQL(logProceso);
                            logDebugView.Debug("(ConfiguracionesOffline-MuestraLogProcesos): Servicio.UpdateSQL(LogProceso) => " + ret.ToString() + ". Id: " + logProceso.Id + ", PID: " + logProceso.PID.ToString() + ", IdSistema: " + logProceso.IdSistema + ", Usuario: " + logProceso.Usuario + ", Descripcion: " + logProceso.Descripcion);
                            if (ret <= 0)
                            {
                                logDebugView.Error("(ConfiguracionesOffline-MuestraLogProcesos): Error UpdateSQL(ret:" + ret.ToString() + ") en LogProceso. Quitamos el IdSistema y modificamos el resto de columnas");
                                // Quitamos el IdSistema y modificamos el resto de columnas
                                logProceso.IdSistema = null;
                                ret = Servicio.UpdateSQL(logProceso);
                            }
                            refrescar = true;
                            procesoActivo = false;
                        }
                    }
                }

                if (refrescar || procesoActivo)
                {
                    DameDatosLogProcesos();
                    gvLogProcesos.DataSource = datosLogProcesos;
                    gvLogProcesos.DataBind();
                }

                if (procesoActivo)
                {
                    if (!timerRefrescoLogProcesos.Enabled)
                        timerRefrescoLogProcesos.Enabled = true;
                }
                else if (timerRefrescoLogProcesos.Enabled)
                {
                    // Actualizamos la lista de conf. por si el proceso ha creado una nueva
                    gvConfiguraciones.Columns[1].Visible = true;
                    gvConfiguraciones.Columns[2].Visible = true;

                    if (datos != null)
                    {
                        DameDatosConfOffline();
                        gvConfiguraciones.DataSource = datosConfOffline;
                        gvConfiguraciones.DataBind();
                    }

                    gvConfiguraciones.Columns[1].Visible = false;
                    gvConfiguraciones.Columns[2].Visible = false;

                    timerRefrescoLogProcesos.Enabled = false;
                }

                ProcesosDisponibles(!procesoActivo);
            }

            gvLogProcesos.Columns[0].Visible = false;
            gvLogProcesos.Columns[1].Visible = false;
            gvLogProcesos.Columns[2].Visible = false;

            if (gvLogProcesos.Rows.Count == 0)
            {
                lblProcesando.Visible = false;
            }
            else
            {
                lblProcesando.Visible = true;
                if (timerRefrescoLogProcesos.Enabled)
                {
                    if (lblProcesando.Text == "Log de procesos")
                    {
                        lblProcesando.Text = "Ejecutando proceso...";
                    }
                    else
                    {
                        lblProcesando.Text = lblProcesando.Text + "..";
                    }
                }
                else if (lblProcesando.Text != "Log de procesos")
                    lblProcesando.Text = "Log de procesos";
            }

        }
        catch (Exception ex)
        {
            logDebugView.Error("(ConfiguracionesOffline-MuestraLogProcesos) " + ex.ToString());
            Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO", false);
            return;
        }
    }

    protected void gvConfiguraciones_PageIndexChanging(object sender, GridViewPageEventArgs e)
    {
        gvConfiguraciones.PageIndex = e.NewPageIndex;
        MuestraConfOffline(datosConfOffline);
        ProcesosDisponibles(!timerRefrescoLogProcesos.Enabled);
    }

    protected void Editar_Configuracion(string conf, string conf_ed)
    {
        string newWebConfig = "";
        using (StreamReader sr = new StreamReader(Application["ConfOfflineEditarWebConfig"].ToString()))
        {
            int i = 0;
            do
            {
                i++;
                string linea = sr.ReadLine();
                if (linea.IndexOf("name=\"" + Application["ConfOfflineEditarWebConfig_BD"].ToString() + "\"") >= 0)
                {
                    linea = System.Text.RegularExpressions.Regex.Replace(linea, ";Database=[\\w_]+;", ";Database=" + conf + ";");
                    newWebConfig = newWebConfig + linea + Environment.NewLine;
                }
                else if (linea.IndexOf("name=\"" + Application["ConfOfflineEditarWebConfig_BD_Trans"].ToString() + "\"") >= 0)
                {
                    linea = System.Text.RegularExpressions.Regex.Replace(linea, ";Database=[\\w_]+;", ";Database=" + conf_ed + ";");
                    newWebConfig = newWebConfig + linea + Environment.NewLine;
                }
                else
                {
                    newWebConfig = newWebConfig + linea + Environment.NewLine;
                }
            } while (sr.EndOfStream == false);
        }
        File.WriteAllText(Application["ConfOfflineEditarWebConfig"].ToString(), newWebConfig);

        //Page.ClientScript.RegisterStartupScript(this.GetType(), "OpenWindow", "window.open('" + Application["ConfOfflineEditarURL"].ToString() + "','_blank');", true);
        Page.ClientScript.RegisterStartupScript(this.GetType(), "OpenWindow", "window.open('" + Application["ConfOfflineEditarURL"].ToString() + "','_newtab');", true);
    }

    protected void gvConfiguraciones_RowCommand(object sender, GridViewCommandEventArgs e)
    {
        if (!(e.CommandName == "Page"))
        {
            GridViewRow rowSelect = (GridViewRow)(((LinkButton)e.CommandSource).NamingContainer);
            string conf = rowSelect.Cells[0].Text;
            string conf_ed = rowSelect.Cells[1].Text;
            string operacion, script, argumentos, pathConfOffline;

            operacion = script = argumentos = "";
            pathConfOffline = Application["PathConfOffline"].ToString();

            if (string.IsNullOrEmpty(dbUser) || string.IsNullOrEmpty(dbPassword))
            {
                Configuration webConfig = WebConfigurationManager.OpenWebConfiguration("~");
                dbUser = webConfig.AppSettings.Settings["DBAccesU"].Value;
                dbPassword = webConfig.AppSettings.Settings["DBAccesP"].Value;
            }

            switch ((Accion)Enum.Parse(typeof(Accion), e.CommandName, true))
            {
                case Accion.Editar:
                    Editar_Configuracion(conf, conf_ed);
                    break;
                case Accion.Eliminar:
                    operacion = "Eliminando " + conf;
                    argumentos = dbUser + " " + dbPassword + " " + conf;
                    script = Application["ScriptBorrar"].ToString();
                    break;
                case Accion.Exportar:
                    if (File.Exists(pathConfOffline + "\\" + conf + ".sql"))
                    {
                        accionProceso = e.CommandName;
                        scriptProceso = Application["ScriptExportar"].ToString();
                        argProceso = dbUser + " " + dbPassword + " " + conf + " \"" + literalVersion + Application["CFGVersion"].ToString() + "\" TODO \"" + pathConfOffline + "\\" + conf + ".sql\" LOCALHOST";
                        descProceso = "Exportando " + conf;
                        fichProceso = pathConfOffline + "\\" + conf + ".sql";
                        cMsg.confirm("El fichero " + pathConfOffline.Replace("\\", "\\\\") + "\\\\" + conf + ".sql" + " ya existe. ¿Desesa sobrescribirlo?", "EjecutarProceso");
                    }
                    else
                    {
                        operacion = "Exportando " + conf;
                        argumentos = dbUser + " " + dbPassword + " " + conf + " \"" + literalVersion + Application["CFGVersion"].ToString() + "\" TODO \"" + pathConfOffline + "\\" + conf + ".sql\" LOCALHOST";
                        script = Application["ScriptExportar"].ToString();
                    }
                    break;
                case Accion.Copiar:
                    txtNuevaConfOrigen.Text = conf;
                    MostrarVentanaNuevaConf(Accion.Copiar);
                    break;
                case Accion.Activar:
                    operacion = "Pasando a producción " + conf_ed;
                    argumentos = dbUser + " " + dbPassword + " " + conf_ed + " LOCALHOST ";
                    script = Application["ScriptActivar"].ToString();
                    break;
                default:
                    break;
            }

            if (!string.IsNullOrEmpty(script))
            {
                LanzarScript(script, argumentos, operacion);
            }
        }
        MuestraDatos();
    }

    protected void gvLogProcesos_PageIndexChanging(object sender, GridViewPageEventArgs e)
    {
        gvLogProcesos.PageIndex = e.NewPageIndex;
        MuestraLogProcesos(datosLogProcesos);
    }
    protected void gvLogProcesos_RowCommand(object sender, GridViewCommandEventArgs e)
    {
        if (!(e.CommandName == "Page"))
        {
            GridViewRow rowSelect = (GridViewRow)(((LinkButton)e.CommandSource).NamingContainer);
            ServiciosCD40.LogProcesos l = new ServiciosCD40.LogProcesos();
            string idSistema, desc;
            int ret;

            idSistema = Application["IdSistema"].ToString();
            desc = "";

            if (!String.IsNullOrWhiteSpace(rowSelect.Cells[0].Text))
                l.Id = rowSelect.Cells[0].Text;
            if (!String.IsNullOrWhiteSpace(rowSelect.Cells[1].Text))
                l.PID = Convert.ToInt32(rowSelect.Cells[1].Text);
            if (!String.IsNullOrWhiteSpace(rowSelect.Cells[2].Text))
                l.Activo = bool.Parse(rowSelect.Cells[2].Text);
            if (!String.IsNullOrWhiteSpace(rowSelect.Cells[3].Text))
                l.FechaHora = Convert.ToDateTime(rowSelect.Cells[3].Text);
            if (!String.IsNullOrWhiteSpace(rowSelect.Cells[4].Text))
                desc = rowSelect.Cells[4].Text;

            l.Usuario = ((FormsIdentity)Context.User.Identity).Name;
            l.IdSistema = idSistema; // Se le da valor en el proceso de inserción. Las siguientes veces provocan el error "Cannot add or update a child row"

            switch (e.CommandName)
            {
                case "Leido":
                    l.FechaHoraLeido = DateTime.Now;
                    ret = Servicio.UpdateSQL(l);
                    logDebugView.Debug("(ConfiguracionesOffline-gvLogProcesos_RowCommand): Servicio.UpdateSQL(LogProceso) => " + ret.ToString() + ". Id: " + l.Id + ", PID: " + l.PID.ToString() + ", IdSistema: " + l.IdSistema + ", Usuario: " + l.Usuario + ", Descripcion: " + l.Descripcion);
                    if (ret <= 0)
                    {
                        logDebugView.Error("(ConfiguracionesOffline-gvLogProcesos_RowCommand): Error UpdateSQL(ret:" + ret.ToString() + ") en LogProceso. Quitamos el IdSistema y modificamos el resto de columnas");
                        // Quitamos el IdSistema y modificamos el resto de columnas
                       l.IdSistema = null;
                       ret = Servicio.UpdateSQL(l);
                    }
                    break;
                case "Cancelar":
                    PararProceso(l.PID);
                    l.FechaHoraLeido = DateTime.MinValue;
                    l.Activo = false;
                    if (desc == "")
                        l.Descripcion = "Proceso cancelado por el usuario (" + ((FormsIdentity)Context.User.Identity).Name + ")";
                    else
                        l.Descripcion = desc.Substring(0, desc.IndexOf("...")) + "... Proceso cancelado por el usuario (" + ((FormsIdentity)Context.User.Identity).Name + ")";

                    ret = Servicio.UpdateSQL(l);
                    logDebugView.Debug("(ConfiguracionesOffline-gvLogProcesos_RowCommand): Servicio.UpdateSQL(LogProceso) => " + ret.ToString() + ". Id: " + l.Id + ", PID: " + l.PID.ToString() + ", IdSistema: " + l.IdSistema + ", Usuario: " + l.Usuario + ", Descripcion: " + l.Descripcion);
                    if (ret <= 0)
                    {
                        logDebugView.Error("(ConfiguracionesOffline-gvLogProcesos_RowCommand): Error UpdateSQL(ret:" + ret.ToString() + ") en LogProceso. Quitamos el IdSistema y modificamos el resto de columnas");
                        // Quitamos el IdSistema y modificamos el resto de columnas
                        l.IdSistema = null;
                        ret = Servicio.UpdateSQL(l);
                    }
                    // Registramos la incidencia
                    int idIncidencia;
                    if (!Int32.TryParse(Application["IncidenciaConfiguracionesOffline"].ToString(), out idIncidencia))
                        idIncidencia = 6000;

                    ServiciosCD40.HistoricoIncidencias inc = new ServiciosCD40.HistoricoIncidencias();
                    inc.IdSistema = idSistema;
                    inc.Scv = 0;
                    inc.IdHw = "GOFF";
                    inc.TipoHw = 0;
                    //inc.Usuario = ((FormsIdentity)Context.User.Identity).Name;
                    inc.IdIncidencia = (uint)idIncidencia;
                    inc.Descripcion = l.Descripcion;
                    Servicio.InsertSQL(inc);
                    break;
                default:
                    break;
            }
        }

        MuestraLogProcesos(DameDatosLogProcesos());
    }

    protected void lnkNueva_Click(object sender, EventArgs e)
    {
        MostrarVentanaNuevaConf(Accion.Nueva);
    }
    protected void lnkImportar_Click(object sender, EventArgs e)
    {
        MostrarVentanaNuevaConf(Accion.Importar);
    }
    protected void lnkReplicar_Click(object sender, EventArgs e)
    {
        accionProceso = Enum.GetName(typeof(Accion), Accion.Replicar);
        scriptProceso = Application["ScriptReplicar"].ToString();
        descProceso = "Sincronizando esquemas. ";
        if (string.IsNullOrEmpty(dbUser) || string.IsNullOrEmpty(dbPassword))
        {
            Configuration webConfig = WebConfigurationManager.OpenWebConfiguration("~");
            dbUser = webConfig.AppSettings.Settings["DBAccesU"].Value;
            dbPassword = webConfig.AppSettings.Settings["DBAccesP"].Value;
        }
        argProceso = dbUser + " " + dbPassword + " " + IPLocal + " " + IPRemoto;

        cMsg.confirm("¿Desesa eliminar antes los esquemas de " + IPRemoto + " que no existen en " + IPLocal + "?", "EjecutarProceso");
    }

    private void MostrarVentanaNuevaConf(Accion accion)
    {
        // Comprobar el máximo de configuraciones offline permitidas 
        if (datosConfOffline.Length > maxConfOffline)
            cMsg.alert("Se ha alcanzado el máximo de configuraciones offline permitidas (" + maxConfOffline.ToString() + "). \nNo se permitirá crear nuevas configuraciones mientras no se elimine alguna.");
        else
        {
            txtNuevaConfAccion.Text = Enum.GetName(typeof(Accion), accion);
            txtNuevaConfNombre.Text = string.Empty;
            txtNuevaConfDescripcion.Text = string.Empty;

            if (!(accion == Accion.Importar))
            {
                lblNuevaConfFicheros.Visible = false;
                lstNuevaConfFicheros.Visible = false;
            }
            else
            {
                // Cancelar la operación si el directorio desde el que se van a importar los datos no existe
                string pathConfOffline = Application["PathConfOffline"].ToString();
                try
                {
                    lblNuevaConfFicheros.Text = "Seleccionar fichero en " + pathConfOffline + ":";
                    lblNuevaConfFicheros.Visible = true;
                    lstNuevaConfFicheros.Visible = true;
                    lstNuevaConfFicheros.Items.Clear();

                    var ficheros = System.IO.Directory.EnumerateFiles(pathConfOffline, "*.sql");

                    if (ficheros != null)
                    {
                        foreach (string fich in ficheros)
                        {
                            lstNuevaConfFicheros.Items.Add(fich.Substring(pathConfOffline.Length + 1));
                        }
                    }
                    lstNuevaConfFicheros.Items.Insert(0, new ListItem("Seleccione el fichero de Origen...", "0"));
                }
                catch (DirectoryNotFoundException e)
                {
                    cMsg.alert("El directorio " + pathConfOffline.Replace("\\", "\\\\") + " no existe.");
                    return;
                }
                catch (Exception e)
                {
                    cMsg.alert("Error al abrir la ventana para la nueva configuración offline.\n" + e.ToString());
                    return;
                }
            }

            modalNuevaConfiguracion.Show();
        }
    }
    private void NuevaConfiguracion(Accion accion)
    {
        string operacion, script, argumentos, pathConfOffline;

        modalNuevaConfiguracion.Hide();

        pathConfOffline = Application["PathConfOffline"].ToString();
        operacion = script = argumentos = "";

        if (string.IsNullOrEmpty(dbUser) || string.IsNullOrEmpty(dbPassword))
        {
            Configuration webConfig = WebConfigurationManager.OpenWebConfiguration("~");
            dbUser = webConfig.AppSettings.Settings["DBAccesU"].Value;
            dbPassword = webConfig.AppSettings.Settings["DBAccesP"].Value;
        }

        switch (accion)
        {
            case Accion.Nueva:
                operacion = "Creando configuración " + txtNuevaConfNombre.Text;
                argumentos = dbUser + " " + dbPassword + " " + txtNuevaConfNombre.Text + " \"" + txtNuevaConfDescripcion.Text + "\" LOCALHOST";
                script = Application["ScriptNueva"].ToString();
                break;
            case Accion.Importar:
                if (lstNuevaConfFicheros.SelectedItem.Value == "0")
                {
                    cMsg.alert("Es necesario seleccionar el fichero a importar");
                    modalNuevaConfiguracion.Show();
                    break;
                }
                if (CheckVersion(pathConfOffline + "\\" + lstNuevaConfFicheros.SelectedItem.Value))
                {
                    operacion = "Importando " + lstNuevaConfFicheros.SelectedItem.Value + " en " + txtNuevaConfNombre.Text;
                    argumentos = dbUser + " " + dbPassword + " " + txtNuevaConfNombre.Text + " \"" + txtNuevaConfDescripcion.Text + "\" 1 \"" + pathConfOffline + "\\" + lstNuevaConfFicheros.SelectedItem.Value + "\" LOCALHOST ";
                    script = Application["ScriptImportar"].ToString();
                }
                else
                {
                    modalNuevaConfiguracion.Show();
                }
                break;
            case Accion.Copiar:
                operacion = "Copiando " + txtNuevaConfOrigen.Text + " en " + txtNuevaConfNombre.Text;
                argumentos = dbUser + " " + dbPassword + " " + txtNuevaConfOrigen.Text + " " + txtNuevaConfNombre.Text + " \"" + txtNuevaConfDescripcion.Text + "\" LOCALHOST";
                script = Application["ScriptCopiar"].ToString();
                break;
            default:
                break;
        }

        if (!string.IsNullOrEmpty(script))
        {
            LanzarScript(script, argumentos, operacion);
        }

        MuestraDatos();

    }

    protected void btnNuevaConfAceptar_Click(object sender, EventArgs e)
    {
        // Botón aceptar de la ventana modal para la nueva configuración
        bool confExiste = false;
        string comando = txtNuevaConfAccion.Text;
        //string nombreConf = getNombreconf();

        // Comprobar si la configuración ya existe
        if (txtNuevaConfNombre.Text.ToLower().IndexOf("new_cd40") != 0)
        {
            txtNuevaConfNombre.Text = "new_cd40_" + txtNuevaConfNombre.Text.ToLower();
        }

        foreach (ServiciosCD40.Configuraciones conf in datosConfOffline)
        {
            if (txtNuevaConfNombre.Text.ToLower() == conf.Nombre.ToLower())
                confExiste = true;
        }

        if (confExiste)
        {
            modalConfirmarNuevaConf.Show();
        }
        else
        {
            NuevaConfiguracion((Accion)Enum.Parse(typeof(Accion), txtNuevaConfAccion.Text, true));
        }
    }
    protected void btnNuevaConfCancelar_Click(object sender, EventArgs e)
    {
        modalNuevaConfiguracion.Hide();
    }

    protected void btnConfirmarNuevaConfAceptar_Click(object sender, EventArgs e)
    {
        modalConfirmarNuevaConf.Hide();
        NuevaConfiguracion((Accion)Enum.Parse(typeof(Accion), txtNuevaConfAccion.Text, true));
    }
    protected void btnConfirmarNuevaConfCancelar_Click(object sender, EventArgs e)
    {
        modalConfirmarNuevaConf.Hide();
        modalNuevaConfiguracion.Show();
    }


    private void LanzarScript(string script, string arg, string operacion)
    {
        ProcessStartInfo processInfo;
        Process process;
        try
        {
            // Añadimos como primer argumento el guid, para que el script pueda dejar log del proceso
            Guid guid = Guid.NewGuid();
            string guidProceso;
            guidProceso = guid.ToString();

            logDebugView.Debug("(ConfiguracionesOffline-LanzarScript): " + script + " " + guidProceso + " " + arg);

            processInfo = new ProcessStartInfo("cmd.exe", "/c @" + script + " " + guidProceso + " " + arg);
            processInfo.WorkingDirectory = Application["PathScripts"].ToString();
            //processInfo.CreateNoWindow = true;
            processInfo.WindowStyle = ProcessWindowStyle.Minimized;
            processInfo.UseShellExecute = false;
            processInfo.RedirectStandardError = true;
            processInfo.RedirectStandardOutput = true;
            
            process = Process.Start(processInfo);

            process.EnableRaisingEvents = true;
            process.Exited += new EventHandler(OnProcessExited);
            process.OutputDataReceived += new DataReceivedEventHandler(OnProcessStdOut);
            process.BeginOutputReadLine();

            IntPtr hProceso = process.Handle;

            // Insertamos el registro en el log de procesos
            ServiciosCD40.LogProcesos l = new ServiciosCD40.LogProcesos();
            l.Usuario = ((FormsIdentity)Context.User.Identity).Name;
            l.Id = guidProceso;
            l.PID = process.Id;
            l.IdSistema = Application["IdSistema"].ToString();
            l.Activo = true;
            l.Descripcion = operacion;
            int ret = Servicio.InsertSQL(l);
            if (ret < 0)
            {
                logDebugView.Debug("(ConfiguracionesOffline-LanzarScript): Error InsertSQL(ret:" + ret.ToString() + ") en LogProceso (Id, PID, IdSistema, Usuario, Descripcion) " + l.Id + ", " + l.PID.ToString() + ", " + l.IdSistema + ", " + l.Usuario + ", " + l.Descripcion);
                // Si no ha insertado ningún registro, intentamos modificarlo
                l.Descripcion = null;
                ret = Servicio.UpdateSQL(l);
                logDebugView.Debug("(ConfiguracionesOffline-LanzarScript): Servicio.UpdateSQL(LogProceso) => " + ret.ToString() + ". Id: " + l.Id + ", PID: " + l.PID.ToString() + ", IdSistema: " + l.IdSistema + ", Usuario: " + l.Usuario + ", Descripcion: " + l.Descripcion);
                if (ret <= 0)
                {
                    logDebugView.Error("(ConfiguracionesOffline-LanzarScript): Error UpdateSQL(ret:" + ret.ToString() + ") en LogProceso. Quitamos el IdSistema y modificamos el resto de columnas");
                    // Quitamos el IdSistema y modificamos el resto de columnas
                    l.IdSistema = null;
                    ret = Servicio.UpdateSQL(l);
                }
            }

            ShowWindow(hProceso, 2);

        }
        catch (Exception err)
        {
            logDebugView.Error("(ConfiguracionesOffline-LanzarScript): Error al ejecutar el proceso de " + operacion + "\n" + err.Message.ToString());
            cMsg.alert("Error al ejecutar el proceso de " + operacion + "\n" + err.Message.ToString());
        }
    }

    void OnProcessExited(object sender, EventArgs e)
    {
        Process p = sender as Process;
        string processError;
        string arg, script;
        int pos1, pos2;

        timerRefrescoLogProcesos.Enabled = false;
        processError = p.StandardError.ReadToEnd();

        arg = p.StartInfo.Arguments;
        pos1 = arg.IndexOf("@");
        pos2 = arg.IndexOf(".bat", pos1);
        script = arg.Substring(pos1 + 1, pos2 - pos1 + 4);

        if (!string.IsNullOrEmpty(processError))
            ProcesarError(script, processError, false);
    }

    void OnProcessStdOut(object sender, DataReceivedEventArgs e)
    {
        Process p = sender as Process;
        string arg, script;
        int pos1, pos2;

        arg = p.StartInfo.Arguments;
        pos1 = arg.IndexOf("@");
        pos2 = arg.IndexOf(".bat", pos1);
        script = arg.Substring(pos1 + 1, pos2 - pos1 + 4);

        logDebugView.Debug("(ConfiguracionesOffline-OnProcessStdOut) " + script + ": " + e.Data);

    }

    protected void timerRefrescoLogProcesos_Tick(object sender, EventArgs e)
    {
        logDebugView.Debug("(ConfiguracionesOffline-timerRefrescoLogProcesos_Tick) ");
    }

    private void ProcesarError(string script, string err, bool showMsg = true)
    {
        string processError, codError, msgError, mensaje;

        processError = err;
        processError = processError.Replace("\r\n", "\n");
        processError = processError.Replace("\r", "\n");

        if (processError.IndexOf("\n") > 0)
            mensaje = processError.Substring(0, processError.IndexOf("\n"));
        else
            mensaje = processError;

        if (mensaje.IndexOf(":") > 0)
        {
            codError = mensaje.Substring(0, mensaje.IndexOf(":")).Trim();
            mensaje = mensaje.Substring(mensaje.IndexOf(":") + 1).Trim();
            if (string.IsNullOrEmpty(mensaje))
                msgError = "Error al ejecutar el script " + script + "\n" + codError;
            else
                msgError = "Error al ejecutar el script " + script + "\n" + "Codigo: " + codError + "\n" + mensaje;
        }
        else
            msgError = "Error al ejecutar el script " + script + "\n" + mensaje;

        if (showMsg)
            cMsg.alert(msgError);
        else
            logDebugView.Error("(ConfiguracionesOffline-ProcesarError): MENSAJE ERROR" + msgError);

        logDebugView.Error("(ConfiguracionesOffline-ProcesarError): Error " + script);
        if (!string.IsNullOrEmpty(processError))
            logDebugView.Error("(ConfiguracionesOffline-ProcesarError): ERROR  \n" + processError);

    }

    private bool CheckVersion(string fichero)
    {
        string version;
        bool versionEncontrada = false;

        version = Application["CFGVersion"].ToString();

        foreach (string linea in File.ReadLines(fichero))
        {
            if (linea.IndexOf(literalVersion) == 0)
            {
                versionEncontrada = true;
                if (!linea.Trim().Equals(literalVersion + version, StringComparison.OrdinalIgnoreCase))
                {
                    cMsg.alert("La versión con la que se ha exportado el fichero (" + linea.Substring(literalVersion.Length) + ") no se corresponde con al versión que se está ejecutando (" + version + ")");
                    return false;
                }
            }
        }

        if (versionEncontrada)
            return true;
        else
        {
            cMsg.alert("El fichero " + fichero.Replace("\\", "\\\\") + " no incluye información de la versión con la que se ha exportado");
            return false;
        }
    }

    private bool ExisteProceso(int pid)
    {
        Process proceso;

        try
        {
            proceso = Process.GetProcessById(pid);
            return true;
        }
        catch (ArgumentException e)
        {
            logDebugView.Error("(ConfiguracionesOffline-ExisteProceso) El proceso " + pid + " no existe");
        }
        catch (Exception e)
        {
            logDebugView.Error("(ConfiguracionesOffline-ExisteProceso) Proceso: " + pid + ". Error: " + e.ToString());
        }
        return false;
    }

    private void PararProceso(int pid)
    {
        if (ExisteProceso(pid))
        {
            Process proceso;

            try
            {
                proceso = Process.GetProcessById(pid);
                proceso.Kill();
                proceso.WaitForExit();
            }
            catch (Exception e)
            {
                logDebugView.Error("(ConfiguracionesOffline-PararProceso) Proceso: " + pid + ". Error: " + e.ToString());
            }
        }
    }

    private void ProcesosDisponibles(bool disponible)
    {
        //logDebugView.Error("(ConfiguracionesOffline-ProcesosDisponibles) " + (disponible? "SI" : "NO"));
        foreach (GridViewRow row in gvConfiguraciones.Rows)
        {
            ((LinkButton)row.FindControl("lnkExportar")).Visible = disponible;
            ((LinkButton)row.FindControl("lnkCopiar")).Visible = disponible;

            if (row.Cells[2].Text == "ONLINE")
            {
                ((LinkButton)row.FindControl("lnkEditar")).Visible = false;
                ((LinkButton)row.FindControl("lnkEliminar")).Visible = false;
                ((LinkButton)row.FindControl("lnkActivar")).Visible = false;
            }
            else
            {
                ((LinkButton)row.FindControl("lnkEditar")).Visible = disponible;
                ((LinkButton)row.FindControl("lnkEliminar")).Visible = disponible;
                ((LinkButton)row.FindControl("lnkActivar")).Visible = disponible;
            }
        }

        lnkNueva.Visible = disponible;
        lnkImportar.Visible = disponible;
        if ((bool)Application["Cnf_ModoCluster"])
            lnkReplicar.Visible = disponible;
        else
            lnkReplicar.Visible = false;
    }
}