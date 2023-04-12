using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Web.Configuration;


public partial class _Default : System.Web.UI.Page 
{
    //private static KeyValueConfigurationElement Version;
    private static Ulises5000Configuration.ToolsUlises5000Section UlisesToolsVersion;

    protected void Page_Load(object sender, EventArgs e)
    {
		if (Context.Request.IsAuthenticated)      
		{   
			// retrieve user's identity from httpcontext user 
			FormsIdentity ident = (FormsIdentity)Context.User.Identity;             
			// retrieve roles from the authentication ticket userdata field            
			string perfil = ident.Ticket.UserData;

			switch (perfil)
			{
				case "0":
					Response.Redirect("Controladores.aspx");
					break;
				case "1":
				case "2":
					LBPpalMantenimientoCluster.Enabled = false;
					break;
				case "3":
					break;
				case "4":
                    LBPpalMantenimientoCluster.Enabled = false;
                    LBPpalConfiguracion.Enabled = false;
                    LBWizardConfiguracion.Enabled = false;
					break;
				default:
					break;
			}
		}

        //Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
        //Version = config.AppSettings.Settings["Version"];
        Ulises5000Configuration.ToolsUlises5000Section ulisesVersion = Ulises5000Configuration.ToolsUlises5000Section.Instance;
        UlisesToolsVersion = ulisesVersion;

        //LBEstadisticas.Visible = UlisesToolsVersion.Tools["Estadisticas"] != null; // Sólo para Tenerife (Version == 3)

        //MVO: se deshabilita la acción del enlace del Wizard, si no está habilitado
        if (!LBWizardConfiguracion.Enabled && null!=LBWizardConfiguracion.OnClientClick)
        { 
            LBWizardConfiguracion.OnClientClick = null;
        }

        //Si la aplicación se configura en modo cluster, la entrada del Web Config [Servidor-2] debe tomar el valor SI
        //Si no está configurada en modo cluster (null == config.AppSettings.Settings["Servidor-2"] || config.AppSettings.Settings["Servidor-2"].Value!= "SI") 
        //se oculta el enlace a la pantalla de mantenimiento del cluster
        //La variable Cnf_ModoCluster, se lee en el módulo Global.asax cuando se accede por primera vez a la aplicación

        if ((null == Application["Cnf_ModoCluster"] || false == (bool)Application["Cnf_ModoCluster"]) && LBPpalMantenimientoCluster.Visible)
        {
            LBPpalMantenimientoCluster.Visible = false;
        }
        if (SistemaEnConfiguracionOffline())
        {
            LBPpalMantenimientoCluster.Visible = false;
            LBPpalMantenimiento.Visible = false;
        }
    }

    protected void LBPpalConfiguracion_Click(object sender, EventArgs e)
    {
        Response.Redirect("~/Configuracion/Inicio.aspx");
    }

    protected void LBPpalMantenimientoCluster_Click(object sender, EventArgs e)
    {
		Response.Redirect("~/Cluster/Default.aspx");
    }

    protected void LBPpalMantenimiento_Click(object sender, EventArgs e)
    {
        Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
        string urlMantto = config.AppSettings.Settings["UrlMantto"].Value;
        string urlRtn = config.AppSettings.Settings["UrlRetorno"].Value;
        Response.Redirect(urlMantto + "?RetUrl=" + urlRtn + "&user=" + Context.User.Identity.Name);
    }

	protected void LBWizard_Click(object sender, EventArgs e)
	{
		Response.Redirect("~/Wizard/WizardConfiguracion.aspx");
	}

    protected void LBEstadisticas_Click(object sender, EventArgs e)
    {
        Response.Redirect("~/Mantenimiento/Estadisticas.aspx");
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

}
