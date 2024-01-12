using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Linq;
using System.Timers;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using ServiciosCD40;
using log4net;
using log4net.Config;

using UtilitiesCD40;
using System.Collections.Generic;
using System.Web.SessionState;

public partial class CD40 : System.Web.UI.MasterPage
{
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

    static ServiciosCD40.ServiciosCD40 serviceServiciosCD40;
    static private bool bSinTelefoniaATS = false; // Indica si no se deben mostar las opciones de telefonía ATS (troncales y encaminamientos)
    static private bool bAccesoInfOperadores = false; // Indica si se debe mostrar el informe de operadores
    static private bool bAccesoDependenciasATS = false; // Indica si se debe mostrar la gestión de dependencias ATS
    //20211130 #2851
    static private bool bAccesoBackupRestore = false; // Indica si se debe mostrar la gestión de Backup/Restore
    // RQF-33 
    static private bool bSistemaEnConfiguracionOffline = false;
    static private string sistema = string.Empty;
    static private string operador = string.Empty;
    static private bool bAccesoConferenciasPreprogramadas = false;

	protected void Page_Load(object sender, EventArgs e)
	{
        NLogLogger.Trace<CD40>(String.Format("Master Page Load Event IsPostBack {0}", IsPostBack));
        if (Session.IsNewSession)
            Response.Redirect("~/Login.aspx", false);

        if (serviceServiciosCD40 == null)
            serviceServiciosCD40 = new ServiciosCD40.ServiciosCD40();

		if (Context.Request.IsAuthenticated)
		{
			// Capturar el idioma para el sitemap
			if (System.Globalization.CultureInfo.CurrentCulture.TwoLetterISOLanguageName == "es")
				SiteMapDataSource1.Provider = SiteMap.Providers["Spanish-Sitemap"];
            else if (System.Globalization.CultureInfo.CurrentCulture.TwoLetterISOLanguageName == "fr")
				SiteMapDataSource1.Provider = SiteMap.Providers["French-Sitemap"];
            else
                SiteMapDataSource1.Provider = SiteMap.Providers["English-Sitemap"];
			
			string[] strPerfiles ={ (string)GetGlobalResourceObject("Espaniol", "Operador"),
                                  (string)GetGlobalResourceObject("Espaniol", "Tecnico1"),
                                  (string)GetGlobalResourceObject("Espaniol", "Tecnico2"),
                                  (string)GetGlobalResourceObject("Espaniol", "Tecnico3"),
                                  (string)GetGlobalResourceObject("Espaniol", "Supervisor")};
            
            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            sistema = s.Value;
			// retrieve user's identity from httpcontext user 
			FormsIdentity ident = (FormsIdentity)Context.User.Identity;
            string strPerfil = ident.Ticket.UserData;
            operador = ident.Ticket.Name;
            switch (strPerfil)
			{
				case "0":
					break;
				case "1":
				case "2":
					BtCluster.Visible = false;
					break;
				case "3":
					break;
                case "4":
                    BtCluster.Visible = false;
                    break;
				default:
					break;
			}

            LabelPerfil.Text = strPerfiles[Convert.ToInt16(strPerfil)];
			LabelUsuario.Text = ident.Name;

            if (false == IsPostBack)
            {
                bAccesoConferenciasPreprogramadas = MostrarConferenciasPreprogramadas(); 
                // 20210317 #4749
                // Acceso a configuración dependenciasATS
                //bAccesoDependenciasATS = SistemaConDependenciasATS();
                // RQF-33
                if (SistemaEnConfiguracionOffline())
                {
                    bSistemaEnConfiguracionOffline = true;
                    LbIdEsquema.Text = serviceServiciosCD40.GetIdEsquema();
                    bAccesoBackupRestore = false;
                }
                if (TreeView1.Visible || DivWizard.Visible)
                {
                    //Se lee del fichero de configuración si hay que eliminar del árbol de menú las opciones
                    //Encaminamientos y Troncales
                    if (Ulises5000Configuration.ToolsUlises5000Section.Instance.Tools["SinTelefoniaATS"] != null)
                    {
                        bSinTelefoniaATS = true;
                    }
                    else
                        bSinTelefoniaATS = false;
                }

                if (DivWizard.Visible)
                {
                    //Si estamos en la pantalla del wizard
                    if (bSinTelefoniaATS)
                    {
                        //Se ocultan en la pantalla del wizard los botones de encaminamientos y troncales
                        BtnTroncales.Visible = false;
                        BtnEncaminamientos.Visible = false;
                    }

                    // 20210317 #4749
                    // Acceso a configuración dependenciasATS en Wizard siempre desactivado
                     bAccesoDependenciasATS = false;
                     //20211130 #2851
                     // Acceso a Backup/Restore en Wizard siempre desactivado
                     bAccesoBackupRestore = false;

                    if (string.Compare(strPerfil, "3") == 0)
                        BtnOperadores.Visible = true;
                    else
                        BtnOperadores.Visible = false;

                    if (null != Session["PasoWizard"])
                    {
                        try
                        {
                            int iPaso = (int)Session["PasoWizard"];

                            //Activamos el botón correspondiente al paso en el que estamos
                            ActivarOpcionMenu(iPaso, bSinTelefoniaATS);
                        }
                        catch (Exception)
                        {
                        }
                    }
                }
                else
                {
                    //Solo el perfil 3 tiene acceso al informe de operadores
                    //20211130 #2851
                    if (string.Compare(strPerfil, "3") == 0 && !SistemaEnConfiguracionOffline())
                    {
                        bAccesoInfOperadores = true;
                        bAccesoBackupRestore = true;
                    }
                    else
                    {
                        bAccesoInfOperadores = false;
                        bAccesoBackupRestore = false;
                    }
                }

                //Si la aplicación se configura en modo cluster, la entrada del Web Config [Servidor-2] debe tomar el valor SI
                //Si no está configurada en modo cluster (null == config.AppSettings.Settings["Servidor-2"] || config.AppSettings.Settings["Servidor-2"].Value!= "SI") 
                //se oculta el enlace a la pantalla de mantenimiento del cluster
                //La variable Cnf_ModoCluster, se lee en el módulo Global.asax cuando se accede por primera vez a la aplicación
                if  ((null==Application["Cnf_ModoCluster"] || false == (bool) Application["Cnf_ModoCluster"]) && BtCluster.Visible)
                {
                    BtCluster.Visible = false;
                }

                if (bSistemaEnConfiguracionOffline)
                {
                    BtCluster.Visible = false;
                    BtMantenimiento.Visible = false;
                    bAccesoBackupRestore = false;
                    LabelOffline.Visible = true;
                    LblEsquema.Visible = true;
                    LbIdEsquema.Visible = true;
                }

            }

        }
    }

