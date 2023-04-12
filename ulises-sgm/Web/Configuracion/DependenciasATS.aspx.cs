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
using System.Web.Configuration;
using System.Collections.Generic;
using log4net;
using log4net.Config;

public partial class DependenciasATS : PageBaseCD40.PageCD40	
{
    private static ServiciosCD40.Tablas[] datos;
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
    private static bool Modificando;


    protected new void Page_Load(object sender, EventArgs e)
    {
        base.Page_Load(sender, e);

        cMsg = (Mensajes.msgBox)this.Master.FindControl("MsgBox1");

        if (Context.Request.IsAuthenticated)
        {
            if (((FormsIdentity)Context.User.Identity).Ticket.Name != "*CD40*")
            {
                Response.Redirect("~/Configuracion/Inicio.aspx", false);
            }

            // retrieve user's identity from httpcontext user 
            FormsIdentity ident = (FormsIdentity)Context.User.Identity;
            string perfil = ident.Ticket.UserData;
            if (perfil != "3")	// Sólo el perfil 3 (Técnico 3) tiene acceso a Dependencias ATS.
            {
                Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO", false);
                return;
            }
        }

        if (!IsPostBack)
        {
            BtnAceptar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "AceptarCambios");
            BtnCancelar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "CancelarCambios");
            //Se lee la variable de sesión idsistema
            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            Session["idsistema"] = s.Value;

            IndexListBox1 = -1;

