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
    private static bool bNoAutorizado;
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
				case "1":
				case "2":
				case "4":
                    bNoAutorizado = true;
					break;
                case "3":
                    bNoAutorizado = false;
                    break;
				default:
                    bNoAutorizado = true;
					break;
			}
		}

        if (bNoAutorizado)
        {
            // RQF 30
            string userLoggedIn = Session["UserLoggedIn"] == null ? string.Empty : (string)Session["UserLoggedIn"];
            if (userLoggedIn.Length > 0)
            {
                System.Collections.Generic.List<string> d = Application["UsersLoggedIn"]
                    as System.Collections.Generic.List<string>;
                if (d != null)
                {
                    lock (d)
                    {
                        d.Remove(userLoggedIn);
                    }
                }
            }
            Response.Redirect("~/Login.aspx", false);
        }
        //Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
        //Version = config.AppSettings.Settings["Version"];
        Ulises5000Configuration.ToolsUlises5000Section ulisesVersion = Ulises5000Configuration.ToolsUlises5000Section.Instance;
        UlisesToolsVersion = ulisesVersion;

    }

    protected void LBGestionConfiguraciones_Click(object sender, EventArgs e)
    {
        Response.Redirect("~/Configuracion/Inicio.aspx");
    }

}