    //Activa el botón correspondiente al paso que se está visualizando en el Wizard
    private void ActivarOpcionMenu(int piPaso, bool pbSinTelefoniaATS)
    {
        int iPaso = piPaso;

        if (!BtnOperadores.Visible)
        {
            iPaso = iPaso + 1;
        }

        if (pbSinTelefoniaATS && iPaso >= 5)
        {
            //Nos saltamos los botones BtnTroncales y Encaminamientos que están en la posición 5 y 6
            iPaso += 2;
        }

        switch (iPaso)
        {
            case 0:
                BtnNucleos.CssClass = "textbox";
                break;
            case 1:
                BtnOperadores.CssClass = "textbox";
                break;
            case 2:
                BtnTifx.CssClass = "textbox";
                break;
            case 3:
                BtnEqEx.CssClass = "textbox";
                break;
            case 4:
                BtnTops.CssClass = "textbox";
                break;
            case 5:
                BtnRedes.CssClass = "textbox";
                break;
            case 6:
                BtnTroncales.CssClass = "textbox";
                break;
            case 7:
                BtnEncaminamientos.CssClass = "textbox";
                break;
            case 8:
                BtnRecursosTf.CssClass = "textbox";
                break;
            case 9:
                BtnGrupos.CssClass = "textbox";
                break;
            case 10:
                BtnDestinosTf.CssClass = "textbox";
                break;
            case 11:
                BtnEmplazamientos.CssClass = "textbox";
                break;
            case 12:
                BtnZonas.CssClass = "textbox";
                break;
            case 13:
                BtnTabCalidad.CssClass = "textbox";
                break;
            case 14:
                BtnRecursosRd.CssClass = "textbox";
                break;
            case 15:
                BtnDestinosRadio.CssClass = "textbox";
                break;
            case 16:
                BtnSectores.CssClass = "textbox";
                break;
            case 17:
                BtnAgrupaciones.CssClass = "textbox";
                break;
            case 18:
                BtnSectorizaciones.CssClass = "textbox";
                break;
            default:
                break;
        }
    }

