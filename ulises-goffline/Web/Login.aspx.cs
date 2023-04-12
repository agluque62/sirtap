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
using System.Collections.Generic;

public partial class LoginCD40 : System.Web.UI.Page
{

    static int MAXIMASESIONESUSUARIOS = 65535;// Falta implementar correctamente 2 máximo
    static int Sesiones = 0;
    static int Perfil = 0;
    static int TimeOutSesion = 0;
	private static ServiciosCD40.ServiciosCD40 ServiceServiciosCD40 = new ServiciosCD40.ServiciosCD40();

    public class UserUlises
    {
        public string UserName { get; set; }
        public string SessionId { get; set; }
    }
    private static List<UserUlises> _sessions = new List<UserUlises>();

    public static void RegisterLogin(UserUlises user)
    {
        if (user != null)
        {
            _sessions.RemoveAll(u => u.UserName == user.UserName);
            _sessions.Add(user);
        }
    }

    public static void DeregisterLogin(UserUlises user)
    {
        if (user != null)
        {
            if (!string.IsNullOrEmpty(user.UserName))
            {
                _sessions.RemoveAll(u => u.UserName == user.UserName);
            }
            else
            {
                _sessions.RemoveAll(u => u.SessionId == user.SessionId);
            }
        }
    }

    public static bool ValidateCurrentLogin(UserUlises user)
    {
        if (user != null)
        {
            if (!string.IsNullOrEmpty(user.UserName))
            {
                return user != null && _sessions.Exists(u => u.UserName == user.UserName);
            }
            else
            {
                return user != null && _sessions.Exists(u => u.SessionId == user.SessionId);
            }
        }
        else
            return false;
    }

    public static string GetUserById(UserUlises user)
    {
        string userName = string.Empty;
        foreach (UserUlises userfind in _sessions)
        {
            if (userfind.SessionId == user.SessionId)
            {
                userName = userfind.UserName;
            }
        }
        return userName;
    }

    public static bool AlcanzadoNumeroMaximoSesiones()
    {
        return _sessions.Count >= MAXIMASESIONESUSUARIOS;
    }
	protected void Page_Load(object sender, EventArgs e)
	{
        UserUlises usuario = new UserUlises();
        string userLoggedIn = string.Empty;
        userLoggedIn = (string)Session["UserLoggedIn"];

        if (!Page.IsPostBack)
        {
           
            // RQF 30
            if (!string.IsNullOrEmpty(userLoggedIn))
            {
                usuario.UserName = userLoggedIn;
            }
            else
                userLoggedIn = string.Empty;
            
            string sessionUserId = usuario.SessionId = Session.SessionID;
            
            if (userLoggedIn.Length > 0 || !string.IsNullOrEmpty(sessionUserId))
            {
                if (string.IsNullOrEmpty(userLoggedIn))
                {
                    usuario.SessionId = sessionUserId;
                    userLoggedIn = GetUserById(usuario);
                }

                System.Collections.Generic.List<string> d = Application["UsersLoggedIn"] 
                    as System.Collections.Generic.List<string>;
                if (d != null)
                {
                    lock (d)
                    {
                        d.Remove(userLoggedIn);
                        DeregisterLogin(usuario);
                    }
                }
                else if (!string.IsNullOrEmpty(usuario.SessionId) || !string.IsNullOrEmpty(usuario.UserName))
                {
                    DeregisterLogin(usuario);
                }

            }
            Session.Abandon();
        }


        Response.Cookies.Add(new HttpCookie("ASP.NET_SessionId", ""));
        SetFocus(Login1);
        Panel3.Visible = Request.Params["Logout"] == "SI";
		Login1.Visible = !Panel3.Visible;
	}


	private bool UsuarioValido(string id, string pwd, out int perfil, out int timeoutSesion)
	{
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
            timeoutSesion = 30;
			ServiciosCD40.Operadores operador = new ServiciosCD40.Operadores();
			operador.IdSistema = s.Value;
			operador.IdOperador = Login1.UserName;
			operador.Clave = Login1.Password;

			ServiciosCD40.Tablas[] validado = ServiceServiciosCD40.ListSelectSQL(operador);

            if (validado.Length > 0)
            {
                perfil = (int)((ServiciosCD40.Operadores)validado[0]).NivelAcceso;
                timeoutSesion = (int)((ServiciosCD40.Operadores)validado[0]).TimeoutSesion;
            }

			return validado.Length > 0;
		}
	}

	protected void Login_Authenticate(object sender, AuthenticateEventArgs e)
	{
		int perfil=0;
        // RQF-18
        int timeoutSesion = 0;

		if (UsuarioValido(Login1.UserName, Login1.Password, out perfil, out timeoutSesion))
		{
			Perfil = perfil;
            TimeOutSesion = timeoutSesion;
            UsuarioAutenticado();

		}
		else if (perfil < 0)	// Aplicación bloqueada por estar otro usuario en ella
		{
			Response.Redirect("~/BloqueoAplicacion.aspx");
		}
	}

	private void UsuarioAutenticado()
	{
        //SetAuthCookie
        UserUlises usuario = new UserUlises();
		string userData = Perfil.ToString();
        usuario.SessionId = Session.SessionID;
        usuario.UserName = Login1.UserName;
        // RQF 30
        System.Collections.Generic.List<string> d = Application["UsersLoggedIn"]
            as System.Collections.Generic.List<string>;
        if (d != null)
        {
            lock (d)
            {

                if (!d.Contains(Login1.UserName) && d.Count < MAXIMASESIONESUSUARIOS)
                {
                    d.Add(Login1.UserName);
                }
                else if (!d.Contains(Login1.UserName) && d.Count >= MAXIMASESIONESUSUARIOS)
                {
                    Response.Redirect("~/MaximoSesiones.aspx");
                }      
            }
        }
        Session["UserLoggedIn"] = Login1.UserName;

        if (!ValidateCurrentLogin(usuario))
        {
            RegisterLogin(usuario);
        }
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
		Response.Cookies.Add(new HttpCookie(FormsAuthentication.FormsCookieName, encTicket));
        // Redirect back to original URL.
		Response.Redirect(FormsAuthentication.GetRedirectUrl(Login1.UserName, false));
	}

	private bool GestionarUltimasModificaciones()
	{


		return true;
	}
	
	protected void OnBtnAceptarModoContinuar(object sender, EventArgs e)
	{
        //if (RBContinuarActiva.Checked)
        //{
        //    // Recupera la última configuración activa como configuración de trabajo
        //    ServiceServiciosCD40.Rollback();
        //}

		UsuarioAutenticado();
	}

	protected void LinkButton1_Click(object sender, EventArgs e)
	{
        Response.Redirect("~/Login.aspx", false);
	}


    protected void Timer_Msj(object sender, EventArgs e)
    {
         int i = 0;
        i++;
    }

}
