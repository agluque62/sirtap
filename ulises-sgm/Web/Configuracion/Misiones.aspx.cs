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


public partial class Misiones : PageBaseCD40.PageCD40	//	System.Web.UI.Page
{
    private static bool Editando = false;
    private static bool Modificando = false;
    static bool PermisoSegunPerfil = false;
    private static KeyValueConfigurationElement s;
    private Ulises5000Configuration.ToolsUlises5000Section UlisesToolsVersion;
    private static ServiciosCD40.ServiciosCD40 ServicioCD40 = new ServiciosCD40.ServiciosCD40();
    private static ServiciosCD40.Tablas[] datos; 
    private static ServiciosCD40.Tablas[] datosInternos;
    private static ServiciosCD40.Tablas[] datosExternos;
    private static ServiciosCD40.Tablas[] datosLCext;
    private const int TIPO_RADIO = 0;
    private const int TIPO_TELEFONIA = 1;
    private const int TIPO_LCEN = 2;
    protected static uint _IdMision = 0;
    protected static bool _Seguro = false;

    // protected static bool[]  paginaSeguraRadio = new bool[POS_FIN_BOTON_PAGINA_RD + 1];
    // protected static bool[]  paginaSeguraTelefonia = new bool[POS_FIN_BOTON_PAGINA_TLF + 1];

    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected new void Page_Load(object sender, EventArgs e)
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
                Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO");
                return;
            }

            PermisoSegunPerfil = (perfil == "3");
            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            Ulises5000Configuration.ToolsUlises5000Section UlisesTools = Ulises5000Configuration.ToolsUlises5000Section.Instance;
            UlisesToolsVersion = UlisesTools;
        }

        if (!IsPostBack)
        {
            IndexListBox1 = -1;
            BtNuevo.Visible = PermisoSegunPerfil;
            BtAceptar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "AceptarCambios");
            BtCancelar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "CancelarCambios");
            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            s = config.AppSettings.Settings["Sistema"];
            Session["idsistema"] = s.Value;
            if (Session["IdentificadorMision"] != null && !string.IsNullOrEmpty((string)Session["IdentificadorMision"]))
            {
                NewItem = (string)Session["IdentificadorMision"];
                Session["IdentificadorMision"] = null;
            }
            PreparaPaginaMisiones();
            PresentaPosicionesPanelRadio();
            CargaDestinosTelefonia();
            MuestraPosicionesPanelTelefonia();
            MuestraDatos(DameDatos());
            GestionaOpcionesMenu("INICIO");
            ActualizaWebPadre(true);
        }
        else
        {
            if (datos == null)
            {
                DameDatos();
            }
        }
    }

    private void PreparaPaginaMisiones()
    {
        // paginaSeguraRadio = Enumerable.Repeat(true, (int)(POS_FIN_BOTON_PAGINA_RD + 1)).ToArray();
        // paginaSeguraTelefonia = Enumerable.Repeat(true, (int)(POS_FIN_BOTON_PAGINA_TLF + 1)).ToArray();
        PanelRadioPresentaCheckPaginasSeleccionadas(false);
        PanelTelefoniaPresentaCheckPaginasSeleccionadas(false);
        CargaParametrosPanel();
        PresentaInicioCabecerasPagina();
        CargaAlarmasAcusticasExistentes();
        CargaLCExistentes();
        MuestraLCExistentes();
    }

    private void PresentaInicioCabecerasPagina()
    {
        // Radio
        Session["PaginaRadMS"] = 1;
        numPagActual = 1;
        LabelPag.Text = (string)GetGlobalResourceObject("Espaniol", "Pagina") + " " + numPagActual.ToString();
        // Telefonía
        prefijosPosiciones = new uint[(NumPaginasT * NumPosicionesPagT) + 1];
        Session["PaginaTF"] = 1;
        numPagActualT = 1;
        LabelPagT.Text = (string)GetGlobalResourceObject("Espaniol", "Pagina") + " " + numPagActualT.ToString();
        PanelSelLC.Visible = false; // 20240227
    }

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

    #region GESTIÓN DE MISIONES

    private ServiciosCD40.Tablas[] DameDatos()
    {
        try
        {
            ServiciosCD40.Misiones ms = new ServiciosCD40.Misiones();
            ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(ms);
            datos = d;
            return d;
        }
        catch (Exception e)
        {
            logDebugView.Error("(Misiones-DameDatos):", e);
        }
        return null;
    }

    private void MostrarElemento()
    {
         if (ListBox1.Items.Count > 0 && null != datos)
        {
            ActualizaWebPadre(true);
            int elemento = ListBox1.SelectedIndex;
            DListTipo.Enabled = false;
            GestionaOpcionesMenu("INICIO");
            PresentaPanelesMostrarElemento();
            TxtMision.Text = ((ServiciosCD40.Misiones)datos[elemento]).Descripcion;
            _IdMision = ((ServiciosCD40.Misiones)datos[elemento]).IdMision;
            DListTipo.SelectedValue = ((ServiciosCD40.Misiones)datos[elemento]).Tipo.ToString();
            LabelMSAS.Visible = HayOperadoresAsignadosAMision();
            _Seguro = false;// ((ServiciosCD40.Misiones)datos[elemento]).Seguro;
            ImagenSegura.Visible = false;// _Seguro;
            ImagenNoSegura.Visible = false; //  !_Seguro;
            CargaAlarmasAcusticasAsignadas(_IdMision);
            PresentaPaginasSeleccionadas(_IdMision);
            PresentaBotonesLineaCalienteSeleccionados(_IdMision);
        }
    }

    private void PresentaPanelesMostrarElemento()
    {
        PresentaInicioCabecerasPagina();
        MostrarRadio();
        MostrarInternos();
        MostrarExternos();
    }

    private void PresentaPaginasSeleccionadas(uint IdMision)
    {
        try
        {
            InicializaGestionPaginasSeleccionadas();
            ServiciosCD40.Misiones_Paginas mp = new ServiciosCD40.Misiones_Paginas();
            mp.IdMision = IdMision;
            mp.TipoDestino = 99; // Para pedir todos los tipos.
            ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(mp);
            if (d != null)
            {
                for (int i = 0; i < d.Length; i++)
                {
                    if (((ServiciosCD40.Misiones_Paginas)d[i]).TipoDestino == TIPO_RADIO)
                    {
                        PanelRadioGestionaCheckPaginasSeleccionadas((int)((ServiciosCD40.Misiones_Paginas)d[i]).IdPagina, true);
                        // paginaSeguraRadio[(int)((ServiciosCD40.Misiones_Paginas)d[i]).IdPagina] = ((ServiciosCD40.Misiones_Paginas)d[i]).Seguro;
                    }
                    else if (((ServiciosCD40.Misiones_Paginas)d[i]).TipoDestino == TIPO_TELEFONIA) 
                    {
                        PanelTelefoniaGestionaCheckPaginasSeleccionadas((int)((ServiciosCD40.Misiones_Paginas)d[i]).IdPagina, true);
                        // paginaSeguraTelefonia[(int)((ServiciosCD40.Misiones_Paginas)d[i]).IdPagina] = ((ServiciosCD40.Misiones_Paginas)d[i]).Seguro;
                    }
                }
            }
        }
        catch (Exception e)
        {
            logDebugView.Error("(Misiones-PresentaPaginasSeleccionadas):", e);
        }
    }

    private void InicializaGestionPaginasSeleccionadas()
    {
       // paginaSeguraRadio = Enumerable.Repeat(true, (int)(POS_FIN_BOTON_PAGINA_RD + 1)).ToArray();
        PanelRadioPresentaCheckPaginasSeleccionadas(false);
        PanelTelefoniaPresentaCheckPaginasSeleccionadas(false);
      // paginaSeguraTelefonia = Enumerable.Repeat(true, (int)(POS_FIN_BOTON_PAGINA_TLF + 1)).ToArray();
    }

    private void PresentaBotonesLineaCalienteSeleccionados(uint IdMision)
    {
        MuestraLCMision(IdMision);
    }

    protected void ActualizaMisionPaginasSeleccionadas()
    {
        try
        {
            ServiciosCD40.Misiones_Paginas mp = new ServiciosCD40.Misiones_Paginas();
            mp.IdMision = _IdMision;
            ServicioCD40.DeleteSQL(mp);
            for (uint ind = POS_INI_BOTON_PAGINA; ind < POS_FIN_BOTON_PAGINA_RD + 1; ind++)
            {
                CheckBox imagenpaginas = (CheckBox)PanelRadio.FindControl("CBR" + ind);
                if (imagenpaginas != null)
                {
                    if (imagenpaginas.Checked)
                    {
                        mp = new ServiciosCD40.Misiones_Paginas();
                        mp.IdMision = _IdMision;
                        mp.IdPagina = ind;
                        mp.TipoDestino = TIPO_RADIO;
                        mp.Seguro = false; // paginaSeguraRadio[ind];
                        ServicioCD40.InsertSQL(mp);
                    }
                }
            }
            for (uint ind = POS_INI_BOTON_PAGINA; ind < POS_FIN_BOTON_PAGINA_TLF + 1; ind++)
            {
                CheckBox imagenpaginas = (CheckBox)PanelTelefonia.FindControl("CBT" + ind);
                if (imagenpaginas != null)
                {
                    if (imagenpaginas.Checked)
                    {
                        mp = new ServiciosCD40.Misiones_Paginas();
                        mp.IdMision = _IdMision;
                        mp.IdPagina = ind;
                        mp.TipoDestino = TIPO_TELEFONIA;
                        mp.Seguro = false;//paginaSeguraTelefonia[ind];
                        ServicioCD40.InsertSQL(mp);
                    }
                }
            }
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Misiones-ActualizaMisionPaginasSeleccionadas):", ex);
        }
    }

    private void ActualizaClasificacionMision()
    {
        /* *
        uint indpagina = 0;
        for (indpagina = POS_INI_BOTON_PAGINA; indpagina < (POS_FIN_BOTON_PAGINA_RD + 1); indpagina++)
        {
            if (paginaSeguraRadio[indpagina] == false)
            {
                _Seguro = false;
                break;
            }
        }
        if (_Seguro)
            for (indpagina = POS_INI_BOTON_PAGINA; indpagina < (POS_FIN_BOTON_PAGINA_TLF + 1); indpagina++)
            {
             if (paginaSeguraTelefonia[indpagina] == false)
                 _Seguro = false;
             break;
            }
        ImagenSegura.Visible = _Seguro;
        ImagenNoSegura.Visible = !_Seguro;
         * */
    }


    #endregion GESTIÓN DE MISIONES

    protected void ListBox1_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (ListBox1.SelectedIndex >= 0)
        {
            BtEliminar.Enabled = PermisoSegunPerfil;

            MostrarElemento();            
            BtModificar.Visible = PermisoSegunPerfil;
            BtAceptar.Visible = false;
            BtCancelar.Visible = false;


        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="nu"></param>
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
                BtNuevo.Visible = BtModificar.Visible = BtEliminar.Visible = PermisoSegunPerfil;

            }
            else
            {
                InicializaDatosMision();
                GestionaOpcionesMenu("INICIO");
                BtNuevo.Visible = PermisoSegunPerfil;
                BtModificar.Visible = BtEliminar.Visible = false;
            }
            DesBloqueaSeleccionElementos(false);
        }
    }

    private void DesBloqueaSeleccionElementos(bool desbloquea)
    {
        PanelRadioActivaSeleccionPagina(desbloquea);
        PanelTelefoniaActivaSeleccionPagina(desbloquea);
        PanelLineaCalienteActivaSeleccion(desbloquea);
        ActivaSeleccionTeclasLC(desbloquea);
        PanelAlarmasAcusticasActivaSeleccion(desbloquea);
        ActivaSeleccionAlarmasAcusticas(desbloquea);
        DListTipo.Enabled = desbloquea;
        PanelRadioActivaPasoPagina(true);
        PanelTelefoniaActivaPasoPagina(true); 
    }

    private void NuevaMision()
    {
        InicializaDatosMision();
        DesBloqueaSeleccionElementos(true);
        TxtMision.Enabled = true;
        InicializaAlarmasAcusticasAsignadas();
    }

    private void InicializaDatosMision()
    {
        DListTipo.SelectedValue = "0";
        ImagenSegura.Visible = false;
        ImagenNoSegura.Visible = false;
        TxtMision.Text = String.Empty;
        LabelMSAS.Visible = false;
        InicializaGestionPaginasSeleccionadas();
        InicializaLCEN();
    }

    private void ModificaMision()
    {
            DesBloqueaSeleccionElementos(true);
            TxtMision.Enabled = false;
    }

    private void GestionaOpcionesMenu(string Opcion)
    {
        if (Opcion == "ALTA")
        {
            BtAceptar.Visible = BtCancelar.Visible = true;
            BtModificar.Visible = BtNuevo.Visible = BtEliminar.Visible = ListBox1.Enabled = false;
            TxtMision.Enabled = true;
        }
        else if (Opcion == "MODIFICA" || Opcion == "ELIMINAR")
        {
            BtAceptar.Visible = BtCancelar.Visible = true;
            BtModificar.Visible = BtNuevo.Visible = BtEliminar.Visible = ListBox1.Enabled = false;
            TxtMision.Enabled = false;
        }
        else if (Opcion == "INICIO")
        {
            BtAceptar.Visible = BtCancelar.Visible = false;
            BtModificar.Visible = BtEliminar.Visible = ListBox1.Enabled = (ListBox1.Items.Count > 0);
            TxtMision.Enabled = false;
        }
    }

    private bool HayOperadoresAsignadosAMision()
    {
        bool asignados = false;
        ServiciosCD40.Misiones_Operadores oper = new ServiciosCD40.Misiones_Operadores();
        oper.IdMision = _IdMision;
        ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(oper);
        if (d.Length > 0)
        {
            asignados = true;
        }
        return asignados;
    }

    // Selección Tipo de Misión. Real/Entrenamiento.
    protected void DListTipo_SelectedIndexChanged(object sender, EventArgs e)
    {

    }

    private bool IdentificadorDuplicado()
    {
        bool existe = false;
        if (datos != null)
        {
            for (int i = 0; i < datos.Length; i++)
            {
                if (((ServiciosCD40.Misiones)datos[i]).Descripcion == TxtMision.Text)
                {
                    cMsg.confirm((string)GetLocalResourceObject("AvisoIdentificadorDuplicado"), "aceptparam");
                    existe = true;
                    break;
                }
            }
        }  
        return existe;
    }

    private bool IdentificadorNoValido()
    {
        bool novalido = false;
        if (!Regex.IsMatch(TxtMision.Text, @"^[\w-._]{1,32}$"))
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ErrorIdentificadorNoValido"));
            novalido = true;
        }

        return novalido;
    }

    protected void BtAceptar_Click(object sender, EventArgs e)
    {

        // Valida datos
        if (!Modificando)
        {
            // Identificador valido o Existe
            if (IdentificadorNoValido() || IdentificadorDuplicado())
                return;
        }
        GuardarCambios();

    }

    private void GuardarCambios()
    {
        try
        {
            Page.ClientScript.RegisterStartupScript(this.GetType(), "displaySloader", "displaySloader();", true);
            ServiciosCD40.Misiones ms = new ServiciosCD40.Misiones();
            ms.Seguro = _Seguro;
            ms.Tipo = Convert.ToUInt32(DListTipo.SelectedValue);
            if (!Modificando)
            {
                _IdMision = 0;
                ms.Descripcion = TxtMision.Text;
                ms.Seguro = _Seguro;
                ms.Tipo = Convert.ToUInt32(DListTipo.SelectedValue);
                ServicioCD40.InsertSQL(ms);
                ServiciosCD40.Tablas[] d =  ServicioCD40.ListSelectSQL(ms);
                if (d != null)
                {
                    _IdMision = ((ServiciosCD40.Misiones)d[0]).IdMision;
                }
            }
            ActualizaMisionPaginasSeleccionadas();
            ActualizaMisionLCENSeleccionadas();
            ActualizaMisionAlarmasAlertasAcusticas();
            NewItem = TxtMision.Text;
            MuestraDatos(DameDatos());
            Editando = false;
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Misiones-GuardarCambios):", ex);
        }

    }

    protected void BtCancelar_Click(object sender, EventArgs e)
    {
        GestionaOpcionesMenu("INICIO");
        MuestraDatos(DameDatos());
        Editando = false;
    }

    protected void BtModificar_Click(object sender, EventArgs e)
    {
        Editando = true;
        Modificando = true;
        GestionaOpcionesMenu("MODIFICA");
        ModificaMision();
    }

    protected void BtNuevo_Click(object sender, EventArgs e)
    {
        Editando = true;
        GestionaOpcionesMenu("ALTA");
        NuevaMision();
    }

    protected void BtEliminar_Click(object sender, EventArgs e)
    {
        if (ListBox1.SelectedValue != "")
        {       
            EliminarMision();            
        }
    }

    private void EliminarMision()
    {
            BtEliminar_ConfirmButtonExtender.ConfirmText = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarRecurso"), ListBox1.SelectedValue);
            ServiciosCD40.Misiones ms = new ServiciosCD40.Misiones();
            
            ms.IdMision = Convert.ToUInt32(ListBox1.SelectedValue);

            if (ServicioCD40.DeleteSQL(ms) < 0)
            {
                logDebugView.Warn("(Misiones-EliminarElemento): No se ha podido eliminar la misión.");
                cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "ErrorEliminarMision"), TxtMision.Text));
            }
            else
            {
                cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ElementoEliminado"));
            }
            MuestraDatos(DameDatos());
    }

    protected void CeldaEnlaceRadio_OnClick(object sender, EventArgs e)
    {

    }

    protected void CBSeguroRadio_OnCheckedChanged(object sender, EventArgs e)
    {

    }
    

    #region GESTION PÁGINAS RADIO
    private const uint NUM_COLUMNAS_FIJAS = 5;
    private const uint NUM_FILAS_FIJAS = 4;
    private static uint NumColumnasVisibles = NUM_COLUMNAS_FIJAS;
    private static uint NumFilasVisibles = NUM_FILAS_FIJAS;
    private const uint MAX_FREC_PAG = NUM_COLUMNAS_FIJAS * NUM_FILAS_FIJAS;
    private static uint numPagActual = 0;
    private static uint NumPaginas;
    private static uint NumPosicionesPag;
    private const uint POS_INI_BOTON_PAGINA = 1;
    private const uint POS_FIN_BOTON_PAGINA_RD = 28;

    private void PanelRadioPresentaCheckPaginasSeleccionadas(bool seleccionado)
    {
        for (uint ind = POS_INI_BOTON_PAGINA; ind < POS_FIN_BOTON_PAGINA_RD + 1; ind++)
        {
            CheckBox imagenpaginas = (CheckBox)PanelRadio.FindControl("CBR" + ind);
            if (imagenpaginas != null)
            {
                imagenpaginas.Visible = true;
                imagenpaginas.Checked = seleccionado;
            }
        }
    }

    private void PanelRadioGestionaCheckPaginasSeleccionadas(int ind, bool seleccionado)
    {
        if (ind < POS_INI_BOTON_PAGINA || ind > POS_FIN_BOTON_PAGINA_RD)
            return;
        CheckBox imagenpagina = (CheckBox)PanelRadio.FindControl("CBR" + ind);
        if (imagenpagina != null)
        {
            imagenpagina.Checked = seleccionado;
            //paginaSeguraRadio[ind] = CBSeguroRadio.Checked;
            //ActualizaClasificacionMision();
        }
    }

    private void PanelRadioActivaPasoPagina(bool activa)
    {
        IButPagAbajo.Enabled = activa;
        IButPagArriba.Enabled = activa;
    }

    private void PanelRadioActivaSeleccionPagina(bool activa)
    {
        IButRDAsignar.Enabled = activa;
        IButRDDesAsignar.Enabled = activa;
    }

    protected void IButRDAsignar_Click(object sender, ImageClickEventArgs e)
    {
        PanelRadioGestionaCheckPaginasSeleccionadas((int)numPagActual, true);
    }

    protected void IButRDDesAsignar_Click(object sender, ImageClickEventArgs e)
    {
        PanelRadioGestionaCheckPaginasSeleccionadas((int)numPagActual, false);
    }

    protected void IButPagArribaR_Click(object sender, ImageClickEventArgs e)
    {
        if (numPagActual < NumPaginas)
            numPagActual += 1;
        else
            numPagActual = 1;
        Session["PaginaRadMS"] = numPagActual;
        LabelPag.Text = (string)GetGlobalResourceObject("Espaniol", "Pagina") + " " + numPagActual.ToString();
        LimpiarPanel();
        IButRDAsignar.Enabled = (MostrarRadio() && Editando);
    }

    protected void IButPagAbajoR_Click(object sender, ImageClickEventArgs e)
    {
        if (numPagActual > 1)
            numPagActual -= 1;
        else
            numPagActual = (uint)NumPaginas;
        Session["PaginaRadMS"] = numPagActual;
        LabelPag.Text = (string)GetGlobalResourceObject("Espaniol", "Pagina") + " " + numPagActual.ToString();
        LimpiarPanel();
        IButRDAsignar.Enabled = (MostrarRadio() && Editando);
    }

    private void LimpiarPanel()
    {
        int visibleCount = 0;
        for (int i = 1; i <= NumFilasVisibles * NUM_COLUMNAS_FIJAS; i++)
        {
            TableCell tCell = (TableCell)TEnlacesRadio.FindControl("TableCell" + i.ToString());
            if (tCell != null)
            {
                int fila = (i - 1) / (int)NUM_COLUMNAS_FIJAS; //0..NUM_COLUMNAS_FIJAS
                int columna = (i - 1) % (int)NUM_COLUMNAS_FIJAS; //0..NUM_COLUMNAS_FIJAS
                if ((fila <= NumFilasVisibles) && (columna < NumColumnasVisibles))
                {
                    tCell.Visible = true;
                    if (++visibleCount <= NumPosicionesPag)
                        tCell.Enabled = true;
                    else
                    {
                        tCell.Enabled = false;
                        tCell.Visible = false;
                    }
                }
                else
                    tCell.Visible = false;

                Button ibut = (Button)TEnlacesRadio.FindControl("Button" + i.ToString());
                TextBox tbox = (TextBox)TEnlacesRadio.FindControl("TextBox" + i.ToString());
                ibut.CssClass = "BtnPanelRadioLibre";
                tbox.Text = "";
                ibut.ToolTip = String.Empty;
                tbox.ToolTip = String.Empty;
            }
        }
    }

    private static ServiciosCD40.Tablas[] datosRadio;

    private void PresentaPosicionesPanelRadio()
    {
        CargaRadio();
        LimpiarPanel();
        IButRDAsignar.Enabled = (MostrarRadio() && Editando);
    }

    private void CargaRadio()
    {
        try
        {
            ServiciosCD40.Tablas[] radio = ServicioCD40.DestinosRadioAsignadosAlSector((string)Session["idsistema"], Misiones.SectoresSirtap[0]); 
            datosRadio = radio;
        }
        catch (Exception ex)
        {
            logDebugView.Error("(TFTRadio-CargaRadio): ", ex);
        }
    }

    private bool MostrarRadio()
    {
        bool hayboton = false;
        try
        {
            uint posini = ((numPagActual - 1) * NumPosicionesPag) + 1;
            uint posfin = posini + NumPosicionesPag - 1;
            CBSeguroRadio.Checked = false;
            if (datosRadio != null)
            {
                for (int i = 0; i < datosRadio.Length; i++)
                {
                    uint pos = ((ServiciosCD40.DestinosRadioSector)datosRadio[i]).PosHMI;
                    if ((pos >= posini) && (pos <= posfin))
                    {
                        uint posenpanel = CalculatePosButton(pos);
                        Button ibut = (Button)TEnlacesRadio.FindControl("Button" + posenpanel.ToString());
                        TextBox tbox = (TextBox)TEnlacesRadio.FindControl("TextBox" + posenpanel.ToString());
                          ibut.CssClass = "BtnPanelRadioAsignado";
                        //En el tooltip del botón se muestra el identificador del destino y en el texto el literal
                        //ibut.ToolTip = ((ServiciosCD40.DestinosRadioSector)datosRadio[i]).IdDestino;
                        ibut.ToolTip = ((ServiciosCD40.DestinosRadioSector)datosRadio[i]).DescDestino; // RQF-34
                        tbox.Text = ((ServiciosCD40.DestinosRadioSector)datosRadio[i]).Literal;
                        tbox.ToolTip = ((ServiciosCD40.DestinosRadioSector)datosRadio[i]).IdDestino;
                        tbox.ValidationGroup = ((ServiciosCD40.DestinosRadioSector)datosRadio[i]).IdDestino;
                        CBSeguroRadio.Checked = ((ServiciosCD40.DestinosRadioSector)datosRadio[i]).Seguro;
                        hayboton = true;
                    }
                }
            }
        }
        catch (Exception ex)
        {
            logDebugView.Error("(TFTRadio-MostrarRadio): ", ex);
        }
        return hayboton;
    }

    private uint CalculatePosHmi(uint buttonIndex)
    {
        uint fila = ((uint)buttonIndex - 1) / NUM_COLUMNAS_FIJAS; //0..NUM_COLUMNAS_FIJAS
        uint columna = ((uint)buttonIndex - 1) % NUM_COLUMNAS_FIJAS; //0..NUM_COLUMNAS_FIJAS

        return fila * NumColumnasVisibles + columna + 1 + ((numPagActual - 1) * NumPosicionesPag);
    }

    private uint CalculatePosButton(uint posHmi)
    {
        //pos HMI 1...NumPosicionesPag*Num pag
        uint fila = ((posHmi - 1) % (uint)NumPosicionesPag) / NumColumnasVisibles;
        uint columna = ((posHmi - 1) % (uint)NumPosicionesPag) % NumColumnasVisibles; //0..NUM_COLUMNAS_FIJAS

        return fila * NUM_COLUMNAS_FIJAS + columna + 1;
    }

    private void CargaParametrosPanel()
    {
        try
        {
            ServiciosCD40.ParametrosSector t = new ServiciosCD40.ParametrosSector();
            t.IdSistema = (string)Session["idsistema"];
            t.IdSector = (string)Session["NombreSector"];
            t.IdNucleo = (string)Session["idnucleo"];
            ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(t);
            if (d.Length > 0)
            {
                NumPaginasT = ((ServiciosCD40.ParametrosSector)d[0]).NumPagDestinosInt;
                NumPosicionesPagT = ((ServiciosCD40.ParametrosSector)d[0]).NumDestinosInternosPag;
                NumPaginas = ((ServiciosCD40.ParametrosSector)d[0]).NumPagFrec;
                NumPosicionesPag = ((ServiciosCD40.ParametrosSector)d[0]).NumFrecPagina;
                if (NumPosicionesPag > 12)
                {
                    NumColumnasVisibles = 5;
                    if (NumPosicionesPag < 16) NumFilasVisibles = 3;
                }
                else if (NumPosicionesPag > 9)
                {
                    NumColumnasVisibles = 4;
                    NumFilasVisibles = 3;
                }
                else if (NumPosicionesPag > 4)
                {
                    NumColumnasVisibles = 3;
                    NumFilasVisibles = 3;
                }
                else
                {
                    NumColumnasVisibles = 2;
                    NumFilasVisibles = 2;
                }
            }
            prefijosPosicionesLC = new uint[NumPosicionesPag + 1];	// Las posiciones empiezan en 1
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Mision-CargaParametrosPanel): ", ex);
        }
    }
    #endregion 


    #region GESTIÓN PÁGINAS TELEFONÍA
    private const uint NUM_COLUMNAS_FIJAS_TELEFONIA = 5;
    private static uint NumColumnasVisiblesT = NUM_COLUMNAS_FIJAS_TELEFONIA;
    private static uint numPagActualT = 0;
    private static uint NumPaginasT;
    private static uint NumPosicionesPagT;
    private static uint[] prefijosPosiciones;
    private const uint POS_FIN_BOTON_PAGINA_TLF = 9;

    protected void IButPagTArriba_Click(object sender, ImageClickEventArgs e)
    {
        if (numPagActualT < NumPaginasT)
        {
            numPagActualT += 1;
        }
        else
        {
            numPagActualT = 1;
        }
        Session["PaginaTF"] = numPagActualT;
        LabelPagT.Text = (string)GetGlobalResourceObject("Espaniol", "Pagina") + " " + numPagActualT.ToString();
        CBSeguroTelefonia.Checked = false;
        LimpiarPanelTelefonia();
        bool hayInternos = MostrarInternos();
        bool hayExternos = MostrarExternos();
        if ((hayInternos || hayExternos) && Editando)
            IButTLFAsignar.Enabled = true;
        else
            IButTLFAsignar.Enabled = false;
    }

    protected void IButPagTAbajo_Click(object sender, ImageClickEventArgs e)
    {
        if (numPagActualT > 1)
        {
            numPagActualT -= 1;
        }
        else
        {
            numPagActualT = NumPaginasT;
        }
        Session["PaginaTF"] = numPagActualT;
        LabelPagT.Text = (string)GetGlobalResourceObject("Espaniol", "Pagina") + " " + numPagActualT.ToString();
        CBSeguroTelefonia.Checked = false;
        LimpiarPanelTelefonia();
        bool hayInternos = MostrarInternos();
        bool hayExternos = MostrarExternos();
        if ((hayInternos || hayExternos) && Editando)
            IButTLFAsignar.Enabled = true;
        else
            IButTLFAsignar.Enabled = false;
    }

    private void LimpiarPanelTelefonia()
    {
        uint numFilasT = (NumPosicionesPagT / NumColumnasVisiblesT) + 1;
        int visibleCount = 0;
        for (int i = 1; i < numFilasT * NUM_COLUMNAS_FIJAS_TELEFONIA; i++)
        {
            TableCell tCell = (TableCell)TEnlacesInternos.FindControl("TableCellTI" + i.ToString());
            if (tCell != null)
            {
                int fila = (i - 1) / (int)NUM_COLUMNAS_FIJAS_TELEFONIA; 
                int columna = (i - 1) % (int)NUM_COLUMNAS_FIJAS_TELEFONIA;
                if ((fila <= numFilasT) && (columna < NumColumnasVisiblesT))
                {
                    tCell.Visible = true;
                    if (++visibleCount <= NumPosicionesPagT)
                        tCell.Enabled = true;
                    else
                    {
                        tCell.Enabled = false;
                        tCell.Visible = false;
                    }

                }
                else
                    tCell.Visible = false;
                Button ibut = (Button)TEnlacesInternos.FindControl("ButtonTI" + i.ToString());
                if (ibut != null)
                {
                    ibut.CssClass = "BtnPanelRadioLibre";
                    ibut.Text = "";
                }
            }
        }
    }

    private bool MostrarInternos()
    {
        bool haybotones = false;
        uint posini = ((numPagActualT - 1) * NumPosicionesPagT) + 1;
        uint posfin = posini + NumPosicionesPagT - 1;

        if (datosInternos != null)
        {
            for (int i = 0; i < datosInternos.Length; i++)
            {
                uint posHmi = ((ServiciosCD40.DestinosInternosSector)datosInternos[i]).PosHMI;
                if ((posHmi >= posini) && (posHmi <= posfin))
                {
                    uint posenpanel = CalculatePosButtonT(posHmi);
                    Button ibut = (Button)TEnlacesInternos.FindControl("ButtonTI" + posenpanel.ToString());
                    ibut.CssClass = "BtnPanelTfAsignado";
                    ibut.Text = ((ServiciosCD40.DestinosInternosSector)datosInternos[i]).Literal;
                    prefijosPosiciones[posHmi] = ((ServiciosCD40.DestinosInternosSector)datosInternos[i]).IdPrefijo;
                    CBSeguroTelefonia.Checked = ((ServiciosCD40.DestinosInternosSector)datosInternos[i]).Seguro;
                    haybotones = true;
                }
            }
        }
        return haybotones;
    }

    private bool MostrarExternos()
    {
        bool haybotones = false;
        uint posini = ((numPagActualT - 1) * NumPosicionesPagT) + 1;
        uint posfin = posini + NumPosicionesPagT - 1;

        if (datosExternos != null)
        {
            for (int i = 0; i < datosExternos.Length; i++)
            {
                uint posHmi = ((ServiciosCD40.DestinosExternosSector)datosExternos[i]).PosHMI;

                if ((posHmi >= posini) && (posHmi <= posfin))
                {
                    uint posenpanel = CalculatePosButtonT(posHmi);
                    Button ibut = (Button)TEnlacesInternos.FindControl("ButtonTI" + posenpanel.ToString());
                    ibut.CssClass = "BtnPanelTfAsignado";
                    ibut.Text = ((ServiciosCD40.DestinosExternosSector)datosExternos[i]).Literal;
                    prefijosPosiciones[posHmi] = ((ServiciosCD40.DestinosExternosSector)datosExternos[i]).IdPrefijo;
                    CBSeguroTelefonia.Checked = ((ServiciosCD40.DestinosExternosSector)datosExternos[i]).Seguro;
                    haybotones = true;
                }
            }
        }
        return haybotones;
    }

    private void MuestraPosicionesPanelTelefonia()
    {
        LimpiarPanelTelefonia();
        MostrarInternos();       
        MostrarExternos();
    }

    private void CargaDestinosTelefonia()
    {
        CargaDestinosInternos();
        CargaDestinosExternos();
    }

    private void CargaDestinosInternos()
    {
        try
        {
            ServiciosCD40.Tablas[] internos = ServicioCD40.DestinosTelefoniaAsignadosAlSector((string)Session["idsistema"], Misiones.SectoresSirtap[0], true, true); 
            datosInternos = internos;
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Mision-CargaDestinosInternos): ", ex);
        }
    }

    private void CargaDestinosExternos()
    {
        try
        {
            ServiciosCD40.Tablas[] externos = ServicioCD40.DestinosTelefoniaAsignadosAlSector((string)Session["idsistema"], Misiones.SectoresSirtap[0], true, false);
            datosExternos = externos;
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Mision-CargaDestinosExternos): ", ex);
        }
    }

    protected void IButTLFAsignar_Click(object sender, ImageClickEventArgs e)
    {
        PanelTelefoniaGestionaCheckPaginasSeleccionadas((int)numPagActualT, true);
    }

    protected void IButTLFDesAsignar_Click(object sender, ImageClickEventArgs e)
    {
        PanelTelefoniaGestionaCheckPaginasSeleccionadas((int)numPagActualT, false);
    }

    private void PanelTelefoniaActivaPasoPagina(bool activa)
    {
        IButPagTAbajo.Enabled = activa;
        IButPagTArriba.Enabled = activa;
    }

    private void PanelTelefoniaActivaSeleccionPagina(bool activa)
    {
        IButTLFAsignar.Enabled = activa;
        IButTLFDesAsignar.Enabled = activa;
    }

    private void PanelTelefoniaPresentaCheckPaginasSeleccionadas(bool seleccionado)
    {
        for (uint ind = POS_INI_BOTON_PAGINA; ind < POS_FIN_BOTON_PAGINA_TLF + 1; ind++)
        {
            CheckBox imagenpaginas = (CheckBox)PanelTelefonia.FindControl("CBT" + ind);
            if (imagenpaginas != null)
            {
                imagenpaginas.Visible = true;
                imagenpaginas.Checked = seleccionado;
            }
        }
    }

    private void PanelTelefoniaGestionaCheckPaginasSeleccionadas(int ind, bool seleccionado)
    {
        if (ind < POS_INI_BOTON_PAGINA || ind > POS_FIN_BOTON_PAGINA_TLF)
            return;
        CheckBox imagenpagina = (CheckBox)PanelTelefonia.FindControl("CBT" + ind);
        if (imagenpagina != null)
        {
            imagenpagina.Checked = seleccionado;
            //paginaSeguraTelefonia[ind] = CBSeguroTelefonia.Checked;
            //ActualizaClasificacionMision();
        }

    }

    private uint CalculatePosButtonT(uint posHmi)
    {
        //pos HMI 1...NumPosicionesPag*Num pag
        uint fila = ((posHmi - 1) % (uint)NumPosicionesPagT) / NumColumnasVisiblesT;
        uint columna = ((posHmi - 1) % (uint)NumPosicionesPagT) % NumColumnasVisiblesT; //0..NUM_COLUMNAS_FIJAS

        return fila * NUM_COLUMNAS_FIJAS_TELEFONIA + columna + 1;
    }
    #endregion


    #region GESTIÓN DE LINEA CALIENTE
    private const int PREFIJO_DESTINO_LCI = 0;
    private const int PREFIJO_DESTINO_LCE = 1;
    private const int PREFIJO_DESTINO_ATS = 2;
    private const int PREFIJO_DESTINO_ATS_BIS = 3;
    private static uint[] prefijosPosicionesLC;

    protected void PulsaEnlaceLineaCaliente_OnClick(object sender, EventArgs e)
    {
        Button ibut = (Button)TEnlacesLC.FindControl(((Button)sender).ID);

        if (ibut.CssClass == "BtnPanelRadioLibre")
        {
            BtLiberarLC.Enabled = false;

            //El botón Asignar sólo debe estar habilitado si el botón seleccionado del panel está libre y
            //se ha seleccionado algún destino de la lista
            if (LBLCExistentes.SelectedIndex >= 0)
            {
               
                BtAsignarLC.Enabled = PermisoSegunPerfil;
                ViewState["IdDestino"] = LBLCExistentes.SelectedItem.Text;
                ViewState["PosHMI"] = LBLCExistentes.SelectedItem.Value;
                ViewState["IdBoton"] = ((Button)sender).ID;
                LBLCExistentes.Enabled = false;
                
            }
            else
                BtAsignarLC.Enabled = false;
        }
        else
        {         
            BtLiberarLC.Enabled = PermisoSegunPerfil;
            BtAsignarLC.Enabled = false;
            ViewState["IdBoton"] = ((Button)sender).ID;
            LBLCExistentes.Enabled = false;
        }

        if (ibut.Text != "" || PermisoSegunPerfil)
        {
            ViewState["IdBoton"] = ((Button)sender).ID;
            PanelSelLC.Visible = true;
        }

    }

    private void MuestraLCMision(uint IdMision)
    {
        try
        {
            LimpiarPanelLC();
            ServiciosCD40.Misiones_LCEN mlc = new ServiciosCD40.Misiones_LCEN();
            mlc.IdMision = IdMision;
            ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(mlc);
            if (d != null)
            {
                for (int i = 0; i < d.Length; i++)
                {
                    uint posHMI = ((ServiciosCD40.Misiones_LCEN)d[i]).PosHMI;
                    uint posMision = ((ServiciosCD40.Misiones_LCEN)d[i]).PosMision;
                    string textoBoton = DameLiteralLC(posHMI);
                    CargaBotonMisionLC(posMision, posHMI, textoBoton);
                }
                ActualizaLCExistentes();
            }
        }
        catch (Exception e)
        {
            logDebugView.Error("(Misiones-PresentaPaginasSeleccionadas):", e);
        }

    }

    private void CargaBotonMisionLC(uint posicion, uint posicionHMI, string strEtiLCEN)
    {
        TableCell tCell = (TableCell)TEnlacesLC.FindControl("TableCellLC" + posicion.ToString());
        if (tCell != null)
        {
            tCell.Visible = posicion <= NumPosicionesPag;
            Button ibut = (Button)TEnlacesLC.FindControl("ButtonLC" + posicion.ToString());
            ibut.CssClass = "BtnPanelTfAsignado";
            ibut.Text = strEtiLCEN;
            ibut.AccessKey = posicionHMI.ToString();
        }
    }

    private void ActualizaLCExistentes()
    {
        MuestraLCExistentes();
        for (int i = 1; i <= NumPosicionesPag; i++)
        {
            TableCell tCell = (TableCell)TEnlacesLC.FindControl("TableCellLC" + i.ToString());
            if (tCell != null)
            {
                Button ibut = (Button)TEnlacesLC.FindControl("ButtonLC" + i.ToString());
                if (ibut.CssClass == "BtnPanelTfAsignado")
                {
                    LBLCExistentes.Items.Remove(LBLCExistentes.Items.FindByText(ibut.Text));                 
                }
            }
        }
        LBLCExistentes.Enabled = true;
    }

    private void MuestraLCExistentes()
    {
        try
        {
            int idlbl = 0;
            LBLCExistentes.Items.Clear();
            if (datosLCext != null)
            {
                string strEtiLCEN = string.Empty;

                for (int i = 0; i < datosLCext.Length; i++)
                {
                    uint pos = ((ServiciosCD40.DestinosExternosSector)datosLCext[i]).PosHMI;
                    //20201019
                    if (pos >= prefijosPosicionesLC.Length)
                    {
                        cMsg.alert((string)GetGlobalResourceObject("Espaniol", "PanelLcErrorParamCNF"));
                        continue;
                    }
                    LBLCExistentes.Items.Add(((ServiciosCD40.DestinosExternosSector)datosLCext[idlbl]).Literal);
                    LBLCExistentes.Items[i].Value = ((ServiciosCD40.DestinosExternosSector)datosLCext[idlbl]).PosHMI.ToString();
                    idlbl++;
                }
            }
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Misiones_MostrarLCLB): ", ex);
        }
    }

    private void CargaLCExistentes()
    {
        try
        {
            ServiciosCD40.Tablas[] d1lc = ServicioCD40.DestinosTelefoniaAsignadosAlSector((string)Session["idsistema"], Misiones.SectoresSirtap[0], false, false);
            datosLCext = d1lc;
        }
        catch (Exception ex)
        {
            logDebugView.Error("(TFTLC-CargaLCExistentes): ", ex);
        }
    }

    protected void BtLiberarLC_Click(object sender, EventArgs e)
    {

        LiberarDestinoLC((string)ViewState["IdBoton"]);
        ActualizaLCExistentes();
        PanelSelLC.Visible = false;
        TEnlacesLC.Enabled = true;
        
    }

    private void LiberarDestinoLC(string id)
    {
        Button ibut = (Button)TEnlacesLC.FindControl(id);
        ibut.CssClass = "BtnPanelRadioLibre";
        ibut.Text = "";
        ibut.AccessKey = "";
    }

    protected void BtCancelarLC_Click(object sender, EventArgs e)
    {
        PanelSelLC.Visible = false;
        TEnlacesLC.Enabled = true;
        
    }

    protected void BtAsignarLC_Click(object sender, EventArgs e)
    {
        AsignarDestinoLC((string)ViewState["IdBoton"]);
        ActualizaLCExistentes();
        PanelSelLC.Visible = false;
        TEnlacesLC.Enabled = true;
    }

    private void AsignarDestinoLC(string id)
    {
        Button ibut = (Button)TEnlacesLC.FindControl(id);
        ibut.CssClass = "BtnPanelTfAsignado";
        ibut.Text = (string)ViewState["IdDestino"];
        ibut.AccessKey = ViewState["PosHMI"].ToString();

    }

    private void LimpiarPanelLC()
    {
        TEnlacesLC.Height = 40;

        for (int i = 1; i <= NumPosicionesPag; i++)
        {
            TableCell tCell = (TableCell)TEnlacesLC.FindControl("TableCellLC" + i.ToString());
            if (tCell != null)
            {
                tCell.Visible = i <= NumPosicionesPag;
                Button ibut = (Button)TEnlacesLC.FindControl("ButtonLC" + i.ToString());
                ibut.CssClass = "BtnPanelRadioLibre";
                ibut.Text = "";
            }
        }
    }

    private void PanelLineaCalienteActivaSeleccion(bool activa)
    {
        PanelLCEN.Enabled = activa;
    }

    private void ActivaSeleccionTeclasLC(bool activa)
    {
        for (int i = 1; i <= NumPosicionesPag; i++)
        {
            TableCell tCell = (TableCell)TEnlacesLC.FindControl("TableCellLC" + i.ToString());
            if (tCell != null)
            {
                tCell.Visible = i <= NumPosicionesPag;
                Button ibut = (Button)TEnlacesLC.FindControl("ButtonLC" + i.ToString());
                ibut.Enabled = activa;
            }
        }
    }

    private string DameLiteralLC(uint PosHMI)
    {
        string strEtiLCEN =  "*ERROR*";
        try
        {
            if (datosLCext != null)
            {
                
                for (int i = 0; i < datosLCext.Length; i++)
                {
                    if (PosHMI == ((ServiciosCD40.DestinosExternosSector)datosLCext[i]).PosHMI)
                    {
                        strEtiLCEN = ((ServiciosCD40.DestinosExternosSector)datosLCext[i]).Literal;
                        break;
                    }
                }
            }
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Misiones_DameLiteralLC): ", ex);
        }
        return strEtiLCEN;
    }

    private void ActualizaMisionLCENSeleccionadas()
    {
        try
        {
            if (_IdMision == 0)
                return;
            ServiciosCD40.Misiones_LCEN mlc = new ServiciosCD40.Misiones_LCEN();
            mlc.IdMision = _IdMision;
            ServicioCD40.DeleteSQL(mlc);
            for (uint posms = 1; posms <= NumPosicionesPag; posms++)
            {
                TableCell tCell = (TableCell)TEnlacesLC.FindControl("TableCellLC" + posms.ToString());
                if (tCell != null)
                {                  
                    Button ibut = (Button)TEnlacesLC.FindControl("ButtonLC" + posms.ToString());
                    if (ibut.CssClass == "BtnPanelTfAsignado")
                    {
                        mlc = new ServiciosCD40.Misiones_LCEN();
                        mlc.IdMision = _IdMision;
                        mlc.PosMision = posms;
                        mlc.PosHMI = Convert.ToUInt32(ibut.AccessKey);
                        ServicioCD40.InsertSQL(mlc);                    
                    }
                }
            }
        }
        catch (Exception e)
        {
            logDebugView.Error("(Misiones-ActualizaMisionLCENSeleccionadas):", e);
        }
    }

    private void InicializaLCEN()
    {
        LimpiarPanelLC();
        CargaLCExistentes();
        MuestraLCExistentes();
    }
    #endregion

    #region GESTIÓN ALARMAS ACUSTICAS

    protected void PanelAlarmasAcusticasActivaSeleccion(bool activa)
    {
        IButSAAsignar.Enabled = activa;
        IButSADesAsignar.Enabled = activa;
    }

    private void CargaAlarmasAcusticasExistentes()
    {
        try
        {
            InicializaAlarmasAcusticas();
            ServiciosCD40.Alarmas_Acusticas aac = new ServiciosCD40.Alarmas_Acusticas();
            ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(aac);
            if (d != null)
            {
                for (int i = 0; i < d.Length; i++)
                {
                    string descripcionAlarma = String.Empty;
                    descripcionAlarma =  ((((ServiciosCD40.Alarmas_Acusticas)d[i]).Tipo == 1) ? "Alarma:" : "Alerta:") + "    " +  ((ServiciosCD40.Alarmas_Acusticas)d[i]).Descripcion;
                    ListaExistentes.Items.Add(descripcionAlarma);
                    ListaExistentes.Items[i].Value = ((ServiciosCD40.Alarmas_Acusticas)d[i]).IdAlarmaAcustica.ToString(); 
                }
            }
        }
        catch (Exception e)
        {
            logDebugView.Error("(Misiones-CargaAlarmasAcusticas):", e);
        }
    }

    private void CargaAlarmasAcusticasAsignadas(uint IdMision)
    {
        InicializaAlarmasAcusticasAsignadas();
        DataSet d = ServicioCD40.GetAlarmasAsociadasAMisiones(IdMision);
        int ind = 0;
        if (d != null && d.Tables.Count > 0)
        {
            foreach (System.Data.DataRow ds in d.Tables[0].Rows)
            {
                ListaAsignadas.Items.Add((((uint)ds["Tipo"] == 1) ? "Alarma:" : "Alerta:") + "    " + (string)ds["Descripcion"]);
                ListaAsignadas.Items[ind].Value = ((UInt32)ds["IdAlarmaAcustica"]).ToString();
                ind++;
            }
        }
    }

    private void InicializaAlarmasAcusticas()
    {
        ListaExistentes.Items.Clear();
    }

    private void InicializaAlarmasAcusticasAsignadas()
    {
        ListaAsignadas.Items.Clear();
    }
    protected void ListaAsignadas_Click(object sender, EventArgs e)
    {
        for (int i = 0; i < ListaExistentes.Items.Count; i++)
        {
            if (ListaExistentes.Items[i].Selected)
                ListaExistentes.Items[i].Selected = false;
        }
            
    }

    protected void ListaExistentes_Click(object sender, EventArgs e)
    {
        for (int i = 0; i < ListaAsignadas.Items.Count; i++)
        {
            if (ListaAsignadas.Items[i].Selected)
                ListaAsignadas.Items[i].Selected = false;
        }
 
    }

    private void ActivaSeleccionAlarmasAcusticas(bool Activa)
    {
        ListaAsignadas.Enabled = Activa;
        ListaExistentes.Enabled = Activa;
    }

    // Proceso de Señales Acústicas
    protected void IButSADesasignar_Click(object sender, ImageClickEventArgs e)
    {
        if (ListaAsignadas.SelectedIndex >= 0)
        {
            ListaAsignadas.Items.RemoveAt(ListaAsignadas.SelectedIndex);
        }
    }

    protected void IButSAAsignar_Click(object sender, ImageClickEventArgs e)
    {

        if (ListaExistentes.SelectedIndex >= 0)
        {
            ListItem alarmaAcustica = new ListItem();
            alarmaAcustica = ListaExistentes.SelectedItem;
            if (!ListaAsignadas.Items.Contains(alarmaAcustica))
            {
                ListaAsignadas.Items.Add(ListaExistentes.Items[ListaExistentes.SelectedIndex].Text);
                ListaAsignadas.Items[ListaAsignadas.Items.Count - 1].Value = alarmaAcustica.Value;
            }
        } 
    }

    protected void ActualizaMisionAlarmasAlertasAcusticas()
    {
        try 
        {
            if (_IdMision == 0)
                return;
            ServiciosCD40.Misiones_Alarmas_Acusticas maa = new ServiciosCD40.Misiones_Alarmas_Acusticas();
            maa.IdMision = _IdMision;
            ServicioCD40.DeleteSQL(maa);
            for (int indl = 0; indl < ListaAsignadas.Items.Count; indl++)
            {
                maa = new ServiciosCD40.Misiones_Alarmas_Acusticas();
                maa.IdMision = _IdMision;
                maa.IdAlarmaAcustica =   Convert.ToUInt32(ListaAsignadas.Items[indl].Value);
                ServicioCD40.InsertSQL(maa);
            }
        }
        catch (Exception e)
        {
            logDebugView.Error("(Misiones-ActualizaAlarmasAlertasAcusticas):", e);
        }
    }
    #endregion ALARMAS ACUSTICAS
}