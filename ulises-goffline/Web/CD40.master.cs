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
using ServiciosCD40;

public partial class CD40 : System.Web.UI.MasterPage
{
    static ServiciosCD40.ServiciosCD40 serviceServiciosCD40;
    static private bool bSinTelefoniaATS = false; // Indica si no se deben mostar las opciones de telefonía ATS (troncales y encaminamientos)
    static private bool bAccesoInfOperadores = false; // Indica si se debe mostrar el informe de operadores
    static private bool bAccesoDependenciasATS = false; // Indica si se debe mostrar la gestión de dependencias ATS
    //20211130 #2851
    static private bool bAccesoBackupRestore = false; // Indica si se debe mostrar la gestión de Backup/Restore
    // RQF-33 
    static private bool bSistemaEnConfiguracionOffline = false;

	protected void Page_Load(object sender, EventArgs e)
	{
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

			// retrieve user's identity from httpcontext user 
			FormsIdentity ident = (FormsIdentity)Context.User.Identity;
            string strPerfil = ident.Ticket.UserData;
            switch (strPerfil)
			{
				case "0":
					break;
				case "1":
				case "2":
					break;
				case "3":
					break;
                case "4":
                    break;
				default:
					break;
			}

            LabelPerfil.Text = strPerfiles[Convert.ToInt16(strPerfil)];
			LabelUsuario.Text = ident.Name;

            if (false == IsPostBack)
            {
                if (bSistemaEnConfiguracionOffline)
                {
                    bAccesoBackupRestore = false;
                    LabelOffline.Visible = true;
                    LblEsquema.Visible = true;
                    LbIdEsquema.Visible = true;
                }

            }
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
        else if (!bAccesoInfOperadores)
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
    }


	protected void BtInicio_Click(object sender, EventArgs e)
	{
        string url = "~/Default.aspx";
        Response.Redirect(url, false);
	}

	private bool GestionarUltimasModificaciones()
	{
        Session["UserLoggedIn"] = LabelUsuario.Text;


		FormsAuthentication.Initialize();
		HttpContext context1 = HttpContext.Current;
		HttpCookie cookie1 = new HttpCookie(FormsAuthentication.FormsCookieName, "");
		cookie1.Path = FormsAuthentication.FormsCookiePath;
		cookie1.Expires = new DateTime(0x7cf, 10, 12);
		cookie1.Secure = FormsAuthentication.RequireSSL;
		context1.Response.Cookies.Remove(FormsAuthentication.FormsCookieName);
		context1.Response.Cookies.Add(cookie1);

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
		return true;
	}


	protected void LinkButton1_Click(object sender, EventArgs e)
	{
		GestionarUltimasModificaciones();
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

