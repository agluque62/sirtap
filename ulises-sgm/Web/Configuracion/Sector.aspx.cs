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
using SincronizaCD30;
using System.Text.RegularExpressions;

public partial class Sector :	PageBaseCD40.PageCD40	//	System.Web.UI.Page
{
	private static bool Modificando = false;
	private static string IdCentralPropia;
	private static DataSet DSNumerosAbonados, DSAgenda, DSPermisosRedes;
    private static ServiciosCD40.Tablas[] datos;
	private static ServiciosCD40.ServiciosCD40 ServicioCD40 = new ServiciosCD40.ServiciosCD40();
	static bool PermisoSegunPerfil;
	private static int NumPaginaActiva = 0;
    private AsyncCallback CallbackCompletado;
    private Ulises5000Configuration.ToolsUlises5000Section UlisesToolsVersion;
    private static string _NumAbonadoToDelete = string.Empty;
    private static bool OldSeleccionadoFS = false;
    //20210317 #4749
    private enum accion { alta = 0, baja = 1, modificacion = 2, presentacion = 3 };
    private static bool ControlDependenciasATS = false;
    //20210518 #4824
    private enum Estado_Sector { NO_ASOCIADO = 1, ASOCIADO_NOACTIVA = 2, ASOCIADO_ACTIVANOSACTA = 3, ASOCIADO_ACTIVASACTA = 4, ERROR_SQL = 5 }
    private static uint NumSectoresActuales = 0;
    private static uint NumMaxSectores = 0;
    