            MuestraDatos(DameDatos());
        }
        else
        {

            if (datos == null)
            {
                //Si se recarga la pagina y el vector datos es null lo recargamos
                //Dentro de la función DameDatos, se lee la variable de sesión Session["idsistema"] del fichero Web.config
                DameDatos();
            }
        }
    }


    private ServiciosCD40.Tablas[] DameDatos()
    {
        try
        {
            ServiciosCD40.DependenciasATS t = new ServiciosCD40.DependenciasATS();
            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            t.IdSistema = s.Value;
            Session["idsistema"] = s.Value;

            ServiciosCD40.Tablas[] d = Servicio.ListSelectSQL(t);
            datos = d;
            return d;
        }
        catch (Exception e)
        {
            logDebugView.Error("(DependenciasATS-DameDatos):", e);
        }
        return null;
    }


    private void MuestraDatos(ServiciosCD40.Tablas[] nu)
    {
        LBDependenciasATS.Items.Clear();
        if (nu != null)
            for (int i = 0; i < nu.Length; i++)
                LBDependenciasATS.Items.Add(((ServiciosCD40.DependenciasATS)nu[i]).IdDependenciaATS);

        if (LBDependenciasATS.Items.Count > 0)
        {
            if (LBDependenciasATS.Items.FindByText(NewItem) != null)
                LBDependenciasATS.Items.FindByText(NewItem).Selected = true;
            else
                LBDependenciasATS.SelectedIndex = IndexListBox1 != -1 && IndexListBox1 < LBDependenciasATS.Items.Count ? IndexListBox1 : 0;

            BtnModificar.Visible = BtnEliminar.Visible = true;
            MostrarElemento();
            BtnEliminar_ConfirmButtonExtender.ConfirmText = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarDependenciaATS"), LBDependenciasATS.SelectedValue);
        }
        else
            BtnModificar.Visible = BtnEliminar.Visible = false;
    }


    private void MostrarElemento()
    {
        if (LBDependenciasATS.Items.Count > 0 && null != datos)
        {
            ActualizaWebPadre(true);

            int elemento = LBDependenciasATS.SelectedIndex;
            TBDependencia.Text = ((ServiciosCD40.DependenciasATS)datos[elemento]).IdDependenciaATS;
        }
    }


    protected void BtnAceptar_Click(object sender, EventArgs e)
    {
        if ((TBDependencia.Text.Length < 1) || TBDependencia.Text.Contains(GetLocalResourceObject("MSGDatoInvalido").ToString()))
        {
            TBDependencia.Text = GetLocalResourceObject("MSGDatoInvalido").ToString();
        }
        else
        {
            if (Modificando)
                ModificarDependenciaATS();
            else
                AnadirDependenciaATS();
        }
    }


    protected void BtnNuevo_Click(object sender, EventArgs e)
    {
        Modificando = false;
        Panel1.Enabled = true;
        BtnAceptar.Visible = BtnCancelar.Visible = true;
        BtnEliminar.Visible = BtnNuevo.Visible = BtnModificar.Visible = false;
        LBDependenciasATS.Enabled = false;
        TBDependencia.Enabled = true;
        ResetItems();
    }


    private void ResetItems()
    {
        if (!Modificando)
            TBDependencia.Text = string.Empty;
    }


    protected void BtnModificar_Click(object sender, EventArgs e)
    {
        Modificando = true;
        Panel1.Enabled = true;
        BtnAceptar.Visible = BtnCancelar.Visible = true;
        BtnEliminar.Visible = BtnNuevo.Visible = BtnModificar.Visible = false;
        LBDependenciasATS.Enabled = false;
        TBDependencia.Enabled = true;
        IndexListBox1 = LBDependenciasATS.SelectedIndex;
    }


    protected void BtnEliminar_Click(object sender, EventArgs e)
    {
        EliminaDependenciaATS();
    }


    private void EliminaDependenciaATS()
    {
        ServiciosCD40.DependenciasATS da = new ServiciosCD40.DependenciasATS();
        da.IdSistema = (string)Session["idsistema"];
        da.IdDependenciaATS = TBDependencia.Text;

        if (Servicio.DeleteSQL(da) > 0)
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ElementoEliminado"));
            TerminaActualizacion();
        }
    }


	private void AnadirDependenciaATS()
	{
        ServiciosCD40.DependenciasATS da = new ServiciosCD40.DependenciasATS();
		da.IdSistema = (string)Session["idsistema"];
        da.IdDependenciaATS = TBDependencia.Text;
        NewItem = TBDependencia.Text;
        if (Servicio.InsertSQL(da) > 0)
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "DependenciaATSDadaDeAlta"));
            TerminaActualizacion();
            ActualizaWebPadre(true);
        }
        else
        {
            TBDependencia.Text = GetLocalResourceObject("MSGDatoInvalido").ToString();
        }
	}


	private void ModificarDependenciaATS()
	{
        ServiciosCD40.DependenciasATS da = new ServiciosCD40.DependenciasATS();
		da.IdSistema = (string)Session["idsistema"];
        da.IdDependenciaATS = LBDependenciasATS.SelectedValue;
        da.IdDependenciaATSNew = TBDependencia.Text;

        IndexListBox1 = LBDependenciasATS.SelectedIndex;

        if (Servicio.UpdateSQL(da) > 0)
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "DependenciaATSModificada"));
            TerminaActualizacion();
            ActualizaWebPadre(true);
        }
        else
        {
            TBDependencia.Text = GetLocalResourceObject("MSGDatoInvalido").ToString();
        }
	}


    private void TerminaActualizacion()
    {
        LBDependenciasATS.Enabled = true;
        LblErrorConfirmacion.Visible = false;
        LblMensajeConfirmacion.Visible = false;
        Modificando = false;
        Panel1.Enabled = false;
        BtnEliminar.Visible = BtnEliminar.Visible = LBDependenciasATS.Items.Count > 0;
        BtnNuevo.Visible = true;
        BtnAceptar.Visible = BtnCancelar.Visible = false;
        MuestraDatos(DameDatos());
    }


    protected void LBDependenciasATS_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (LBDependenciasATS.SelectedIndex >= 0)
        {
            BtnEliminar_ConfirmButtonExtender.ConfirmText = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarDependenciaATS"), LBDependenciasATS.SelectedValue);
            MostrarElemento();
        }
    }


    protected void BtnCancelar_Click(object sender, EventArgs e)
    {
        CancelarDependenciaATS();
        cMsg.confirm((string)GetGlobalResourceObject("Espaniol", "CancelarCambios"), "cancelaElemento");
    }


    private void CancelarDependenciaATS()
    {
        if (!Modificando)
        {
            if (LBDependenciasATS.Items.Count > 0)
                LBDependenciasATS.SelectedIndex = IndexListBox1 != -1 ? IndexListBox1 : 0;
            MostrarElemento();
        }
        TerminaActualizacion();
    }


}