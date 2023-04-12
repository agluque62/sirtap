using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Collections.Generic;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Web.Configuration;
using log4net;
using log4net.Config;
using System.Text.RegularExpressions;
using System.Text;
using System.Linq;
public partial class PingServer : PageBaseCD40.PageCD40
{

    private static ServiciosCD40.ServiciosCD40 serviceServiciosCD40;
    protected void Page_Load(object sender, EventArgs e)
    {
        string usuario = string.Empty;
        string perfil = string.Empty;
        string version = string.Empty;




        if (Context.Request.IsAuthenticated)
        {
            FormsIdentity ident = (FormsIdentity)Context.User.Identity;
            usuario = ident.Ticket.Name;
            perfil = ident.Ticket.UserData;
            if (serviceServiciosCD40 == null)
                serviceServiciosCD40 = new ServiciosCD40.ServiciosCD40();
            FormsAuthenticationTicket ticket = new FormsAuthenticationTicket(1,
                                                                            ident.Ticket.Name,
                                                                            ident.Ticket.IssueDate,
                                                                            ident.Ticket.Expiration,
                                                                            false,
                                                                            ident.Ticket.UserData,
                                                                            FormsAuthentication.FormsCookiePath);
            // Encrypt the ticket.
            // Encrypt the ticket.
            string encTicket = FormsAuthentication.Encrypt(ticket);
            // Create the cookie.
            Response.Cookies.Add(new HttpCookie(FormsAuthentication.FormsCookieName, encTicket));
            serviceServiciosCD40.GestionSesionOperador("departamento", usuario, 1, 1);


        }
        else
        {

            Response.Redirect("~/Login.aspx", false);
        }
        /*
        if (serviceServiciosCD40 == null)
            serviceServiciosCD40 = new ServiciosCD40.ServiciosCD40();

        if (!IsPostBack)
        {
            //Se lee la variable de sesión idsistema
            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            Session["idsistema"] = s.Value;


 
        }

        serviceServiciosCD40.GestionSesionOperador("departamento",usuario,1,1);*/
      
    }
}