    //20210824 #4815
    private static uint NUM_MAX_SECTORES = 40;
    // RQF-4
    const int MAX_NUCLEOS = 4;
    const int MAX_SECTORES_NUCLEOS = 32;

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

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected new void Page_Load(object sender, EventArgs e)
    {
		base.Page_Load(sender, e);

        cMsg = (Mensajes.msgBox)this.Master.FindControl("MsgBox1");

		//ServicioCD40.NoTransaction();	// Las actuaciones sobre la base de datos se realizarán inmediatamente, independientemente de TransactionTimeout

		if (Context.Request.IsAuthenticated)
		{
			// retrieve user's identity from httpcontext user 
			FormsIdentity ident = (FormsIdentity)Context.User.Identity;
			string perfil = ident.Ticket.UserData;
			if (perfil == "0")
			{
				Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO");
				return;
			}
            //Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            //Version = config.AppSettings.Settings["Version"];
            Ulises5000Configuration.ToolsUlises5000Section ulisesTools = Ulises5000Configuration.ToolsUlises5000Section.Instance;

            UlisesToolsVersion = ulisesTools;


			PermisoSegunPerfil = perfil == "3";
			if (CallbackCompletado == null)
				CallbackCompletado = new AsyncCallback(OnCallBackCompleted);

			if (!IsPostBack)
			{
				NumPaginaActiva = 0;
                IndexListBox1 = -1;

				if (!MuestraNucleos())
				{
					MultiView1.Visible = false;
					BtNuevo.Visible = false;	// No se pueden dar de alta sectores sin algún núcleo configurado.
					cMsg.alert((string)GetGlobalResourceObject("Espaniol","SinNucleo"));
					return;
				}

                switch (UlisesToolsVersion.Version)
                {
                    case 0:
                        PanelAsecna.Visible = PanelComunes.Visible = PanelNoImplementadas.Visible = PanelTwr.Visible = true;
                        //20200911 #4591
                        CheckSeleccionadoFS.Visible = true;
                        break;
                    case 1:
                    case 2:
                        PanelComunes.Visible = PanelAsecna.Visible = true;
                        PanelTwr.Visible = PanelNoImplementadas.Visible = false;
                        //20200911 #4591
                        CheckSeleccionadoFS.Visible = false;
                        break;
                    case 3:
                        PanelComunes.Visible = PanelTwr.Visible = true;
                        PanelAsecna.Visible = PanelNoImplementadas.Visible = false;
                        // 20200911 #4591
                        CheckSeleccionadoFS.Visible = true;
                        break;
                }

                IBPropiedadesGenerales.CssClass = "ButtonImageSelected";
                IBUtilidades.CssClass = "ButtonImage";
                IBNivelesIntrusion.CssClass = "ButtonImage";
                IBPermisosRedes.CssClass = "ButtonImage";
                IBAgenda.CssClass = "ButtonImage";

                BtAceptar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "AceptarCambios");
                BtCancelar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "CancelarCambios");
                
                CargarIdSacta();

                if (Session["NombreSector"] != null && !string.IsNullOrEmpty((string)Session["NombreSector"]))
                {
                    NewItem = (string)Session["NombreSector"];
                    Session["NombreSector"] = null;
                }
                // 20210317 #4749
                // ControlDependenciasATS = SistemaConDependenciasATS();
                // RQF-4
                ControlDependenciasATS = false;
                ObtenerNumeroMaximoSectores();

				MuestraDatos(DameDatos());
				CargarPrefijos();
			}
			else
			{

                //Si se ha recargado la página, las variables datos y la variable de session tienen valor nulo es porque 
                // si ha cambiado la sesión del servidor, bien por conmutación o reinicio
                //por lo que se va a la página de login
                if (datos == null || Session["idsistema"] == null)
                {
                    ScriptManager.RegisterStartupScript(this, typeof(Page), "redirect", "<Script language = 'Javascript'> window.parent.location='../Login.aspx' ; </Script>", false);
                    return;
                }

				//CargaTipoSector();

				MultiView1.ActiveViewIndex = NumPaginaActiva;

                //if (Request.Form["eliminaelemento"] == "1")//El usuario elige eliminar el elemento 
                //{
                //    Request.Form["eliminaelemento"].Replace("1", "0");

                //    EliminarElemento();
                //}
                //if (Request.Form["cancelparam"] == "1")    //El usuario elige no guardar los cambios 
                //{
                //    Request.Form["cancelparam"].Replace("1", "0");

                //    CancelarCambios();
                //}
                //if (Request.Form["aceptparam"] == "1")     //El usuario elige guardar los cambios
                //{
                //    Request.Form["aceptparam"].Replace("1", "0");

                //    GuardarCambios();
                //}
				if (Request.Form["IrAEncaminamientos"] == "1")	// El usuario desea ir a dar de alta una Central Propia
				{
					Request.Form["IrAEncaminamientos"].Replace("1", "0");

					IrAEncaminamientos();
				}
				if (Request.Form["DefinirRangosCentralPropia"] == "1")	// El usuario desea ir a dar de alta rangos para la central propia
				{
					Request.Form["DefinirRangosCentralPropia"].Replace("1", "0");

					IrAEncaminamientos();
				}               
			}
		}
    }


    /// <summary>
    /// 
    /// </summary>
	private void IrAEncaminamientos()
	{
		Modificando = false;

		Response.Redirect("~/Configuracion/Encaminamientos.aspx");
	}

    /// <summary>
    /// 
    /// </summary>
	private void CargaTipoSector()
	{
        string strAux = string.Empty;

		DListTipoSector.Items.Clear();

        //Se leen las descripciones de los tipos de sector del fichero de recursos local
        //Real
        try
        {
            strAux = GetLocalResourceObject("ListItemTipoSectorReal.Text").ToString();
            DListTipoSector.Items.Add(new ListItem(strAux, "R"));
        }
        catch
        {
            //Recurso no encontrado
            DListTipoSector.Items.Add(new ListItem("Real", "R"));
        }

        //virtual
        try
        {
            strAux = GetLocalResourceObject("ListItemTipoSectorVirtual.Text").ToString();
            DListTipoSector.Items.Add(new ListItem(strAux, "V"));
        }
        catch
        {
            //Recurso no encontrado
            DListTipoSector.Items.Add(new ListItem("Virtual", "V"));
        }

        //Mantenimiento
        try
        {
            strAux = GetLocalResourceObject("ListItemTipoSectorMto.Text").ToString();
            DListTipoSector.Items.Add(new ListItem(strAux, "M"));
        }
        catch
        {
            //Recurso no encontrado
            DListTipoSector.Items.Add(new ListItem("Mantenimiento", "M"));
        }
	}

    /// <summary>
    /// 
    /// </summary>
	private void CargarIdSacta()
	{
		DListIdSacta.Items.Clear();
		for (int i = 1; i < 10000; i++)
			DListIdSacta.Items.Add(i.ToString());
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtNuevo_Click(object sender, EventArgs e)
    {
        // 20210421 NUM_MAX_SECTORES 
        if (NumSectoresActuales >= NumMaxSectores)
        {
            cMsg.confirm((string)GetGlobalResourceObject("Espaniol", "MaximoNumeroSectores"), "aceptparam");
            return;         
        }
   
        MostrarMenu();

        IndexListBox1 = ListBox1.SelectedIndex;

		BtEliminar.Visible = BtNuevo.Visible = BtModificar.Visible = false;
		BtAceptar.Visible = BtCancelar.Visible = true;

        CbCompartir.Checked = false;

		TxtIdSector.Enabled = true;
		HabilitaElementos(true);

        //TxtSacta.Visible = false;
        RellenaIdsSacta(true, 0);
        //DListIdSacta.Visible = true;    
		if (DSNumerosAbonados != null)
			DSNumerosAbonados.Clear();
		
        // Crear DsNumerosAbonados
		CargarAbonados((string)Session["idsistema"], TxtIdSector.Text, DListNucleo.SelectedValue);

        if (DSAgenda != null)
        {            
            DSAgenda.Clear();
            //Se actualiza el grid en pantalla
            GVAgenda.DataBind();
        }
        else	// Crear DsAgenda
            CargarAgenda((string)Session["idsistema"], TxtIdSector.Text, DListNucleo.SelectedValue);

        if (GVAbonados.DataSource != null && ((DataSet)GVAbonados.DataSource).Tables.Count > 0)
		    GVAbonados.DataBind();
        if (GVAgenda.DataSource != null && ((DataSet)GVAgenda.DataSource).Tables.Count > 0)
		    GVAgenda.DataBind();    
		
        DListNucleo.Items.Clear();
        MuestraNucleos();
        MostrarValidacion();
        //Nos situamos en la pestaña de parámetros generales
        if (MultiView1.ActiveViewIndex != 0)
            OnButton_Click(IBPropiedadesGenerales, null);

        // RQF-4
        OcultaFiltro(true);
	}

    /// <summary>
    /// 
    /// </summary>
    private void MostrarElemento()
    {
        MostrarMenu();

		DListNucleo.Items.Clear();
        // RQF-4
        MuestraNucleos();

		System.Diagnostics.Debug.Assert(datos != null, "Datos = null");
        for (int i = 0; i < datos.Length; i++)
        {
            if (String.Compare((((ServiciosCD40.Sectores)datos[i]).IdSector), ListBox1.SelectedValue) == 0)
            {
                BtEliminar_ConfirmButtonExtender.ConfirmText = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarSector"), ListBox1.SelectedValue);

                TxtIdSector.Text = ((ServiciosCD40.Sectores)datos[i]).IdSector;

				DDLPrioridadR2.SelectedValue=(((ServiciosCD40.Sectores)datos[i]).PrioridadR2).ToString();
                if (((ServiciosCD40.Sectores)datos[i]).Tipo != "M")
                {
					DListIdSacta.SelectedValue = (((ServiciosCD40.Sectores)datos[i]).NumSacta).ToString();
                    //TxtSacta.Text = (((ServiciosCD40.Sectores)datos[i]).NumSacta).ToString();
                    //TxtSacta.Visible = true;
					DListIdSacta.Visible = Label17.Visible = true;
                }
				else
				{
				    DListIdSacta.Visible = false;
				    Label17.Visible = false;
				}
                
                DListTipoSector.SelectedValue = ((ServiciosCD40.Sectores)datos[i]).Tipo;

                //Sólo se admite el tipo de posición C (Controlador), si en la BD tuviera un valor distinto, nos protegemos de la excepción
                try
                {
                    DListTipoPosicion.SelectedValue = ((ServiciosCD40.Sectores)datos[i]).TipoPosicion;
                }
                catch (Exception ex)
                {
                    DListTipoPosicion.SelectedValue = "C";
                }

                DListNucleo.SelectedValue = ((ServiciosCD40.Sectores)datos[i]).IdNucleo;
                DListNucleo.Text = ((ServiciosCD40.Sectores)datos[i]).IdNucleo;  
              
                ObtenerNiveles(((ServiciosCD40.Sectores)datos[i]).IdSistema, TxtIdSector.Text, DListNucleo.Text);
                ObtenerParametrosSector(((ServiciosCD40.Sectores)datos[i]).IdSistema, TxtIdSector.Text, DListNucleo.Text);
                ObtenerTeclasSector(((ServiciosCD40.Sectores)datos[i]).IdSistema, TxtIdSector.Text, DListNucleo.Text);
                CargarAbonados(((ServiciosCD40.Sectores)datos[i]).IdSistema, TxtIdSector.Text, DListNucleo.Text);
				CargarAgenda(((ServiciosCD40.Sectores)datos[i]).IdSistema, TxtIdSector.Text, DListNucleo.Text);

                //if (NumPaginaActiva == 3)   // Página de permisos redes
                ObtenerPermisosRedes(((ServiciosCD40.Sectores)datos[i]).IdSistema,
                                    ((ServiciosCD40.Sectores)datos[i]).IdSector);
                //20200911 JOI #4591
                CheckSeleccionadoFS.Checked = OldSeleccionadoFS = ((ServiciosCD40.Sectores)datos[i]).SeleccionadoFS;
                CheckSeleccionadoFS.Enabled = false;
                #region ControlDependenciasATS
                /*
                try
                {
                    if (ControlDependenciasATS)
                    {
                        if (((ServiciosCD40.Sectores)datos[i]).IdDependenciaATS == null || ((ServiciosCD40.Sectores)datos[i]).IdDependenciaATS == string.Empty)
                        {
                            // RQF-4
                            TBDepATS.Text = " ";
                        }
                        else
                        {
                            // RQF-4
                            TBDepATS.Text = ((ServiciosCD40.Sectores)datos[i]).IdDependenciaATS;
                        }
                    }
                }
                catch (ArgumentOutOfRangeException)
                {
                    TBDepATS.Text = " ";
                }*/
                #endregion ControlDependenciasATS
                // RQF-4
                DListNucleo.Enabled = false;
            }
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sistema"></param>
    /// <param name="sector"></param>
    /// <param name="elnucleo"></param>
	private void CargarAgenda(string sistema, string sector, string elnucleo)
	{
		try
		{
			ServiciosCD40.Agenda t = new ServiciosCD40.Agenda();
			t.IdSistema = sistema;
			t.IdSector = sector;
			t.IdNucleo = elnucleo;

			DataSet d = ServicioCD40.DataSetSelectSQL(t);
			GVAgenda.DataSource = DSAgenda = d;
			GVAgenda.DataBind();
		}
		catch (Exception ex)
		{
			logDebugView.Error("(Sector-CargarAgenda): ", ex);
		}
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sistema"></param>
    /// <param name="sector"></param>
    /// <param name="elnucleo"></param>
    private void CargarAbonados(string sistema, string sector, string elnucleo)
    {
        try
        {
			ServiciosCD40.UsuariosAbonados t = new ServiciosCD40.UsuariosAbonados();
			t.IdSistema = sistema;
			t.IdSector = sector;
			t.IdNucleo = elnucleo;

			DataSet d = ServicioCD40.RedesUsuariosAbonados(sistema, elnucleo, sector);
			GVAbonados.DataSource = DSNumerosAbonados = d;
			GVAbonados.DataBind();
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Sector-CargarAbonados): ",ex);
        }
    }

    /// <summary>
    /// 
    /// </summary>
    private void MostrarMenu()
    {
        LimpiarMenu();
        LimpiarChecks();

		BtRadio.Visible = true;
        BtTelefonia.Visible = true;
        BtLC.Visible = true;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="habilita"></param>
    private void HabilitaElementos(bool habilita)
    {
        CheckCaptura.Enabled = habilita;
        CheckTecla55.Enabled = habilita;
        CheckReAutomatica.Enabled = habilita;
        CheckTeclaPrio.Enabled = habilita;
        CheckConferencia.Enabled = habilita;
        CheckEscucha.Enabled = habilita;
        CheckRetener.Enabled = habilita;
        CheckRepUltLlamada.Enabled = habilita;
        // RQF-36
        CheckPermisoRTXSQ.Enabled = habilita;
        // RQF-36
        CheckPermisoRTXSect.Enabled = habilita;

        CheckMonitoring.Enabled = habilita;
        CheckRedireccion.Enabled = habilita;
        CheckCoordTF.Enabled = habilita;
        CheckCoorRD.Enabled = habilita;
        CheckLlamadaSelect.Enabled = habilita;
        CheckIntRDTF.Enabled = habilita;
        CheckTransDirect.Enabled = habilita;
        CheckBSS.Enabled = habilita;
        CheckLTT.Enabled = habilita;
        CheckRediCA.Enabled = habilita;
        CheckSayAgain.Enabled = habilita;
        CheckTransPre.Enabled = habilita;
        CheckIntrusion.Enabled = habilita;
        Panel2.Enabled = habilita;
        //CheckIntruido.Enabled = habilita;
        CheckRecording.Enabled = habilita;
        //20200911 JOI #4591
        CheckSeleccionadoFS.Enabled = habilita;
        //20200911 JOI #4591 FIN
        DListTipoPosicion.Enabled = habilita;
        DListTipoSector.Enabled = habilita;
        DListNucleo.Enabled = habilita;
        DListPrefijos.Enabled = habilita;
        DListIdSacta.Enabled = habilita;
        DDLPrioridadR2.Enabled = habilita;

        //TxtLlamEntIDA.Enabled = habilita;
        //TxtLlamIDA.Enabled = habilita;
        //TxtFrecPag.Enabled = habilita;
        //TxtPagFrec.Enabled = habilita;
        //TxtEnlIntPag.Enabled = habilita;
        //TxtPagEnlInt.Enabled = habilita;
        ////TxtAbonado.Enabled = habilita;
        //TxtKAM.Enabled = habilita;
        //TxtKAP.Enabled = habilita;

		DDLCicl.Enabled = habilita;
		DDLCipl.Enabled = habilita;
		DDLCpicl.Enabled = habilita;
		DDLCpipcl.Enabled = habilita;

		//LimpiarMenu();

//		LBoxAbonados.Enabled = habilita;
		PanelAbonado.Enabled = habilita;

		ListBox1.Enabled = !habilita;
		BtAceptar.Visible = habilita;
        BtCancelar.Visible = habilita;

        BtModificar.Visible = !habilita;

		BtRadio.Visible = !habilita;
		BtTelefonia.Visible = !habilita;
		BtLC.Visible = !habilita;
		//BtNuevo.Enabled = BtEliminar.Enabled = !habilita;

		BtNuevoAbonado.Visible = habilita;
		BtnAnadirAgenda.Enabled = habilita;
		LblNombre.Visible = LblNumero.Visible = LblPrefijo.Visible = habilita;
		TBNombreAgenda.Visible = TBNumeroAgenda.Visible = DDLPrefijoAgenda.Visible = habilita;
		GVAgenda.Columns[0].Visible = CbCompartir.Visible = habilita;
        HabilitaCheckBoxPermisos(habilita);
    }

    /// <summary>
    /// 
    /// </summary>
    private void LimpiarMenu()
    {
        DDLPrioridadR2.SelectedIndex = 0;
		TxtIdSector.Text = "";
		DDLPrioridadR2.SelectedIndex = 0;
		DListIdSacta.SelectedIndex = 0;

		DListTipoSector.SelectedIndex = 0;
        DListTipoPosicion.SelectedIndex = 0;
        if (DListNucleo.Items.Count>0)
		    DListNucleo.SelectedIndex = 0;
		GVAbonados.DataSource = null;
		GVAbonados.DataBind();

		DDLCicl.SelectedIndex = DDLCipl.SelectedIndex = DDLCpicl.SelectedIndex = DDLCpipcl.SelectedIndex = 0;
    }

    /// <summary>
    /// 
    /// </summary>
    private void LimpiarChecks()
    {
        CheckCaptura.Checked = false;
        CheckReAutomatica.Checked = false;
        CheckTeclaPrio.Checked = false;
        CheckTecla55.Checked = false;
        CheckConferencia.Checked = false;
        CheckEscucha.Checked = false;
        CheckRetener.Checked = false;
        CheckRepUltLlamada.Checked = false;
        // RQF-36
        CheckPermisoRTXSQ.Enabled = false;
        // RQF-36
        CheckPermisoRTXSect.Enabled = false;

        CheckMonitoring.Checked = false;
        CheckRedireccion.Checked = false;
        CheckCoordTF.Checked = false;
        CheckCoorRD.Checked = false;
        CheckLlamadaSelect.Checked = false;
        CheckIntRDTF.Checked = false;
        CheckTransDirect.Checked = false;
        CheckBSS.Checked = false;
        CheckLTT.Checked = false;
        CheckRediCA.Checked = false;
        CheckSayAgain.Checked = false;
        CheckTransPre.Checked = false;
        CheckIntrusion.Checked = false;
        CheckIntruido.Checked = false;
        CheckRecording.Checked = false;
        //20200911 JOI #4591
        CheckSeleccionadoFS.Checked = false;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void ListBox1_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (ListBox1.SelectedIndex >= 0)
        {
            _NumAbonadoToDelete = string.Empty;
            BtEliminar.Enabled = true;
            MostrarElemento();
        } 
    }

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    private ServiciosCD40.Tablas[] DameDatos()
    {
        try
        {
            ServiciosCD40.Sectores t = new ServiciosCD40.Sectores();
            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            t.IdSistema = s.Value;
            t.SectorSimple = true;
            Session["idsistema"] = s.Value;
			ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(t);
            datos = d;
            return d;
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Sector-DameDatos): ", ex);
        }
        return null;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sistema"></param>
    /// <param name="sector"></param>
    /// <param name="elnucleo"></param>
    private void ObtenerNiveles(string sistema,string sector,string elnucleo)
    {
        try
        {
            ServiciosCD40.Niveles t = new ServiciosCD40.Niveles();
            t.IdSistema = sistema;
            t.IdSector = sector;
            t.IdNucleo = elnucleo;
			ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(t);
            if (d.Length > 0)
            {
				DDLCicl.SelectedValue = ((ServiciosCD40.Niveles)d[0]).CICL.ToString();
				DDLCipl.SelectedValue = ((ServiciosCD40.Niveles)d[0]).CIPL.ToString();
				DDLCpicl.SelectedValue = ((ServiciosCD40.Niveles)d[0]).CPICL.ToString();
				DDLCpipcl.SelectedValue = ((ServiciosCD40.Niveles)d[0]).CPIPL.ToString();
            }
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Sector-ObtenerNiveles): ",ex);
        }        
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sistema"></param>
    /// <param name="sector"></param>
    /// <param name="elnucleo"></param>
    private void ObtenerTeclasSector(string sistema, string sector,string elnucleo)
    {
        try
        {
            ServiciosCD40.TeclasSector t = new ServiciosCD40.TeclasSector();
            t.IdSistema = sistema;
            t.IdSector = sector;
            t.IdNucleo = elnucleo;
			ServiciosCD40.Tablas[] datos = ServicioCD40.ListSelectSQL(t);
            if (datos.Length > 0)
            {
                CheckConferencia.Checked = ((ServiciosCD40.TeclasSector)datos[0]).Conferencia;
                CheckEscucha.Checked = ((ServiciosCD40.TeclasSector)datos[0]).Escucha;
                CheckRetener.Checked = ((ServiciosCD40.TeclasSector)datos[0]).Retener;
                CheckCaptura.Checked = ((ServiciosCD40.TeclasSector)datos[0]).Captura;
                CheckRedireccion.Checked = ((ServiciosCD40.TeclasSector)datos[0]).Redireccion;
                CheckCoordTF.Checked = ((ServiciosCD40.TeclasSector)datos[0]).CoordinadorTF;
                CheckLlamadaSelect.Checked = ((ServiciosCD40.TeclasSector)datos[0]).LlamadaSelectiva;
                CheckBSS.Checked = ((ServiciosCD40.TeclasSector)datos[0]).GrupoBSS;
                CheckLTT.Checked = ((ServiciosCD40.TeclasSector)datos[0]).LTT;
                CheckSayAgain.Checked = ((ServiciosCD40.TeclasSector)datos[0]).SayAgain;
                CheckRepUltLlamada.Checked = ((ServiciosCD40.TeclasSector)datos[0]).RepeticionUltLlamada;
                CheckReAutomatica.Checked = ((ServiciosCD40.TeclasSector)datos[0]).RellamadaAut;
                CheckTeclaPrio.Checked = ((ServiciosCD40.TeclasSector)datos[0]).TeclaPrioridad;
                CheckTecla55.Checked = ((ServiciosCD40.TeclasSector)datos[0]).Tecla55mas1;
                CheckMonitoring.Checked = ((ServiciosCD40.TeclasSector)datos[0]).Monitoring;
                CheckCoorRD.Checked = ((ServiciosCD40.TeclasSector)datos[0]).CoordinadorRD;
                CheckIntRDTF.Checked = ((ServiciosCD40.TeclasSector)datos[0]).IntegracionRDTF;
                CheckTransDirect.Checked = ((ServiciosCD40.TeclasSector)datos[0]).TransDirecta;
                CheckRediCA.Checked = ((ServiciosCD40.TeclasSector)datos[0]).InhabilitacionRedirec;
                CheckTransPre.Checked = ((ServiciosCD40.TeclasSector)datos[0]).TransConConsultaPrev;
                CheckRecording.Checked = ((ServiciosCD40.TeclasSector)datos[0]).Glp;
                // RQF-36
                CheckPermisoRTXSQ.Checked = ((ServiciosCD40.TeclasSector)datos[0]).PermisoRTXSQ;
                // RQF-36
                CheckPermisoRTXSect.Checked = ((ServiciosCD40.TeclasSector)datos[0]).PermisoRTXSect;
            }
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Sector-ObtenerTeclasSector): ", ex);
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sistema"></param>
    /// <param name="sector"></param>
    /// <param name="elnucleo"></param>
    private void ObtenerParametrosSector(string sistema, string sector, string elnucleo)
    {
        try
        {
            ServiciosCD40.ParametrosSector t = new ServiciosCD40.ParametrosSector();
            t.IdSistema = sistema;
            t.IdSector = sector;
            t.IdNucleo = elnucleo;
			ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(t);
            if (d.Length > 0)
            {
                //TxtLlamEntIDA.Text = ((ServiciosCD40.ParametrosSector)d[0]).NumLlamadasEntrantesIda.ToString();
                //TxtLlamIDA.Text = ((ServiciosCD40.ParametrosSector)d[0]).NumLlamadasEnIda.ToString();
                //TxtFrecPag.Text = ((ServiciosCD40.ParametrosSector)d[0]).NumFrecPagina.ToString();
                //TxtPagFrec.Text = ((ServiciosCD40.ParametrosSector)d[0]).NumPagFrec.ToString();
                //TxtEnlIntPag.Text = ((ServiciosCD40.ParametrosSector)d[0]).NumDestinosInternosPag.ToString();
                //TxtPagEnlInt.Text = ((ServiciosCD40.ParametrosSector)d[0]).NumPagDestinosInt.ToString();
                //TxtKAP.Text = ((ServiciosCD40.ParametrosSector)d[0]).KeepAlivePeriod.ToString();
                //TxtKAM.Text = ((ServiciosCD40.ParametrosSector)d[0]).KeepAliveMultiplier.ToString();
                CheckIntrusion.Checked = ((ServiciosCD40.ParametrosSector)d[0]).Intrusion;
                CheckIntruido.Checked = ((ServiciosCD40.ParametrosSector)d[0]).Intruido;
            }
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Sector-ObtenerParametrosSector): ",ex);
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="nu"></param>
    private void MuestraDatos(ServiciosCD40.Tablas[] nu)
    {
        // 20210421 NUM_MAX_SECTORES
        if (nu != null)
        {
            NumSectoresActuales = (uint)nu.Length;
        }
      
        CargaTipoSector();

        ListBox1.Items.Clear();
        if (nu != null)
            for (int i = 0; i < nu.Length; i++)
            {
                // Eliminar el sector **FS** si lo hubiera
                if (((ServiciosCD40.Sectores)nu[i]).IdSector!="**FS**")
                    ListBox1.Items.Add(((ServiciosCD40.Sectores)nu[i]).IdSector);
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
			BtNuevo.Visible = BtModificar.Visible = BtEliminar.Visible = PermisoSegunPerfil;

		}
		else
		{
			LimpiarMenu();
			BtNuevo.Visible = PermisoSegunPerfil;
			BtModificar.Visible = BtEliminar.Visible = false;
		}
	}



    /// <summary>
    /// 
    /// </summary>
    /// <param name="nuevo"></param>
    /// <param name="id"></param>
    private void RellenaIdsSacta(bool nuevo, uint id)
    {
        DListIdSacta.Items.Clear();
        for (int i = 1; i < 10000; i++)
            DListIdSacta.Items.Add(i.ToString());
        for (int i = 0; i < datos.Length; i++)
            if (!nuevo)
            {
                if (id != ((ServiciosCD40.Sectores)datos[i]).NumSacta)
                    DListIdSacta.Items.Remove(((ServiciosCD40.Sectores)datos[i]).NumSacta.ToString());
            }
            else
                DListIdSacta.Items.Remove(((ServiciosCD40.Sectores)datos[i]).NumSacta.ToString());
    }

    /// <summary>
    /// 
    /// </summary>
	protected override void AceptarCambios()
	{
		base.AceptarCambios();
	}

    /// <summary>
    /// 
    /// </summary>
    protected override void CancelarCambios()
    {
		Modificando = false;
		//RegeneraSectorizacionPorAgenda = false;

        HabilitaElementos(false);
        BtNuevo.Enabled = true;
        EsconderValidacion();
        ListBox1.Enabled = true;

		CargarIdSacta();
		MuestraDatos(DameDatos());

		BtNuevo.Visible = PermisoSegunPerfil;
		BtModificar.Visible = BtEliminar.Visible = PermisoSegunPerfil && ListBox1.Items.Count > 0;

		BtAceptar.Visible = BtCancelar.Visible = false;

//		MostrarElemento();
    }

    /// <summary>
    /// 
    /// </summary>
    private void GuardarCambios()
    {
        /**
         * AGL 2012.06.18 ID.130
         * */
        //VMG 04/03/2019
        Page.ClientScript.RegisterStartupScript(this.GetType(), "displaySloader", "displaySloader();", true);
        bool bSeleccionadoFS = false;//20200911 JOI #4591
        try
        {
            ServiciosCD40.Sectores n = new ServiciosCD40.Sectores();
            n.IdSistema = (string)Session["idsistema"];
            if (!Modificando) //Sector nuevo
            {
                n.IdSector = TxtIdSector.Text;

                if (AlcanzadoMaximoORsNucleo(DListNucleo.SelectedValue))
                {
                    cMsg.alert((string)GetGlobalResourceObject("Espaniol", "MaximoNumeroSectoresPorNucleo"));
                    return;
                }

            }
            else
                n.IdSector = ListBox1.SelectedValue;

            NewItem = n.IdSector;

            n.SectorSimple = true;
            n.PrioridadR2 = UInt16.Parse(DDLPrioridadR2.SelectedValue);
            n.Tipo = DListTipoSector.SelectedValue;
            if (n.Tipo != "M")
                n.NumSacta = UInt16.Parse(DListIdSacta.SelectedValue);
            else
            {
                if (!Modificando) //Sector nuevo
					n.NumSacta = (uint)ServicioCD40.GetIdSactaMantenimiento(n.IdSistema);
                else
                    for (int i = 0; i < datos.Length; i++)
                        if (String.Compare((((ServiciosCD40.Sectores)datos[i]).IdSector), TxtIdSector.Text) == 0)
                        {
                            TxtIdSector.Text = ((ServiciosCD40.Sectores)datos[i]).IdSector;
                            DDLPrioridadR2.SelectedValue = (((ServiciosCD40.Sectores)datos[i]).PrioridadR2).ToString();
                            if (((ServiciosCD40.Sectores)datos[i]).Tipo == "M")
                                n.NumSacta = ((ServiciosCD40.Sectores)datos[i]).NumSacta;
                        }
            }

            
            //MVO.2018/08/02: en esta versión, el tipo posición será siempre Controlador="C"
            //n.TipoPosicion = DListTipoPosicion.SelectedValue;
            n.TipoPosicion = "C";
            n.IdNucleo = DListNucleo.SelectedValue;
            // 20200911 #4591

            n.SeleccionadoFS = CheckSeleccionadoFS.Checked;
            // 20210317 #4749
            // RQF-34
            // if (ControlDependenciasATS)
            // {
                //n.IdDependenciaATS = DListNucleo.SelectedValue;
            // }
            // else
            // n.IdDependenciaATS = " "; 
            // RQF-4
            n.IdDependenciaATS = DListNucleo.SelectedValue;


            ServiciosCD40.Niveles niv = new ServiciosCD40.Niveles();
            niv.IdSistema = n.IdSistema;
            niv.IdSector = n.IdSector;
            niv.IdNucleo = n.IdNucleo;
            if (DDLCicl.Text.Length > 0)
				niv.CICL = UInt16.Parse(DDLCicl.Text);
            if (DDLCipl.Text.Length > 0)
				niv.CIPL = UInt16.Parse(DDLCipl.Text);
            if (DDLCpicl.Text.Length > 0)
				niv.CPICL = UInt16.Parse(DDLCpicl.Text);
			if (DDLCpipcl.Text.Length > 0)
				niv.CPIPL = UInt16.Parse(DDLCpipcl.Text);

            ServiciosCD40.TeclasSector tec = new ServiciosCD40.TeclasSector();
            tec.IdSistema = n.IdSistema;
            tec.IdSector = n.IdSector;
            tec.IdNucleo = n.IdNucleo;
            tec.Conferencia = CheckConferencia.Checked;
            tec.Escucha = CheckEscucha.Checked;
            tec.Retener = CheckRetener.Checked;
            tec.Captura = CheckCaptura.Checked;
            tec.Redireccion = CheckRedireccion.Checked;
            tec.CoordinadorTF = CheckCoordTF.Checked;
            tec.LlamadaSelectiva = CheckLlamadaSelect.Checked;
            tec.GrupoBSS = CheckBSS.Checked;
            tec.LTT = CheckLTT.Checked;
            tec.SayAgain = CheckSayAgain.Checked;
            tec.RepeticionUltLlamada = CheckRepUltLlamada.Checked;
            tec.RellamadaAut = CheckReAutomatica.Checked;
            // RQF-36
            tec.PermisoRTXSQ = CheckPermisoRTXSQ.Checked;
            // RQF-36
            tec.PermisoRTXSect = CheckPermisoRTXSect.Checked;            
            // JCAM. 
            // 13/12/2016
            // Se elimina el check de tecla de prioridad en el panel de Utilidades y se interpreta 
            // que el sector va a tener esta tecla en el panel si tiene permiso de Intruir.
            tec.TeclaPrioridad = CheckIntrusion.Checked;  //  CheckTeclaPrio.Checked;

            tec.Tecla55mas1 = CheckTecla55.Checked;
            tec.Monitoring = CheckMonitoring.Checked;
            tec.CoordinadorRD = CheckCoorRD.Checked;
            tec.IntegracionRDTF = CheckIntRDTF.Checked;
            tec.TransDirecta = CheckTransDirect.Checked;
            tec.InhabilitacionRedirec = CheckRediCA.Checked;
			tec.TransConConsultaPrev = CheckTransPre.Checked;
            tec.Glp = CheckRecording.Checked;

            ServiciosCD40.ParametrosSector parsec = new ServiciosCD40.ParametrosSector();
            ServiciosCD40.Sistema sistema = new ServiciosCD40.Sistema();
            parsec.IdSistema = n.IdSistema;
            parsec.IdSector = n.IdSector;
            parsec.IdNucleo = n.IdNucleo;
            parsec.Intrusion = CheckIntrusion.Checked;
            parsec.Intruido = CheckIntruido.Checked;
            // Obtener los parámetros del sistema que afectan a los paneles de los sectores
            sistema.IdSistema = n.IdSistema;
            ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(sistema);
            if (d!=null && d.Length > 0)
            {
                parsec.KeepAlivePeriod = ((ServiciosCD40.Sistema)d[0]).KeepAlivePeriod;
                parsec.KeepAliveMultiplier = ((ServiciosCD40.Sistema)d[0]).KeepAliveMultiplier;
                parsec.NumDestinosInternosPag = ((ServiciosCD40.Sistema)d[0]).NumDestinosInternosPag;
                parsec.NumFrecPagina = ((ServiciosCD40.Sistema)d[0]).NumFrecPagina;
                parsec.NumLlamadasEnIda = ((ServiciosCD40.Sistema)d[0]).NumLlamadasEnIda;
                parsec.NumLlamadasEntrantesIda = ((ServiciosCD40.Sistema)d[0]).NumLlamadasEntrantesIda;
                parsec.NumPagDestinosInt = ((ServiciosCD40.Sistema)d[0]).NumPagDestinosInt;
                parsec.NumPagFrec = ((ServiciosCD40.Sistema)d[0]).NumPagFrec;
                parsec.NumEnlacesAI = ((ServiciosCD40.Sistema)d[0]).NumEnlacesAI;
            }

            //20200911 JOI #4591
            if ((Modificando && !OldSeleccionadoFS && n.SeleccionadoFS) ||
                (Modificando &&  OldSeleccionadoFS && !n.SeleccionadoFS) ||
                (!Modificando && n.SeleccionadoFS))
            {
               // Quita selección de ** FS ** si la hubiese
                bSeleccionadoFS = true;
            }

            ServicioCD40.ActualizaSector(Modificando, n, niv, tec, parsec, DSNumerosAbonados, DSAgenda, bSeleccionadoFS); //20200911 JOI #4591
            if (!Modificando || bSeleccionadoFS) //Sector nuevo
    			ActualizaWebPadre(true);

            ActualizaPermisosRedes(n.IdSector);

            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement sincronizar = config.AppSettings.Settings["SincronizaCD30"];
            if ((sincronizar != null) && (Int32.Parse(sincronizar.Value) == 1))
            {
                List<string> listaNumPub = new List<string>();
                List<string> listaNumATS = new List<string>();

                if (DSNumerosAbonados.Tables.Count > 0)
                {
                    ServiciosCD40.UsuariosAbonados t = new ServiciosCD40.UsuariosAbonados();

                    for (int i = 0; i < DSNumerosAbonados.Tables[0].Rows.Count; i++)
                    {
                        t.IdPrefijo = (uint)DSNumerosAbonados.Tables[0].Rows[i].ItemArray[1];
                        t.IdAbonado = (string)DSNumerosAbonados.Tables[0].Rows[i].ItemArray[2];
                        //falta el prefijo
                        if (t.IdPrefijo == 3)//ATS
                            listaNumATS.Add(t.IdAbonado);
                        else
                            if (t.IdPrefijo > 3) //NumPub
                                listaNumPub.Add(t.IdAbonado);
                    }
                }
                SincronizaCD30.SincronizaCD30 sincro = new SincronizaCD30.SincronizaCD30();
                if (TxtIdSector.Enabled) //Sector nuevo
                {                    
                    switch (sincro.AltaUsuario(n.IdNucleo, n.IdSector, n.Tipo, (int)n.NumSacta, (int)n.PrioridadR2, listaNumPub.ToArray(), listaNumATS.ToArray()))
                    {
                        case 114:
                            cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "Cod114"), n.IdSector));
                            break;
                        case 115:
                            cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "Cod115"), n.IdSector));
                            break;
                        case 116:
                            cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "Cod116"), n.IdSector));
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    switch (sincro.ModificacionUsuario(n.IdNucleo, n.IdSector, n.Tipo, (int)n.NumSacta, (int)n.PrioridadR2, listaNumPub.ToArray(), listaNumATS.ToArray()))
                    {
                        case 127:
                            cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "Cod127"), n.IdSector));
                            break;
                        case 128:
                            cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "Cod128"), n.IdSector));
                            break;
                        default:
                            break;
                    }
                }
            }

        }
        catch (Exception ex)
        {
            logDebugView.Error("(Sector-GuardarCambios): ",ex);
        }

		HabilitaElementos(false);
        EsconderValidacion();

		ListBox1.Enabled = true;
		BtNuevo.Visible = PermisoSegunPerfil;
		BtModificar.Visible = BtEliminar.Visible = PermisoSegunPerfil && ListBox1.Items.Count > 0;

		BtAceptar.Visible = BtCancelar.Visible = false;
		CargarIdSacta();
        MuestraDatos(DameDatos());

        Session.Add("Sectorizando", true);
        ServicioCD40.BeginRegeneraSectorizaciones((string)Session["idsistema"], true, false, false, CallbackCompletado, null);

		Modificando = false;
    }

    private void ActualizaPermisosRedes(string idSector)
    {
        if (DSPermisosRedes!=null && DSPermisosRedes.Tables.Count > 0)
        {
            ServiciosCD40.PermisosRedes pr = new ServiciosCD40.PermisosRedes();
            pr.IdSistema = (string)Session["idsistema"];
            pr.IdNucleo = DListNucleo.SelectedValue;
            pr.IdSector = idSector;

            // Para cada fila (red) del GVPermisosRedes
            for (int i = 0; i < DSPermisosRedes.Tables[0].Rows.Count; i++)
            {
                // IdRed
                pr.IdRed = (string)(DSPermisosRedes.Tables[0].Rows[i][0]);

                if (i == (GVPermisosRedes.PageIndex * GVPermisosRedes.PageSize) + (i % GVPermisosRedes.PageSize))
                {
                    pr.Llamar = ((CheckBox)(GVPermisosRedes.Rows[i % GVPermisosRedes.PageSize].Cells[1].Controls[0])).Checked;
                    pr.Recibir = ((CheckBox)(GVPermisosRedes.Rows[i % GVPermisosRedes.PageSize].Cells[2].Controls[0])).Checked;
                }
                else
                {
                    pr.Llamar = DSPermisosRedes.Tables[0].Rows[i][1].ToString().ToUpper() == "TRUE";
                    pr.Recibir = DSPermisosRedes.Tables[0].Rows[i][2].ToString().ToUpper() == "TRUE";
                }

                ServicioCD40.DeleteSQL(pr);
                ServicioCD40.InsertSQL(pr);
            }
        }
    }
    
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtCancelar_Click(object sender, EventArgs e)
    {
        CancelarCambios();
        OcultaFiltro(false);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtAceptar_Click(object sender, EventArgs e)
    {
        bool encontrado = false;
        if (DSNumerosAbonados.Tables.Count > 0)
        {
            foreach (DataRow dr in DSNumerosAbonados.Tables[0].Rows)
            {
                if ((uint)dr.ItemArray[1] == 3)	// Prefijo ATS
                {
                    encontrado = true;
                    break;
                }
            }
        }

        if (encontrado)
            GuardarCambios();
        else
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol","UnNumeroATS"));
        }
        OcultaFiltro(false);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtModificar_Click(object sender, EventArgs e)
    {
        try
        {
			Modificando = true;
            IndexListBox1 = ListBox1.SelectedIndex;

            CbCompartir.Checked = false;

			TxtIdSector.Enabled = false;
			HabilitaElementos(true);
            // RQF-4
            DListNucleoSel.Enabled = false;
            DListNucleo.Enabled = false;
			BtEliminar.Visible = BtNuevo.Visible = BtModificar.Visible = false;
			BtAceptar.Visible = BtCancelar.Visible = true;

			if (DListTipoSector.SelectedValue != "M")
			{
				DListTipoSector.Items.Remove(DListTipoSector.Items.FindByValue("M"));
				string idSacta = DListIdSacta.SelectedValue;
				RellenaIdsSacta(false, UInt32.Parse(DListIdSacta.SelectedValue));
				DListIdSacta.SelectedValue = idSacta;
			}
			else
			{
				RellenaIdsSacta(true, 0);
				DListTipoSector.Enabled = false;
			}
			
			DListIdSacta.Enabled = DListTipoSector.SelectedValue != "M";
            MostrarValidacion();
		}
        catch (Exception ex)
        {
            logDebugView.Error("(Sector-BtModificar_Click): ", ex);
        }
        // RQF-4
        OcultaFiltro(true);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtEliminar_Click(object sender, EventArgs e)
    {
        int iSituacion = 0;
		HabilitaElementos(false);
        if (ListBox1.SelectedValue != "")
        {
            string strSector = ListBox1.SelectedValue;
            // RQF-4 
            // FALTA VALIDAR CONTRA GRUPOS-ESPECIALES // OJOOOO
                // 20210518 #4824
                // estadosector NO_ASOCIADO=1, ASOCIADO_NOACTIVA=2 errorsql = 5 
                // Devuelve situacion del sector, si está asignado a alguna sectorización.
                iSituacion = ServicioCD40.SectorAsignadoEnSectorizacionTemporalOActiva((string)Session["idsistema"], strSector);
                if (iSituacion == (int)Estado_Sector.ASOCIADO_NOACTIVA)
                {
                    BloquearFunciones();
                    cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "AvisoSectorEnSectorizacion"),strSector));
                }
                else
                {
                    IndexListBox1 = ListBox1.SelectedIndex;
                    Session["elemento"] = strSector;
                    EliminarElemento();
                }
        }        
        // RQF-4
        HabilitarFunciones();
        OcultaFiltro(false);
    }

    /// <summary>
    /// 
    /// </summary>
    private void EliminarElemento()
    {
        try
        {
			ServiciosCD40.Sectores n = new ServiciosCD40.Sectores();
            n.IdSistema = (string)Session["idsistema"];
            n.IdSector = (string)Session["elemento"];

            if (ServicioCD40.DeleteSQL(n) < 0)
            {
                logDebugView.Warn("(Sector-EliminarElemento): No se ha podido eliminar el sector.");
                cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "ErrorEliminarSector"), n.IdSector));
            }
            else
            {
                Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
                KeyValueConfigurationElement sincronizar = config.AppSettings.Settings["SincronizaCD30"];
                if ((sincronizar != null) && (Int32.Parse(sincronizar.Value) == 1))
                {
                    SincronizaCD30.SincronizaCD30 sincro = new SincronizaCD30.SincronizaCD30();
                    switch (sincro.BajaUsuario(DListNucleo.SelectedValue, n.IdSector))
                    {
                        case 117:
                            string s = (string)GetGlobalResourceObject("Espaniol", "ElementoEliminado") + "\\n\\n"
                                + String.Format((string)GetGlobalResourceObject("Espaniol", "Cod117"), n.IdSector);
                            cMsg.alert(s);
                            break;
                        case 118:
                            string s1 = (string)GetGlobalResourceObject("Espaniol", "ElementoEliminado") + "\\n\\n"
                                + String.Format((string)GetGlobalResourceObject("Espaniol", "Cod118"), n.IdSector);
                            cMsg.alert(s1);
                            break;
                        default:
                            break;
                    }
				}
                else
                    cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ElementoEliminado"));
			
				MuestraDatos(DameDatos());
				Session.Add("Sectorizando", true);
				ServicioCD40.BeginRegeneraSectorizaciones((string)Session["idsistema"], true, false, false, CallbackCompletado, null);
			}

        }
        catch (Exception ex)
        {
            logDebugView.Error("(Sector-EliminarElemento): ",ex);
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    private bool MuestraNucleos()
    {
		bool hayNucleos = false;
        try
        {
			Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
			KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
			
			ServiciosCD40.Nucleos t = new ServiciosCD40.Nucleos();
			t.IdSistema = s.Value;
			ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(t);
            //RQF-4
            DListNucleo.Items.Clear();
            DListNucleoSel.Items.Clear();
            for (int i = 0; i < d.Length; i++)
            {
                DListNucleo.Items.Add(((ServiciosCD40.Nucleos)d[i]).IdNucleo);
                DListNucleoSel.Items.Add(((ServiciosCD40.Nucleos)d[i]).IdNucleo);
            }

			hayNucleos = d.Length > 0;
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Sector-MuestraNucleos): ", ex);
        }

		return hayNucleos;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void Button2_Click(object sender, EventArgs e)
    {
        Session["NombreSector"] = TxtIdSector.Text;
        Session["idnucleo"] = DListNucleo.SelectedValue;
        Response.Redirect("~/Configuracion/TFTRadio.aspx");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtTelefonia_Click(object sender, EventArgs e)
    {
        Session["NombreSector"] = TxtIdSector.Text;
        Session["idnucleo"] = DListNucleo.SelectedValue;
        Response.Redirect("~/Configuracion/TFTTelefonia.aspx");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtLC_Click(object sender, EventArgs e)
    {
        Session["NombreSector"] = TxtIdSector.Text;
        Session["idnucleo"] = DListNucleo.SelectedValue;
        Response.Redirect("~/Configuracion/TFTLC.aspx");
    }

    /// <summary>
    /// 
    /// </summary>
    private void MostrarValidacion()
    {
        ValidationSummary1.Visible = true;
        RequiredFieldSector.Visible = true;

    }

    /// <summary>
    /// 
    /// </summary>
    private void EsconderValidacion()
    {
        ValidationSummary1.Visible = false;
        RequiredFieldSector.Visible = false;
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void DListTipoSector_SelectedIndexChanged(object sender, EventArgs e)
    {
		Label17.Visible = DListIdSacta.Visible = DListTipoSector.SelectedValue != "M";
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtNuevoAbonado_Click(object sender, EventArgs e)
    {
		Label19.Visible = true;
		Label20.Visible = true;
		TxtAbonado.Visible = true;
		DListPrefijos.Visible = true;
		BtNuevoAbonado.Visible = false;
		BtAceptar.Enabled = false;
		//BtCancelar.Enabled = false;
		BtAceptarAbonado.Visible = true;
		BtCancelarAbonado.Visible = true;

		LkBRangos.Visible = DListPrefijos.SelectedValue == "3";	// Los rangos de la central ATS sólo son visibles si se selecciona prefijo ATS
    }

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
	private bool ExisteCentralATSPropia()
	{
		ServiciosCD40.Encaminamientos e = new ServiciosCD40.Encaminamientos();
		Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
		KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];

		IdCentralPropia = ServicioCD40.ExisteCentralPropia(s.Value);
		return IdCentralPropia != null;
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtCancelarAbonado_Click(object sender, EventArgs e)
    {
        Label19.Visible = false;
        Label20.Visible = false;
        DListPrefijos.Visible = false;
        TxtAbonado.Visible = false;
        TxtAbonado.Text = "";
        BtNuevoAbonado.Visible = true;
        BtAceptar.Enabled = true;
        BtCancelar.Enabled = true;
        BtAceptarAbonado.Visible = false;
        BtCancelarAbonado.Visible = false;
        LkBRangos.Visible = false;
        GVRangos.Visible = false;
        GVAbonados.Visible = true;

        if (null != GetLocalResourceObject("Label18Resource1.Text"))
        {
            Label18.Text = GetLocalResourceObject("Label18Resource1.Text").ToString();
        }
        else
            Label18.Text = "Abonados a los que atiende:";
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtAceptarAbonado_Click(object sender, EventArgs e)
    {
		// Comprobar que si el número de abonado es de la red ATS, está dentro del rango para la central propia.
		if (3==Convert.ToInt32(DListPrefijos.SelectedValue))
		{
			bool rangoEncontrado = false;
			ServiciosCD40.Rangos r = new ServiciosCD40.Rangos();

			if (ExisteCentralATSPropia())
			{
				Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
				KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];

				r.IdSistema = s.Value;
				r.Central = IdCentralPropia;
				ServiciosCD40.Tablas[] rangoCentralPropia = ServicioCD40.ListSelectSQL(r);
                //  #3690
                if (rangoCentralPropia.Length > 0)
                {
                    double dNuevoNum = -1;

                    if (Regex.IsMatch(TxtAbonado.Text, @"^(2|3)([0-9]{5})$") && double.TryParse(TxtAbonado.Text, out dNuevoNum))
                    {

                        foreach (ServiciosCD40.Rangos rango in rangoCentralPropia)
                        {
                            double dValorInicial = -1;
                            double dValorFinal = -1;
                            if (double.TryParse(rango.Inicial, out dValorInicial) && double.TryParse(rango.Final, out dValorFinal))
                            {

                                if (dNuevoNum >= dValorInicial && dNuevoNum <= dValorFinal)
                                {
                                    rangoEncontrado = true;
                                    break;
                                }
                            }
                        }
                        if (!rangoEncontrado)
                        {
                            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "EntreRangoCentral") + IdCentralPropia + ".");
                            return;
                        }
                    }
                    else
                    {
                        //El número de abonado no tiene el formato correcto.
                        cMsg.alert((string)GetGlobalResourceObject("Espaniol", "NumAbonadoIncorrecto"));
                        return;
                    }
                }
				else
				{
					cMsg.confirm(String.Format((string)GetGlobalResourceObject("Espaniol", "DefinirRangosCentralPropia"),IdCentralPropia), "DefinirRangosCentralPropia");
					return;
				}
			}
			else
			{
				cMsg.confirm((string)GetGlobalResourceObject("Espaniol", "IrAEncaminamientos"), "IrAEncaminamientos");
				return;
			}
		}

        // Comprobar que este número no coincide con el que se acabe de borrar 
        if (_NumAbonadoToDelete != TxtAbonado.Text)
        {
            // Comprobar que este número de abonado no pertenece a ningún otro usuario
            string sector = ServicioCD40.ExisteSectorConNumeroAbonado((string)Session["idsistema"], DListNucleo.SelectedValue, Convert.ToUInt32(DListPrefijos.SelectedValue), TxtAbonado.Text);

            if (sector != "")
            {
                cMsg.alert((string)GetGlobalResourceObject("Espaniol", "AbonadoAsignado") + sector);
                return;
            }
        }

		DataRow dr = DSNumerosAbonados.Tables[0].NewRow();
		dr[2] = TxtAbonado.Text;
		dr[1] = Convert.ToInt32(DListPrefijos.SelectedValue);
		dr[0] = DListPrefijos.SelectedItem.Text;
		DSNumerosAbonados.Tables[0].Rows.Add(dr);
		DSNumerosAbonados.AcceptChanges();
        
        if (null != GetLocalResourceObject("Label18Resource1.Text"))
        {
            Label18.Text = GetLocalResourceObject("Label18Resource1.Text").ToString();
        }
        else
            Label18.Text = "Abonados a los que atiende:";

		GVAbonados.Visible = true;
		GVAbonados.DataSource = DSNumerosAbonados;
		GVAbonados.DataBind();

        Label19.Visible = false;
        Label20.Visible = false;
        DListPrefijos.Visible = false;
        TxtAbonado.Visible = false;
        TxtAbonado.Text = "";
        BtNuevoAbonado.Visible = true;
        BtAceptar.Enabled = true;
        BtCancelar.Enabled = true;
        BtAceptarAbonado.Visible = false;
        BtCancelarAbonado.Visible = false;
		LkBRangos.Visible = false;
		GVRangos.Visible = false;
	}

	//private void GuardarUsuariosAbonados(string sistema, string sector, string elnucleo)
	//{
	//    try
	//    {
	//        ServiciosCD40.UsuariosAbonados t = new ServiciosCD40.UsuariosAbonados();
	//        t.IdSistema = sistema;
	//        t.IdSector = sector;
	//        t.IdNucleo = elnucleo;
	//        if (ServicioCD40.DeleteSQL(t) < 0) logDebugView.Warn("(Sector-GuardarUsuariosAbonados): No se ha podido eliminar el UsuarioAbonado");

	//        for (int i = 0; i < DSNumerosAbonados.Tables[0].Rows.Count; i++)
	//        {
	//            t.IdPrefijo = (uint)DSNumerosAbonados.Tables[0].Rows[i].ItemArray[1];
	//            t.IdAbonado = (string)DSNumerosAbonados.Tables[0].Rows[i].ItemArray[2];
	//            if (ServicioCD40.InsertSQL(t) < 0) logDebugView.Warn("(Sector-GuardarUsuariosAbonados): No se ha podido guardar el UsuarioAbonado");
	//        }

	//        ServicioCD40.RegeneraParametros(sistema, elnucleo, sector);
	//    }
	//    catch (Exception ex)
	//    {
	//        logDebugView.Error("(Sector-GuardarUsuariosAbonados): ", ex);
	//    }
	//}

    /// <summary>
    /// 
    /// </summary>
    private void CargarPrefijos()
    {
        try
        {
            DListPrefijos.Items.Clear();
            ServiciosCD40.Redes t = new ServiciosCD40.Redes();
            t.IdSistema = (string)Session["idsistema"];
			DataSet d = ServicioCD40.DataSetSelectSQL(t);
			DListPrefijos.DataTextField = "IdRed";
			DListPrefijos.DataValueField = "IdPrefijo";
			DListPrefijos.DataSource = d;
			DListPrefijos.DataBind();

			DDLPrefijoAgenda.DataTextField = "IdRed";
			DDLPrefijoAgenda.DataValueField = "IdPrefijo";
			DDLPrefijoAgenda.DataSource = d;
			DDLPrefijoAgenda.DataBind();
		}
        catch (Exception ex)
        {
            logDebugView.Error("(Sector-CargarPrefijos): ", ex);
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
	protected void OnButton_Click(object sender, EventArgs e)
	{
		Button ibSelected = (Button)sender;

		switch (ibSelected.ID)
		{
			case "IBPropiedadesGenerales":
                IBPropiedadesGenerales.CssClass = "ButtonImageSelected";
                IBUtilidades.CssClass = "ButtonImage";
                IBNivelesIntrusion.CssClass = "ButtonImage";
                IBPermisosRedes.CssClass = "ButtonImage";
                IBAgenda.CssClass = "ButtonImage";
				MultiView1.ActiveViewIndex = NumPaginaActiva = 0;
                Panel1.Height = 350;
                Panel1.Width = 761;
				break;
			case "IBUtilidades":
				ObtenerTeclasSector((string)Session["idsistema"], TxtIdSector.Text, DListNucleo.Text);

                IBPropiedadesGenerales.CssClass = "ButtonImage";
                IBUtilidades.CssClass = "ButtonImageSelected";
                IBNivelesIntrusion.CssClass = "ButtonImage";
                IBPermisosRedes.CssClass = "ButtonImage";
                IBAgenda.CssClass = "ButtonImage";
                MultiView1.ActiveViewIndex = NumPaginaActiva = 1;
                Panel1.Height = 255;
                Panel1.Width = 761;
				break;
			case "IBPermisosRedes":
                //ObtenerPermisosRedes((string)Session["idsistema"], TxtIdSector.Text);
                HabilitaCheckBoxPermisos(BtAceptar.Visible);

                IBPropiedadesGenerales.CssClass = "ButtonImage";
                IBUtilidades.CssClass = "ButtonImage";
                IBNivelesIntrusion.CssClass = "ButtonImage";
                IBPermisosRedes.CssClass = "ButtonImageSelected";
                IBAgenda.CssClass = "ButtonImage";
				MultiView1.ActiveViewIndex = NumPaginaActiva = 2;
                Panel1.Height = 255;
                Panel1.Width = 545;
				break;
			case "IBAgenda":
         //       if (!Modificando)
		 //		    CargarAgenda((string)Session["idsistema"], TxtIdSector.Text, DListNucleo.Text);

                IBPropiedadesGenerales.CssClass = "ButtonImage";
                IBUtilidades.CssClass = "ButtonImage";
                IBNivelesIntrusion.CssClass = "ButtonImage";
                IBPermisosRedes.CssClass = "ButtonImage";
                IBAgenda.CssClass = "ButtonImageSelected";

                MultiView1.ActiveViewIndex = NumPaginaActiva = 3;
                Panel1.Height = 255;
                Panel1.Width = 761;
				break;
            
            case "IBNivelesIntrusion":

                //MVO 20170707: La pestaña Niveles de Intrusión se mueve a a la última posición, y se deshabilita y oculta el botón asociado ID="IBNivelesIntrusion" en Sector.aspx 
                ObtenerNiveles((string)Session["idsistema"], TxtIdSector.Text, DListNucleo.Text);

                IBPropiedadesGenerales.CssClass = "ButtonImage";
                IBUtilidades.CssClass = "ButtonImage";
                IBNivelesIntrusion.CssClass = "ButtonImageSelected";
                IBPermisosRedes.CssClass = "ButtonImage";
                IBAgenda.CssClass = "ButtonImage";
                MultiView1.ActiveViewIndex = NumPaginaActiva = 4;
                Panel1.Height = 255;
                Panel1.Width = 545;

                break;
            default:
                break;
		}
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
	protected void GVAbonados_OnRowDeleting(object sender, GridViewDeleteEventArgs e)
	{
        _NumAbonadoToDelete = (string)e.Values["IdAbonado"];
		DSNumerosAbonados.Tables[0].Rows[GVAbonados.PageIndex * GVAbonados.PageSize + e.RowIndex].Delete();
		DSNumerosAbonados.AcceptChanges();

		GVAbonados.DataSource = DSNumerosAbonados;
		GVAbonados.DataBind();
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
	protected void GVAbonados_OnSelectedIndexChanging(object sender, GridViewPageEventArgs e)
	{
		GVAbonados.PageIndex = e.NewPageIndex;
		GVAbonados.DataSource = DSNumerosAbonados;
		GVAbonados.DataBind();
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
	protected void LkBRangos_OnClick(object sender, EventArgs e)
	{
		GVRangos.Visible = !GVRangos.Visible;
		GVAbonados.Visible = !GVRangos.Visible;

		//Se obtiene la etiqueta adecuada del fichero de recursos local correspondiente
        LkBRangos.Text = ObtenerEtiquetaRangos(GVRangos.Visible);

        if (GVRangos.Visible && ExisteCentralATSPropia())
        {
            //Se obtiene la cadena Rangos de la Central del fichero de recursos
            if (null != GetLocalResourceObject("LbRangosCentral.Text"))
            {
                Label18.Text = GetLocalResourceObject("LbRangosCentral.Text").ToString() + IdCentralPropia;
            }
            else
                Label18.Text = "Rangos de la central: " + IdCentralPropia;

            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            ServiciosCD40.Rangos r = new ServiciosCD40.Rangos();

            r.IdSistema = s.Value;
            r.Central = IdCentralPropia;
            DataSet rangoCentralPropia = ServicioCD40.DataSetSelectSQL(r);
            GVRangos.DataSource = rangoCentralPropia;
            GVRangos.DataBind();
        }
        else
        {
            if (null != GetLocalResourceObject("Label18Resource1.Text"))
            {
                Label18.Text=GetLocalResourceObject("Label18Resource1.Text").ToString();
            }
            else
                Label18.Text = "Abonados a los que atiende:";
        }
	}

    //Obtiene la cadena de la etiqueta LkBRangos.Text correspondiente del fichero de recursos
    private string ObtenerEtiquetaRangos(bool bVisible)
    {
        System.Text.StringBuilder strEtiq = new System.Text.StringBuilder();

        try
        {
            if (bVisible)
            {
                strEtiq.Append(GetLocalResourceObject("LkBOcultarRangosResource1.Text").ToString());
            }
            else
            {
                strEtiq.Append(GetLocalResourceObject("LkBRangosResource1.Text").ToString());
            }
        }
        catch
        {
            strEtiq.Append( bVisible ? "Ocultar Rangos" : "Ver Rangos");
        }

        return strEtiq.ToString();
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="habilita"></param>
    protected void HabilitaCheckBoxPermisos(bool habilita)
    {
        // Para cada fila del GVPermisosRedes, habilita los dos checkbox
        for (int i = 0; i < GVPermisosRedes.Rows.Count; i++)
        {
            ((WebControl)(GVPermisosRedes.Rows[i].Cells[1].Controls[0])).Enabled = habilita;
            ((WebControl)(GVPermisosRedes.Rows[i].Cells[2].Controls[0])).Enabled = habilita;
        }
    }


    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
	protected void DListPrefijos_SelectedIndexChange(object sender, EventArgs e)
	{
		LkBRangos.Visible = DListPrefijos.SelectedValue == "3";
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
	protected void GVAgenda_OnSelectedIndexChanging(object sender, GridViewPageEventArgs e)
	{
		GVAgenda.PageIndex = e.NewPageIndex;
		GVAgenda.DataSource = DSAgenda;
		GVAgenda.DataBind();
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
	protected void GVagenda_OnRowDeleting(object sender, GridViewDeleteEventArgs e)
	{
		DSAgenda.Tables[0].Rows[GVAgenda.PageIndex * GVAgenda.PageSize + e.RowIndex].Delete();
		DSAgenda.AcceptChanges();

		GVAgenda.DataSource = DSAgenda;
		GVAgenda.DataBind();
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
	protected void OnBtnAnadirAgenda_Click(object sender, EventArgs e)
	{
        //MVO: Se añade la entrada a la agenda si el Nombre, el prefijo y el número están informados
        if (!string.IsNullOrWhiteSpace(TBNombreAgenda.Text) && !string.IsNullOrWhiteSpace(TBNumeroAgenda.Text) && !string.IsNullOrEmpty(DDLPrefijoAgenda.SelectedValue))
        {
            //Si el prefijo es la red ATS, se requiere que el número tenga 6 dígitos y el primer dígito tome el valor 2 o 3.
            if ((string.Compare(DDLPrefijoAgenda.SelectedItem.Text, "ATS", true) == 0) && (!Regex.IsMatch(TBNumeroAgenda.Text, @"^(2|3)([0-9]{5})$")))
            {
                // El numero ATS no tiene el formato correcto
                cMsg.alert((string)GetGlobalResourceObject("Espaniol", "NumATSAgendaIncorrecto"));
            }
            else
            {
                DataRow dr = DSAgenda.Tables[0].NewRow();
                dr[0] = (string)Session["idsistema"];
                dr[1] = DListNucleo.SelectedValue;
                dr[2] = TxtIdSector.Text;
                dr[4] = TBNumeroAgenda.Text;
                dr[3] = Convert.ToInt32(DDLPrefijoAgenda.SelectedValue);
                dr[5] = TBNombreAgenda.Text;
                DSAgenda.Tables[0].Rows.Add(dr);
                DSAgenda.AcceptChanges();
                GVAgenda.Visible = true;
                GVAgenda.DataSource = DSAgenda;
                GVAgenda.DataBind();

                if (CbCompartir.Checked)
                {
                    foreach (ServiciosCD40.Sectores d in datos)
                    {
                        ServiciosCD40.Agenda ag = new ServiciosCD40.Agenda();
                        ag.IdSistema = (string)Session["idsistema"];
                        ag.IdNucleo = d.IdNucleo;//JOI2022 DListNucleo.SelectedValue;
                        ag.IdSector = d.IdSector;
                        ag.Numero = TBNumeroAgenda.Text;
                        ag.Prefijo = Convert.ToUInt32(DDLPrefijoAgenda.SelectedValue);
                        ag.Nombre = TBNombreAgenda.Text;

                        ServicioCD40.InsertSQL(ag);
                    }
                }

                TBNumeroAgenda.Text = TBNombreAgenda.Text = string.Empty;
                DDLPrefijoAgenda.SelectedIndex = 0;
            }
        }
        else
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "AddNumAgendaRequerido"));

        }

    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="idSistema"></param>
    /// <param name="idSector"></param>
    /// <param name="idNucleo"></param>
    private void ObtenerPermisosRedes(string idSistema, string idSector)
    {
        if (DSPermisosRedes != null)
            DSPermisosRedes.Clear();

        DSPermisosRedes = ServicioCD40.PermisoRedesSector(idSistema, idSector);

        GVPermisosRedes.Visible = true;

        if (DSPermisosRedes != null && DSPermisosRedes.Tables.Count > 0)
        {
            GVPermisosRedes.DataSource = DSPermisosRedes;
            GVPermisosRedes.DataBind();
        }
    }

    protected void GridView_OnSelectedIndexChange(object sender, GridViewPageEventArgs e)
    {
        for (int i = GVPermisosRedes.PageIndex * GVPermisosRedes.PageSize, r = 0; r < GVPermisosRedes.Rows.Count ; i++, r++)
        {
            DSPermisosRedes.Tables[0].Rows[i][1] = ((CheckBox)(GVPermisosRedes.Rows[r].Cells[1].Controls[0])).Checked;
            DSPermisosRedes.Tables[0].Rows[i][2] = ((CheckBox)(GVPermisosRedes.Rows[r].Cells[2].Controls[0])).Checked;
        }

        GVPermisosRedes.PageIndex = e.NewPageIndex;
        GVPermisosRedes.DataSource = DSPermisosRedes;
        GVPermisosRedes.DataBind();

        HabilitaCheckBoxPermisos(BtAceptar.Visible);
    }

	//protected void GuardarAgenda()
	//{
	//    ServiciosCD40.Agenda ag = new ServiciosCD40.Agenda();
	//    ag.IdSistema = (string)Session["idsistema"];
	//    ag.IdNucleo = DListNucleo.SelectedValue;
	//    ag.IdSector = TxtIdSector.Text;

	//    if (ServicioCD40.DeleteSQL(ag) < 0) logDebugView.Warn("(Sector-GuardarAgenda): No se ha podido eliminar la agenda");

	//    for (int i = 0; i < DSAgenda.Tables[0].Rows.Count; i++)
	//    {
	//        ag.Nombre = (string)DSAgenda.Tables[0].Rows[i].ItemArray[5];
	//        ag.Prefijo = (uint)DSAgenda.Tables[0].Rows[i].ItemArray[3];
	//        ag.Numero = (string)DSAgenda.Tables[0].Rows[i].ItemArray[4];
	//        if (ServicioCD40.InsertSQL(ag) < 0) logDebugView.Warn("(Sector-GuardarAgenda): No se ha podido guardar la agenda");
	//    }

	//    RegeneraSectorizacionPorAgenda = false;
	//}

	// Respuesta a la regeneración de las sectorizaciones
    /// <summary>
    /// 
    /// </summary>
    /// <param name="result"></param>
	private void OnCallBackCompleted(IAsyncResult result)
	{
		try
		{
			int retorno = ServicioCD40.EndRegeneraSectorizaciones(result);
			Session.Add("Sectorizando", false);
		}
		catch (System.Web.Services.Protocols.SoapException soapException)
		{
			logDebugView.Error("(Sector-OnCallBackCompleted): ", soapException);
		}
	}
    protected void CheckIntrusion_CheckedChanged(object sender, EventArgs e)
    {
        if (!((CheckBox)sender).Checked)
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "NoTeclaPrio"));
        }
    }

    private void ObtenerNumeroMaximoSectores()
    {
        // Obtener los parámetros del sistema que afectan a los paneles de los sectores      
        ServiciosCD40.Sistema sistema = new ServiciosCD40.Sistema();
        sistema.IdSistema = (string)Session["idsistema"];
        ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(sistema);
        if (d != null && d.Length > 0)
        {
            NumMaxSectores = ((ServiciosCD40.Sistema)d[0]).NumMaxSectores;
        }
        else
        {
            NumMaxSectores = NUM_MAX_SECTORES;
        }
    }

    protected void BtnCancelarBorrarSector_OnClick(object sender, EventArgs e)
    {
        PanelBorrarSector.Visible = false;
        HabilitarFunciones();
    }

    protected void BtnAceptarBorrarSector_Click(object sender, EventArgs e)
    {

        PanelBorrarSector.Visible = false;
        HabilitarFunciones();
        EliminarElemento();
    }

    private void BloquearFunciones()
    {
        Panel1.Visible = false;
        BtLC.Visible = false;
        BtTelefonia.Visible = false;
        BtRadio.Visible = false;
        BtAceptar.Visible = false;
        BtCancelar.Visible = false;
        BtModificar.Visible = false;
        BtNuevo.Visible = false;
        BtEliminar.Visible = false;
        ListBox1.Enabled = false;
    }

    private void HabilitarFunciones()
    {
        Panel1.Visible = true;
        BtLC.Visible = true;
        BtTelefonia.Visible = true;
        BtRadio.Visible = true;
        BtNuevo.Visible = BtModificar.Visible = BtEliminar.Visible = PermisoSegunPerfil;
        ListBox1.Enabled = true;
    }

    /// <summary>
    /// Filtro de búsqueda para Núcleos
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void FiltroBusquedaNucleo_SelectedIndexChanged(object sender, EventArgs e)
    {
        //Reset de todos los elementos
        ArrayList ListBoxArray = new ArrayList();
        DListNucleoSel.Visible = false;
        ButtonFiltroBuscarNucleo.Visible = false;
        DListNucleoSel.Enabled = false;

        switch (FiltroBusquedaNucleo.SelectedIndex)
        {
            case 1:// Todos
                MuestraDatos(DameDatos());
                break;
            case 2:// Nombre Nucleo
                DListNucleoSel.Visible = true;
                DListNucleoSel.Enabled = true;
                ButtonFiltroBuscarNucleo.Visible = true;
                break;
            case 3://AlfaB Desc
                for (int i = 0; i < ListBox1.Items.Count; i++)
                    ListBoxArray.Add(ListBox1.Items[i].Value);

                ListBox1.Items.Clear();
                ListBoxArray.Sort();
                ListBox1.DataSource = ListBoxArray;
                ListBox1.DataBind();
                break;
            case 4://AlfaB Asc
                for (int i = 0; i < ListBox1.Items.Count; i++)
                    ListBoxArray.Add(ListBox1.Items[i].Value);

                ListBox1.Items.Clear();
                ListBoxArray.Sort(new ReverseSort());
                ListBox1.DataSource = ListBoxArray;
                ListBox1.DataBind();
                break;
        }
    }

    /// <summary>
    /// Busca los núcleos por nombre a través del 'Contains'
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void ButtonFiltroBuscarNucleo_Click(object sender, EventArgs e)
    {
        ListBox1.Items.Clear();

        ServiciosCD40.Tablas[] nu = DameDatos();

        for (int i = 0; i < nu.Length; i++)
        {
            if (((ServiciosCD40.Sectores)nu[i]).IdNucleo != null)
            {
                if (((ServiciosCD40.Sectores)nu[i]).IdNucleo == DListNucleoSel.SelectedItem.Text)
                    ListBox1.Items.Add(((ServiciosCD40.Sectores)nu[i]).IdSector);
            }
        }
        if (ListBox1.Items.Count > 0)
        {
            ListBox1.SelectedIndex = 0;
            _NumAbonadoToDelete = string.Empty;
            BtEliminar.Enabled = true;
            MostrarElemento();
        }
        else
        {
            ListBox1.SelectedIndex = -1;
            MostrarElemento();
        }

        OcultaFiltro(false);
    }

    private void OcultaFiltro(bool bloquea)
    {
        FiltroBusquedaNucleo.SelectedIndex = 0;
        DListNucleoSel.Visible = false;
        ButtonFiltroBuscarNucleo.Visible = false;
        DListNucleoSel.Enabled = false;
        FiltroBusquedaNucleo.Enabled = bloquea ? false : true;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name=></param>
    /// <param name=></param>
    protected bool AsignadoASectorizacion(string strSector)
    {
        bool asignado = false; 
        if (ServicioCD40.SectorAsignadoEnSectorizacion((string)Session["idsistema"], strSector))
        {
            asignado = true;
        }
        return asignado;
    }

    /// <summary>
    /// Retorna sí alcanzado número máximo de sectores asignados a Núcleo
    /// </summary>
    /// <param name=></param>
    /// <param name=></param>
    protected bool AlcanzadoMaximoORsNucleo(string strNucleo)
    {
        int numOrs = 0;
        bool maximo = false;
        ServiciosCD40.Tablas[] nu = DameDatos();

        for (int i = 0; i < nu.Length; i++)
        {
            if (((ServiciosCD40.Sectores)nu[i]).IdNucleo != null)
            {
                if (((ServiciosCD40.Sectores)nu[i]).IdNucleo == strNucleo)
                    numOrs++;
            }
        }
        if (numOrs >= MAX_SECTORES_NUCLEOS)
        {
            maximo = true;
        }
        return maximo;
    }

}