    protected void TreeView1_TreeNodeDataBound(object sender, TreeNodeEventArgs e)
    {
        //Lee las opciones configuradas en el Web.xx.sitemap
        if (bSinTelefoniaATS)
        {
            // Si la opción SinTelefoniaATS está configurada, se eliminan las opciones encaminamientos --> "~/Configuracion/Encaminamientos.aspx"
            // y troncales -->"~/Configuracion/Troncales.aspx" del arbol de menú
            try
            {
                SiteMapNode mapNode = (SiteMapNode)e.Node.DataItem;

                if (null != mapNode)
                {
                    //Para identificar a estos elementos del menú, en los ficheros Web.xx.sitemap se ha añadido a la definición de ambos sitemapNode la propiedad identificador con el valor correspondiente

                    if (null != mapNode["identificador"])
                    {
                        string strId = mapNode["identificador"];

                        switch (strId)
                        {
                            case "Troncales":
                            case "Encaminamientos":
                                System.Web.UI.WebControls.TreeNode parent = e.Node.Parent;
                                if (parent != null)
                                {
                                    parent.ChildNodes.Remove(e.Node);
                                }
                                break;
                            case "InfOperadores":
                                if (!bAccesoInfOperadores)
                                {
                                    if (e.Node.Parent != null)
                                    {
                                        e.Node.Parent.ChildNodes.Remove(e.Node);
                                    }
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
            catch (Exception)
            {
            }
        }
        if (!bAccesoInfOperadores)
        {
            try
            {
                SiteMapNode mapNode = (SiteMapNode)e.Node.DataItem;

                if (null != mapNode && null!=mapNode["identificador"])
                {
                    string strId = mapNode["identificador"];

                    if (string.Compare(strId,"InfOperadores",true)==0)
                    {
                        System.Web.UI.WebControls.TreeNode parent = e.Node.Parent;
                        if (parent != null)
                        {
                            parent.ChildNodes.Remove(e.Node);
                        }
                    }
                }
            }
            catch (Exception)
            {
            }
        }
        if (!bAccesoConferenciasPreprogramadas)
            {
                try
                {
                    SiteMapNode mapNode = (SiteMapNode)e.Node.DataItem;

                    if (null != mapNode && null != mapNode["identificador"])
                    {
                        string strId = mapNode["identificador"];
                        if (string.Compare(strId, "Conferencias", true) == 0)
                        {
                            System.Web.UI.WebControls.TreeNode parent = e.Node.Parent;
                            if (parent != null)
                            {
                                parent.ChildNodes.Remove(e.Node);
                            }
                        }
                    }
                }
                catch (Exception)
                {
                }
        }
        // 20210317 #4749
        if ((!bAccesoDependenciasATS) || (bAccesoDependenciasATS == true && ((FormsIdentity)Context.User.Identity).Ticket.Name != "*CD40*"))
        {
            try
            {
                SiteMapNode mapNode = (SiteMapNode)e.Node.DataItem;

                if (null != mapNode && null != mapNode["identificador"])
                {
                    string strId = mapNode["identificador"];

                    if (string.Compare(strId, "DependenciasATS", true) == 0)
                    {
                        System.Web.UI.WebControls.TreeNode parent = e.Node.Parent;
                        if (parent != null)
                        {
                            parent.ChildNodes.Remove(e.Node);
                        }
                    }
                }
            }
            catch (Exception)
            {
            }
        }
        //20211130 #2851
        if (!bAccesoBackupRestore)
        {
            try
            {
                SiteMapNode mapNode = (SiteMapNode)e.Node.DataItem;

                if (null != mapNode && null != mapNode["identificador"])
                {
                    string strId = mapNode["identificador"];
                    if (string.Compare(strId, "BackupRestore", true) == 0)
                    {
                        System.Web.UI.WebControls.TreeNode parent = e.Node.Parent;
                        if (parent != null)
                        {
                            parent.ChildNodes.Remove(e.Node);
                        }
                    }
                }
            }
            catch (Exception)
            {
            }
        }

        NLogLogger.Trace<CD40>(String.Format("TreeView1_TreeNodeDataBound => {0}", e.Node.ValuePath));
        MenuPersistence.RestoreExpandedStatus(Session, TreeView1);
    }


	protected void BtInicio_Click(object sender, EventArgs e)
	{
        string url = "~/Default.aspx";
        Response.Redirect(url, false);
	}
	protected void BtCluster_Click(object sender, EventArgs e)
	{
        Response.Redirect("~/Cluster/Default.aspx", false);
	}
	protected void BtMantenimiento_Click(object sender, EventArgs e)
	{
        System.Configuration.Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        string urlMantto = config.AppSettings.Settings["UrlMantto"].Value;
        string urlRtn = config.AppSettings.Settings["UrlRetorno"].Value;

        string _open = "window.open('" + urlMantto + "', '_newtab');";
        ScriptManager.RegisterStartupScript(this, this.GetType(), Guid.NewGuid().ToString(), _open, true);
    }

	private bool GestionarUltimasModificaciones()
	{
        Session["UserLoggedIn"] = LabelUsuario.Text;

		ServiciosCD40.ServiciosCD40 servicio = new ServiciosCD40.ServiciosCD40();
		if (servicio.HayModificacionesPendientes())
		{
			Panel1.Visible = true;

			return false;
		}
        
        var sc = (SessionsControl)Application["SessionsControl"];
        if (sc != null)
        {
            sc.Logout(Request, Response, (userout) =>
            {
                Response.Redirect("~/Login.aspx", false);
            });
            Response.Redirect("~/Login.aspx", false);
        }
        else
        {
            // Algo pasa... No se ha cargado en memoria el control de sesiones.
            Response.Redirect("~/BloqueoAplicacion.aspx");
        }
		return true;
	}

    protected void OnBtnCancelarModoContinuar(object sender, EventArgs e)
    {
        Panel1.Visible = false;
    }

	protected void LinkButton1_Click(object sender, EventArgs e)
	{
		GestionarUltimasModificaciones();
	}

	protected void OnBtnAceptarModoContinuar(object sender, EventArgs e)
	{
		if (RBContinuarActiva.Checked)
		{
            Response.Redirect("~/Configuracion/Sectorizaciones.aspx", false);
		}
        else
        {
            if (RBContinuarModificada.Checked)
            {
                // recupera la última activa (CD40) como configuración de trabajo (CD40_Trans)
                ServiciosCD40.ServiciosCD40 servicio = new ServiciosCD40.ServiciosCD40();
                servicio.Rollback();
            }
            var sc = (SessionsControl)Application["SessionsControl"];
            if (sc != null)
            {
                sc.Logout(Request, Response, (userout) =>
                {
                    Response.Redirect("~/Login.aspx", false);
                });
               
                    Response.Redirect("~/Login.aspx", false);
            }
            else
            {
                // Algo pasa... No se ha cargado en memoria el control de sesiones.
                Response.Redirect("~/BloqueoAplicacion.aspx");
            }
            
        }
    }

    //20210317 #4749
    public bool SistemaConDependenciasATS()
    {
        const string CONF_KEY_CON_DEPATS = "DependenciasATS";
        bool bConDepATS = false;

        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement objConf = null;

        //Se obtiene el parametro que indica si se debe o no visualizar la gestion de dependencias ATS
        objConf = config.AppSettings.Settings[CONF_KEY_CON_DEPATS];

        if ((objConf != null) && (!string.IsNullOrEmpty(objConf.Value) && string.Compare(objConf.Value, "SI", true) == 0))
        {
            //El sistema está configurado con gestión de dependencias ATS
            bConDepATS = true;
        }

        return bConDepATS;
    }

    // RQF-33
    public bool SistemaEnConfiguracionOffline()
    {
        const string CONF_KEY_CONFIGURACION_OFFLINE = "ConfiguracionOffline";
        bool bConfiguracionOffline = false;

        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement objConf = null;

        //Se obtiene el parametro que indica si la cofiguración es ONLINE u OFFLINE
        objConf = config.AppSettings.Settings[CONF_KEY_CONFIGURACION_OFFLINE];

        if ((objConf != null) && (!string.IsNullOrEmpty(objConf.Value) && string.Compare(objConf.Value, "SI", true) == 0))
        {
            //El sistema está configurado en modo OFFLINE
            bConfiguracionOffline = true;
        }
        return bConfiguracionOffline;
    }

    public bool MostrarConferenciasPreprogramadas()
    {
        const string CONF_KEY_MOSTRAR_CONFERENCIAS_PREPROGRAMADAS = "MostrarConferenciasPreprogramadas";
        bool bMostrarConferenciasPreprogramadas = false;

        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement objConf = null;

        //Se obtiene el parametro que indica si se debe o no visualizar la gestion de conferencias preprogramadas
        objConf = config.AppSettings.Settings[CONF_KEY_MOSTRAR_CONFERENCIAS_PREPROGRAMADAS];

        if ((objConf != null) && (!string.IsNullOrEmpty(objConf.Value) && string.Compare(objConf.Value, "SI", true) == 0))
        {
            //El sistema está configurado para gestionar conferencias preprogramadas
            bMostrarConferenciasPreprogramadas = true;
        }
        return bMostrarConferenciasPreprogramadas;
    }

    public void FinsesionOperador()
    {

    }
    protected void OnExpanded(object sender, TreeNodeEventArgs e)
    {
        MenuPersistence.SaveExpandedStatus(Session, e.Node, true);
        NLogLogger.Trace<CD40>(String.Format("OnExpandedEvent => {0}", e.Node.ValuePath));
    }

    protected void OnCollapsed(object sender, TreeNodeEventArgs e)
    {
        MenuPersistence.SaveExpandedStatus(Session, e.Node, false);
        NLogLogger.Trace<CD40>(String.Format("OnCollapsedEvent => {0}", e.Node.ValuePath));
    }
}

public class ReverseSort : IComparer
{
    public int Compare(object x, object y)
    {
        // reverse the arguments
        return Comparer.Default.Compare(y, x);
    }
}

public class MenuPersistence
{
    public static void SaveExpandedStatus(HttpSessionState Session, TreeNode node, bool status)
    {
        var expandedNodes = GetexpandedNodes(Session);
        if (status == false)
        {
            if (expandedNodes.Contains(node.ValuePath))
                expandedNodes.Remove(node.ValuePath);
        }
        else
        {
            if (expandedNodes.Contains(node.ValuePath) == false)
            {
                expandedNodes.Add(node.ValuePath);
            }
        }
    }
    public static void RestoreExpandedStatus(HttpSessionState Session, TreeView menu)
    {    
        GetexpandedNodes(Session).ForEach(a =>
        {
            var item = menu.FindNode(a);
            if (item != null)
            {
                item.Expanded = true;
            }
            else
            {
                NLogLogger.Trace<MenuPersistence>(a.ToString() + " => Nodo no encontrado...");
            }
        });
        //NLogLogger.Debug<MenuPersistence>(String.Format("{0} ExpandesNodes => {1}", expandedNodes.Count, string.Join("##", expandedNodes.ToArray())));
    }
    static void GetExpandedStatus(TreeNode node, List<string> ExpandedNodes)
    {
        //check if node is expanded
        if (node.Expanded.GetValueOrDefault(false))
            ExpandedNodes.Add(node.ValuePath);
        var nextLevelNodes = node.ChildNodes.Cast<TreeNode>().ToList();
        nextLevelNodes.ForEach(a => GetExpandedStatus(a, ExpandedNodes));
    }
 
    static List<string> GetexpandedNodes(HttpSessionState Session)
    {
        if (Session["MenuExpandedNodes"] == null)
        {
            Session["MenuExpandedNodes"] = new List<string>();
        }
        return Session["MenuExpandedNodes"] as List<string>;
    }
}

