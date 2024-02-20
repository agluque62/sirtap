using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Linq;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Text.RegularExpressions;
using System.Web.UI.HtmlControls;
using System.Web.Configuration;
using System.Collections.Generic;
using log4net;
using log4net.Config;

public partial class MisionesAsignacion : PageBaseCD40.PageCD40	//	System.Web.UI.Page
{

    static ServiciosCD40.ServiciosCD40 ServicioCD40 = new ServiciosCD40.ServiciosCD40();
    private Ulises5000Configuration.ToolsUlises5000Section UlisesToolsVersion;
    private static ServiciosCD40.Tablas[] datos;
    static bool PermisoSegunPerfil = false;
    protected static uint _IdMision = 0;

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

    protected new void Page_Load(object sender, EventArgs e)
    {
        base.Page_Load(sender, e);
        cMsg = (Mensajes.msgBox)this.Master.FindControl("MsgBox1");
        if (Context.Request.IsAuthenticated)
        {
            FormsIdentity ident = (FormsIdentity)Context.User.Identity;
            string perfil = ident.Ticket.UserData;
            if (perfil == "0")
            {
                Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO");
                return;
            }

            PermisoSegunPerfil = (perfil == "3" || perfil == "2");
            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            Ulises5000Configuration.ToolsUlises5000Section UlisesTools = Ulises5000Configuration.ToolsUlises5000Section.Instance;
            UlisesToolsVersion = UlisesTools;
        }
        if (!IsPostBack)
        {
            IndexListBox1 = -1;
            BtAceptar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "AceptarCambios");
            BtCancelar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "CancelarCambios");
            if (Session["IdentificadorMision"] != null && !string.IsNullOrEmpty((string)Session["IdentificadorMision"]))
            {
                NewItem = (string)Session["IdentificadorMision"];
                Session["IdentificadorMision"] = null;
            }
            MuestraDatos(DameDatos());
            GestionaOpcionesMenu("INICIO");
            ActualizaWebPadre(true);
        }
    }

    private ServiciosCD40.Tablas[] DameDatos()
    {
        try
        {
            ServiciosCD40.Misiones ms = new ServiciosCD40.Misiones();
            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(ms);
            datos = d;
            return d;
        }
        catch (Exception e)
        {
            logDebugView.Error("(MisionesAsignacion-DameDatos):", e);
        }
        return null;
    }
    private void MuestraDatos(ServiciosCD40.Tablas[] ms)
    {
        ListBox1.Items.Clear();
        if (ms != null)
        {
            for (int i = 0; i < ms.Length; i++)
            {
                ListItem mision = new ListItem();
                mision.Text = ((ServiciosCD40.Misiones)ms[i]).Descripcion;
                ListBox1.Items.Add(mision);
                ListBox1.Items[i].Value = ((ServiciosCD40.Misiones)ms[i]).IdMision.ToString();
            }
            if (ListBox1.Items.Count > 0)
            {
                ActualizaWebPadre(true);

                if (ListBox1.Items.FindByText(NewItem) != null)
                {
                    ListBox1.Items.FindByText(NewItem).Selected = true;
                    IndexListBox1 = ListBox1.SelectedIndex;
                    NewItem = string.Empty;
                }
                else
                    ListBox1.SelectedIndex = IndexListBox1 != -1 && IndexListBox1 < ListBox1.Items.Count ? IndexListBox1 : 0;

                MostrarElemento();

                GestionaOpcionesMenu("INICIO");
                ModificaMisionAsignacion(false);
                BtModificar.Visible = BtEliminar.Visible = PermisoSegunPerfil;

            }
            else
            {
                GestionaOpcionesMenu("INICIO");
                BtModificar.Visible = BtEliminar.Visible = false;
                cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "AvisoNoExistenMisiones")));
            }
        }
    }

    private void MostrarElemento()
    {
        if (ListBox1.Items.Count > 0 && null != datos)
        {
            int elemento = ListBox1.SelectedIndex;
            TxtMision.Text = ((ServiciosCD40.Misiones)datos[elemento]).Descripcion;
            DListTipo.SelectedValue = ((ServiciosCD40.Misiones)datos[elemento]).Tipo.ToString();
            ObtenOperadoresNoAsignadosAMisiones();
            ObtenOperadoresAsignadoAMision(_IdMision);           
        }
    }

    protected void ListBox1_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (ListBox1.SelectedIndex >= 0)
        {
            _IdMision = Convert.ToUInt32(ListBox1.SelectedValue);
            BtEliminar.Enabled = PermisoSegunPerfil;
            MostrarElemento();
            BtModificar.Visible = PermisoSegunPerfil;
            BtAceptar.Visible = false;
            BtCancelar.Visible = false;
        }
    }

    private void ObtenOperadoresAsignadoAMision(uint IdMision)
    {
        string operador = "0"; // No asignado
        ServiciosCD40.Misiones_Operadores oper = new ServiciosCD40.Misiones_Operadores();
        oper.IdMision = IdMision;
        ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(oper);
        ListOperadoresAsignados.Items.Clear();
        if (d.Length > 0)
        {
            for (int i = 0; i < d.Length; i++)
            {
                operador = ((ServiciosCD40.Misiones_Operadores)d[i]).IdOperador;
                ListOperadoresAsignados.Items.Add(((ServiciosCD40.Misiones_Operadores)d[i]).IdOperador);
                ListOperadoresAsignados.Items[ListOperadoresAsignados.Items.Count - 1].Value = ((ServiciosCD40.Misiones_Operadores)d[i]).IdOperador;
            }
        }
    }

    private void ObtenOperadoresNoAsignadosAMisiones()
    {
        ListOperadoresNoAsignados.Items.Clear();
        DataSet d = ServicioCD40.GetOperadoresNoAsignadosMisiones();
        if (d != null && d.Tables.Count > 0)
        {
            foreach (System.Data.DataRow ds in d.Tables[0].Rows)
            {
                ListOperadoresNoAsignados.Items.Add((string)ds["IdOperador"]);
                ListOperadoresNoAsignados.Items[ListOperadoresNoAsignados.Items.Count - 1].Value = ((string)ds["IdOperador"]);
            }
        }
    }

    private void GestionaOpcionesMenu(string Opcion)
    {
        BtNuevo.Visible = BtNuevo.Enabled = false;
        if (Opcion == "MODIFICA" || Opcion == "ELIMINAR")
        {
            BtAceptar.Visible = BtCancelar.Visible = true;
            BtModificar.Visible =  BtEliminar.Visible = ListBox1.Enabled = false;
            TxtMision.Enabled = false;
        }
        else if (Opcion == "INICIO")
        {
            BtAceptar.Visible = BtCancelar.Visible = false;
            BtModificar.Visible = BtEliminar.Visible = ListBox1.Enabled = (ListBox1.Items.Count > 0);
            TxtMision.Enabled = false;
        }
    }

    protected void IButAsignar_Click(object sender, ImageClickEventArgs e)
    {
        if (ListOperadoresNoAsignados.SelectedIndex >= 0)
            for (int i = 0; i < ListOperadoresNoAsignados.Items.Count; i++)
                if (ListOperadoresNoAsignados.Items[i].Selected)
                {
                    ListItem lbItem = new ListItem();
                    lbItem.Value = ListOperadoresNoAsignados.Items[i].Value;
                    ListOperadoresAsignados.Items.Add(lbItem);
                    int index = ListOperadoresNoAsignados.SelectedIndex;
                    ListOperadoresNoAsignados.Items.RemoveAt(index);
                    i--;
                }
    }

    protected void IButQuitar_Click(object sender, ImageClickEventArgs e)
    {
        int index;
        if (ListOperadoresAsignados.SelectedIndex >= 0)
            for (int i = 0; i < ListOperadoresAsignados.Items.Count; i++)
                if (ListOperadoresAsignados.Items[i].Selected)
                {
                    ListItem lbItem = new ListItem();
                    lbItem.Value = ListOperadoresAsignados.Items[i].Value;
                    ListOperadoresNoAsignados.Items.Add(lbItem);
                    index = ListOperadoresAsignados.SelectedIndex;
                    ListOperadoresAsignados.Items.RemoveAt(index);
                    i--;
                }
    }
    protected void BtAceptar_Click(object sender, EventArgs e)
    {
        GuardarCambios();
    }

    private void GuardarCambios()
    {
        try
        {
            Page.ClientScript.RegisterStartupScript(this.GetType(), "displaySloader", "displaySloader();", true);
            ServiciosCD40.Misiones_Operadores ms = new ServiciosCD40.Misiones_Operadores();
             ms.IdMision = _IdMision = Convert.ToUInt32(ListBox1.SelectedValue);
            
            if (ServicioCD40.DeleteSQL(ms) < 0)
            {
                logDebugView.Warn("(MisionesAsignaciones-EliminarElemento): No se ha podido eliminar los operadores asignados.");
                cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "ErrorEliminarMision"), TxtMision.Text));
            }
            else
            {
                for (int i = 0; i < ListOperadoresAsignados.Items.Count; i++)
                    {
                        ms = new ServiciosCD40.Misiones_Operadores();
                        ms.IdMision = _IdMision;
                        ms.IdOperador = ListOperadoresAsignados.Items[i].Value;
                        ms.Seguro = false;
                        ServicioCD40.InsertSQL(ms);
                    }
            }
            NewItem = TxtMision.Text;
            MuestraDatos(DameDatos());
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Misiones-GuardarCambios):", ex);
        }
    }

    protected void BtEliminar_Click(object sender, EventArgs e)
    {
        if (ListBox1.SelectedValue != "")
        {
            EliminarMisionAsignacion();
        }
    }

    private void EliminarMisionAsignacion()
    {
        ServiciosCD40.Misiones_Operadores ms = new ServiciosCD40.Misiones_Operadores();

        ms.IdMision = Convert.ToUInt32(ListBox1.SelectedValue);

        if (ServicioCD40.DeleteSQL(ms) < 0)
        {
            logDebugView.Warn("(MisionesAsignacion-EliminarMisionAsignacion): No se ha podido eliminar las asignaciones.");
            cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "ErrorEliminarMision"), TxtMision.Text));
        }
        else
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ElementoEliminado"));
        }
        MuestraDatos(DameDatos());
    }

    protected void BtNuevo_Click(object sender, EventArgs e)
    {
        // No hay creación de asignación si no existe misión...
    }

    protected void BtCancelar_Click(object sender, EventArgs e)
    {
        GestionaOpcionesMenu("INICIO");
        MuestraDatos(DameDatos());
    }

    protected void BtModificar_Click(object sender, EventArgs e)
    {
        GestionaOpcionesMenu("MODIFICA");
        ModificaMisionAsignacion(true);
    }

    private void ModificaMisionAsignacion(bool habilita)
    {
        ListOperadoresAsignados.Enabled = ListOperadoresNoAsignados.Enabled = habilita;
        IButAsignar.Enabled = IButQuitar.Enabled = habilita;
    }
}