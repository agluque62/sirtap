using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Configuration;
using System.Web.Security;
using System.Text.RegularExpressions;
using log4net;
using log4net.Config;
using UtilitiesCD40;

using TListaDeSectores = System.Collections.Generic.List<string>;
using TListaDeTops = System.Collections.Generic.Dictionary<string, System.Collections.Generic.List<string>>;
using TListaDeSectorizaciones = System.Collections.Generic.Dictionary<string, System.Collections.Generic.Dictionary<string, System.Collections.Generic.List<string>>>;
// 20210317 #4749
using TListaDeTopAutorizados = System.Collections.Generic.List<string>;
using TListaDeDepartamentosATSAutorizados = System.Collections.Generic.List<string>;
using TListaDeNucleosAutorizados = System.Collections.Generic.List<string>;


// Clase para ordenar un ListBox
class myListItemComparer : System.Collections.IComparer
{
	int System.Collections.IComparer.Compare(object x, object y)
	{
		ListItem item_1 = x as ListItem;
		ListItem item_2 = y as ListItem;

		return item_1.Value.CompareTo(item_2.Value);
	}
}


public partial class Controladores : System.Web.UI.Page, System.Web.UI.ICallbackEventHandler
{
	string _Command;
	static ServiciosCD40.ServiciosCD40 ServiceServiciosCD40;
	static int NumTops = 0;
	static TListaDeSectorizaciones ListaDeSectorizaciones = new TListaDeSectorizaciones();
	static string IdSectorizacionActiva;
	static string TopSeleccionada = string.Empty;
    static string TopSeleccionadaDestino = string.Empty;
    private static AsyncCallback CallbackCompletado;
    // 20210317 #4749
    string IdOperador = "";
    private static bool Confirmando = false;
    private static int  NumTopSeleccionado = 0;
    private static bool SectorizacionSinID = false;
    private const int SACTA_ON = 1;
    private const int SACTA_OFF = 0;
    
    // 20210826 #4806
    private const int SACTA_ON_E = 17;
    private const int SACTA_OFF_E = 16;
    private const int SACTA_GET_SECTORIZATION = 1;
    private static int oldstateSACTA = -1;

    static TListaDeTopAutorizados ListaDeTopAutorizados = new TListaDeTopAutorizados();
    static TListaDeDepartamentosATSAutorizados ListaDeDepATSAutorizados = new TListaDeDepartamentosATSAutorizados();
    static TListaDeNucleosAutorizados ListaDeNucleosAutorizados = new TListaDeNucleosAutorizados();
   

    private static bool ControlDependenciasATS = false;
    private static bool ControlNucleos = true;
    const string ID_SECTORIZACION_SACTA="SACTA"; //Identificador de la Sectorización recibida del Sistema SACTA
    const string ID_SECTORIZACION_SCV = "SCV";   //Identificador interno de la Sectorización activa (Controlador)
    const string ID_SECTOR_TEMPORAL_OPERADOR = "TEMPORARY_CONTROLLER_SCTZ";
    
    // RQF-4
    private string STR_SECTOR_FS = "**FS**";

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

    protected void Page_Load(object sender, EventArgs e)
	{
        if (!Context.Request.IsAuthenticated)
        {
            Response.Redirect("~/Login.aspx",false);
            return;
        }
        if (Context.Request.IsAuthenticated)
		{
			// retrieve user's identity from httpcontext user 
			FormsIdentity ident = (FormsIdentity)Context.User.Identity;
			string perfil = ident.Ticket.UserData;
            IdOperador = ident.Name;
			if (perfil != "0")
			{
				Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO");
				return;
			}
		}
		else if (!IsPostBack)
		{
			Response.Redirect("~/Login.aspx");
			return;
		}

        if (CallbackCompletado == null)
            CallbackCompletado = new AsyncCallback(OnCallBackCompleted);

        if (ServiceServiciosCD40 == null)
            ServiceServiciosCD40 = new ServiciosCD40.ServiciosCD40();
    
		string cbReference = ClientScript.GetCallbackEventReference(this, "arg", "ReceiveServerData", "context", "ClientCallbackError", true);
		string callbackScript = "function CallServer(arg, context) {" + cbReference + "; }";
		ClientScript.RegisterClientScriptBlock(this.GetType(), "CallServer", callbackScript, true);
        Traduce();

		if (!IsPostBack)
		{
			Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
			KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            // 20210317 #4749
            #region ControlDependenciasATS
            // RFQ-4
            /*ControlDependenciasATS = SistemaConDependenciasATS();
            if (ControlDependenciasATS)
            {
                RecuperaTopsAutorizados(IdOperador, s.Value);
            }*/
            ControlDependenciasATS = false;
            #endregion ControlDependenciasATS
            if (ControlNucleos)
            {
                RecuperaNucleosAutorizados(IdOperador, s.Value);
            }

			RecuperaIdSectorizacionActiva(s.Value);
			RecuperaTops(s.Value);
			RecuperaSectoresActiva(s.Value);
			RecuperaSectorizaciones(s.Value);
			RecuperaSectoresSectorizaciones(s.Value);
            BloqueaTopsPorNucleo(s.Value);
			MuestraEstadoSectorizacion();
		}
		else
			EliminaFilas();
	}


	private void MuestraEstadoSectorizacion()
	{
		if (LblIdSectorizacion.Text == IdSectorizacionActiva)
		{
			LblEstado.ForeColor = System.Drawing.Color.Green;
            LblEstado.Text = (string)GetGlobalResourceObject("Espaniol", "TLBACTI");
		}
		else
		{
			LblEstado.ForeColor = System.Drawing.Color.White;
            LblEstado.Text = (string)GetGlobalResourceObject("Espaniol", "TLBNACTI");
		}
	}

	private void RecuperaSectoresSectorizaciones(string idSistema)
	{
		foreach (KeyValuePair<string,Dictionary<string, System.Collections.Generic.List<string>>> s in ListaDeSectorizaciones)
		{
			System.Data.DataSet ds = ServiceServiciosCD40.ControladoresRecuperaSectoresSectorizacion(idSistema, s.Key);
			
			if (ds != null && ds.Tables.Count>0)
			{
				foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
				{
					if (!s.Value.ContainsKey((string)dr["IdTop"]))
						s.Value.Add((string)dr["IdTop"], new List<string>());

					if (dr["IdSector"]!= System.DBNull.Value)
					{
						System.Data.DataSet  t = ServiceServiciosCD40.ControladoresRecuperaListaSectores(idSistema, (string)dr["IdNucleo"], (string)dr["IdSector"]);
                        for (int i = 0; t.Tables.Count > 0 && i < t.Tables[0].Rows.Count; i++)
                            s.Value[(string)dr["IdTop"]].Add((string)t.Tables[0].Rows[i]["IdSectorOriginal"]);
					}
				}
			}
		}
	}

	private void RecuperaSectorizaciones(string idSistema)
	{
		System.Data.DataSet ds = ServiceServiciosCD40.ControladoresRecuperaSectorizaciones(idSistema);

        if (ds != null && ds.Tables.Count > 0)
		{
			LBSectorizaciones.Items.Clear();
            LBEliminar.Items.Clear();
            ListaDeSectorizaciones.Clear();

			foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
			{ 
				// Evitar que aparezca la sectorización activa y SACTA
				if (((string)dr["IdSectorizacion"] != ((DateTime)dr["FechaActivacion"]).ToString("dd/MM/yyyy HH:mm:ss")) &&
                    ((string)dr["IdSectorizacion"] != "SACTA") &&
                    ((string)dr["IdSectorizacion"] != "SCV") &&
                    ((string)dr["IdSectorizacion"] != "TEMPORARY_CONTROLLER_SCTZ"))
				{
					LBSectorizaciones.Items.Add((string)dr["IdSectorizacion"]);
                    LBEliminar.Items.Add((string)dr["IdSectorizacion"]);
					TListaDeTops l = new Dictionary<string, TListaDeSectores>();
					ListaDeSectorizaciones[(string)dr["IdSectorizacion"]] = l;
				}
			}
		}
	}

	private void RecuperaIdSectorizacionActiva(string idSistema)
	{
		System.Data.DataSet ds = ServiceServiciosCD40.ControladoresRecuperaIdSectorizacionActiva(idSistema);
		if (ds != null && ds.Tables.Count>0 &&  ds.Tables[0].Rows.Count>0 )
		{
			IdSectorizacionActiva = LblIdSectorizacion.Text = (string)ds.Tables[0].Rows[0]["IdSectorizacion"];
		}
	}

	private void RecuperaSectoresActiva(string idSistema)
	{
		System.Data.DataSet ds = ServiceServiciosCD40.ControladoresRecuperaSectoresActiva(idSistema);
		MuestraSectores(ds);
	}

	private void RecuperaSectoresSectorizacion(string idSectorizacion)
	{
		Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
		KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];

		System.Data.DataSet ds = ServiceServiciosCD40.ControladoresRecuperaSectoresSectorizacion(s.Value, idSectorizacion);

		MuestraSectores(ds);
		MuestraEstadoSectorizacion();
	}

	private void RecuperaListaSectores(string idSistema, string idNucleo, string idSector, int indiceTerminal)
	{
        bool desbloqueadoATS = true;
        bool desbloqueadoNucleo = true;
        bool bloquea_seltodos = false; 
		Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
		KeyValueConfigurationElement mostrarMantto = config.AppSettings.Settings["MostrarMantenimiento"];
		KeyValueConfigurationElement mostrarVirtuales = config.AppSettings.Settings["MostrarVirtuales"];
        #region ControlDependenciasATS
        // 20210317 #4749
        /*if (ControlDependenciasATS)
        {
            CheckBox cb = (CheckBox)Table1.Rows[indiceTerminal].Cells[1].FindControl("CBTop" + (indiceTerminal + 1));
            if (cb != null)
            {
                if (cb.Enabled == false)
                {
                    desbloqueadoATS = false;
                }
            }
        }*/
        #endregion ControlDependenciasATS

        #region ControlNucleos
        // RQF-49
        if (ControlNucleos)
        {
            CheckBox cb = (CheckBox)Table1.Rows[indiceTerminal].Cells[1].FindControl("CBTop" + (indiceTerminal + 1));
            if (cb != null)
            {
                if (cb.Enabled == false)
                {
                    desbloqueadoNucleo = false;
                }
            }
        }
        #endregion ControlNucleos


        System.Data.DataSet listaSectores = ServiceServiciosCD40.ControladoresRecuperaListaSectores(idSistema, idNucleo, idSector);
		CheckBoxList cblSectores = (CheckBoxList)Table1.Rows[indiceTerminal].Cells[4].FindControl("CBLSectores" + (indiceTerminal + 1));
		for (int i = 0; listaSectores.Tables.Count > 0 && i <  listaSectores.Tables[0].Rows.Count; i++)
		{
			if (cblSectores != null)
			{
                ListItem item = new ListItem((string)listaSectores.Tables[0].Rows[i]["IdSectorOriginal"]);
                item.Value=(string)listaSectores.Tables[0].Rows[i]["Tipo"];
                #region ControlDependenciasATS
                // 20210317 #4749
                //if (ControlDependenciasATS)
                //{
                 //   item.Enabled = desbloqueadoATS;
                //}
                #endregion ControlDependenciasATS
                #region ControlNucleosS
                // 20210317 #4749
                if (ControlNucleos)
                {
                    item.Enabled = desbloqueadoNucleo;
                }
                #endregion ControlNucleosS
                cblSectores.Items.Add(item);
			}
		}

        //bloquea_seltodos = ReordenaCheckBoxList(cblSectores, idNucleo, desbloqueadoATS);
        bloquea_seltodos = ReordenaCheckBoxList(cblSectores, idNucleo, desbloqueadoNucleo);
        #region ControlDependenciasATS
        /*
        if (ControlDependenciasATS)
        {
            CheckBox rball = (CheckBox)Table1.Rows[indiceTerminal].Cells[5].FindControl("CBtodos" + (indiceTerminal + 1));
            if (rball != null)
            {
                if (bloquea_seltodos == true && desbloqueadoATS == true)
                {
                    rball.Enabled = false;
                }
                else if (bloquea_seltodos == false && desbloqueadoATS == true)
                {
                    rball.Enabled = true;
                }
            }
        }*/
        #endregion ControlDependenciasATS

        #region ControlNucleos
        
        if (ControlNucleos)
        {
            CheckBox rball = (CheckBox)Table1.Rows[indiceTerminal].Cells[5].FindControl("CBtodos" + (indiceTerminal + 1));
            if (rball != null)
            {
                if (bloquea_seltodos == true && desbloqueadoNucleo == true)
                {
                    rball.Enabled = false;
                }
                else if (bloquea_seltodos == false && desbloqueadoNucleo == true)
                {
                    rball.Enabled = true;
                }
            }
        }
        #endregion ControlNucleos
    }

	private void RecuperaTops(string idSistema)
	{
		System.Data.DataSet ds = ServiceServiciosCD40.ControladoresRecuperaTops(idSistema);
        if (ds != null && ds.Tables.Count > 0)
		{
			NumTops = ds.Tables[0].Rows.Count;

			int i = 0;
			foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
			{
				CheckBox cb = (CheckBox)Table1.Rows[i++].Cells[1].FindControl("CBTop" + i);
				if (cb != null)
				{
					cb.Text = (string)dr["IdTop"];

                    CheckBox rball = (CheckBox)Table1.Rows[i - 1].Cells[5].FindControl("CBtodos" + i);
                    if (rball != null)
                    {
                        rball.Text = (string)GetGlobalResourceObject("Espaniol", "TRDTODOS");
                    }
                    #region ControlDependenciasATS
                    //20210317 #4749
                    /*
                    if (ControlDependenciasATS)
                    {
                        if (!ListaDeTopAutorizados.Contains(cb.Text))
                        {
                            cb.Enabled = false;
                            RadioButton rbTop = (RadioButton)Table1.Rows[i - 1].Cells[0].FindControl("RadioButton" + i);
                            if (rbTop != null)
                            {
                                rbTop.Enabled = false;
                            }

                            //CheckBox rbTodos = (CheckBox)Table1.Rows[i - 1].Cells[5].FindControl("CBtodos" + i);
                            if (rball != null)
                            {
                                rball.Enabled = false;                            
                            }
                        }
                    }*/
                    #endregion ControlDependenciasATS
                }
			}
			int cuantos = Table1.Rows.Count - i;
			for (int numFilas=0; numFilas < cuantos; numFilas++)
			{
				Table1.Rows.RemoveAt(i);
			}
		}
	}

    private void BloqueaTopsPorNucleo(string idSistema)
    {
        System.Data.DataSet ds = ServiceServiciosCD40.ControladoresRecuperaTops(idSistema);
        if (ds != null && ds.Tables.Count > 0)
        {
            NumTops = ds.Tables[0].Rows.Count;

            int i = 0;
            foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
            {
                CheckBox cb = (CheckBox)Table1.Rows[i++].Cells[1].FindControl("CBTop" + i);
                if (cb != null)
                {
                    
                    CheckBox rball = (CheckBox)Table1.Rows[i - 1].Cells[5].FindControl("CBtodos" + i);
                    if (rball != null)
                    {
                        rball.Text = (string)GetGlobalResourceObject("Espaniol", "TRDTODOS");
                    }
                    string sNucleo = (string)Table1.Rows[i - 1].Cells[2].Text;
                    #region ControlNucleo
                    if (ControlNucleos)
                    {
                        if (!ListaDeNucleosAutorizados.Contains(sNucleo))
                        {
                            cb.Enabled = false;
                            RadioButton rbTop = (RadioButton)Table1.Rows[i - 1].Cells[0].FindControl("RadioButton" + i);
                            if (rbTop != null)
                            {
                                rbTop.Enabled = false;
                            }

                            //CheckBox rbTodos = (CheckBox)Table1.Rows[i - 1].Cells[5].FindControl("CBtodos" + i);
                            if (rball != null)
                            {
                                rball.Enabled = false;                            
                            }
                        }
                    }
                    #endregion ControlNucleo
                }
            }
        }
    }


	private void MuestraSectores(System.Data.DataSet ds)
	{
        if (ds != null && ds.Tables.Count > 0)
		{
			ResetTablaSectorizacion();

			foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
			{
				int i = 0;
				foreach (TableRow tr in Table1.Rows)
				{
					CheckBox cb = (CheckBox)tr.Cells[1].FindControl("CBTop" + (i + 1));
					if (cb != null)
					{
						if (cb.Text != (string)dr["IdTop"])
							i++;
						else
							break;
					}
				}
				if (dr["IdSector"] != System.DBNull.Value && dr["IdNucleo"] != System.DBNull.Value)
				{
					Table1.Rows[i].Cells[3].Text = (string)dr["IdSector"];			// Nombre de la agrupación
					Table1.Rows[i].Cells[2].Text = (string)dr["IdNucleo"];			// Núcleo
                    if ((string)dr["IdSector"] != "**FS**")
					    RecuperaListaSectores((string)dr["IdSistema"], (string)dr["IdNucleo"], (string)dr["IdSector"], i);
				}
			}
		}
	}

	private void EliminaFilas()
	{
		int numRows = Table1.Rows.Count;

		for (int numFilas = NumTops; numFilas < numRows; numFilas++)
		{
			Table1.Rows.RemoveAt(NumTops);
		}
	}

	private void ResetTablaSectorizacion()
	{
		int i = 0;
		foreach (TableRow tr in Table1.Rows)
		{
			tr.Cells[3].Text = string.Empty;			// Nombre de la agrupación
			tr.Cells[2].Text = string.Empty;			// Núcleo
			CheckBoxList cblSectores = (CheckBoxList)tr.Cells[4].FindControl("CBLSectores" + (i++ + 1));
			cblSectores.Items.Clear();					// La lista de sectores 
		}
	}

	public void OnClick_BtnCargarSectorizacion(object sender, EventArgs e)
	{
        //if (SACTA_ON == (int)ServiceServiciosCD40.GetEstadoSacta())
        //{
            // 20210826 #4806
        int estadoSacta = 0;
        estadoSacta = (int)ServiceServiciosCD40.GetEstadoSacta();
        if (estadoSacta == SACTA_ON || estadoSacta == SACTA_ON_E)
        {
            Panel13.Visible = true;
            Confirmando = true;
            return;
        }

        if (LblIdSectorizacion.Text == string.Empty)
        {
            BtnGuardar_Click(sender, e);
            return;
            //SectorizacionSinID = true;
            //LblIdSectorizacion.Text = "SCV";
        }
        LBPreguntaSectorizacion.Text = (string)GetGlobalResourceObject("Espaniol", "TActivarSectorizacion");
        TBSectorizacion.Text = LblIdSectorizacion.Text;
        Confirmando = true;
        Panel8.Visible = true;
    }

    // 20210826 #4806
    public void OnClick_BtnCargarSectorizacionSacta(object sender, EventArgs e)
    {
        int estadoSacta = 0;
        estadoSacta  = (int)ServiceServiciosCD40.GetEstadoSacta();
        if (estadoSacta == 0 || estadoSacta == 16)
        {
            Panel13.Visible = true;
            Confirmando = true;
            return;
        }

        LBPreguntaSectorizacionSacta.Text = (string)GetGlobalResourceObject("Espaniol", "TActivarSectorizacionSACTA");
        TBSectorizacionSacta.Text = ID_SECTORIZACION_SACTA;
        Confirmando = true;
        Panel11.Visible = true;
    }

    protected void BtnAceptarSectorizar_Click(object sender, EventArgs e)
    {
        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
        DateTime fechaActivacion = DateTime.Now;

        if (LblIdSectorizacion.Text == "SCV" && SectorizacionSinID == true)
		{
            SectorizacionSinID = false;

            ServiceServiciosCD40.ControladoresRenombraSectorizacion(s.Value, LblIdSectorizacion.Text);

            if (!ServiceServiciosCD40.ComunicaSectorizacionActiva(config.AppSettings.Settings["ListenIP"].Value, s.Value, LblIdSectorizacion.Text, ref fechaActivacion))
            {
                SectorizacionFallida();
                return;
            }
            RecuperaIdSectorizacionActiva(s.Value);
            MuestraEstadoSectorizacion();           
        }
		else
		{
            if (!ServiceServiciosCD40.ComunicaSectorizacionActiva(config.AppSettings.Settings["ListenIP"].Value, s.Value, LblIdSectorizacion.Text, ref fechaActivacion))
            {
                SectorizacionFallida();
                return;
            }
            RecuperaIdSectorizacionActiva(s.Value);
            MuestraEstadoSectorizacion();
        }
        Confirmando = false;
        Panel8.Visible = false;
	}

    protected void BtnCancelarSectorizar_OnClick(object sender, EventArgs e)
    {
        
        if (SectorizacionSinID == true)
        {
            SectorizacionSinID = false;
            LblIdSectorizacion.Text = string.Empty;
        }
        Confirmando = false;
        Panel8.Visible = false;
    }

    protected void SectorizacionFallida()
    {
        LbNoSectorizacion.Text = (string)GetGlobalResourceObject("Espaniol", "NoTodosSectoresReales");
        Confirmando = false;
        LBSectorizacionFallida.Text = (string)GetGlobalResourceObject("Espaniol", "Sectorizacion");
        TBSectorizacionFallida.Text = LblIdSectorizacion.Text;
        Panel8.Visible = false;
        Panel9.Visible = true;
    }

    protected void BtnAceptarSectorizarFallida_Click(object sender, EventArgs e)
    {
        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];

        RecuperaIdSectorizacionActiva(s.Value);

        MuestraEstadoSectorizacion();

        Panel9.Visible = false;
    }
    // 20210826 #4806
    protected void BtnAceptarSectorizarFallidaSACTA_Click(object sender, EventArgs e)
    {
        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];

        RecuperaIdSectorizacionActiva(s.Value);

        MuestraEstadoSectorizacion();

        Panel12.Visible = false;
    }


    protected void BtnSectorizacionRechazada_Click(object sender, EventArgs e)
    {
        Confirmando = false;
        Panel13.Visible = false;
    }

    // 20210826 #4806
    protected void SectorizacionSactaFallida()
    {
        LbNoSectorizacionSACTA.Text = (string)GetGlobalResourceObject("Espaniol", "SectorizacionSactaNoRealizada");
        Confirmando = false;
        LBSectorizacionFallidaSACTA.Text = (string)GetGlobalResourceObject("Espaniol", "Sectorizacion");
        TBSectorizacionFallidaSACTA.Text = LblIdSectorizacion.Text;
        Panel11.Visible = false;
        Panel12.Visible = true;
    }

    // RQF-4
    protected void AgrupacioncionFallida()
    {
        LbNoSectorizacionSACTA.Text = (string)GetGlobalResourceObject("Espaniol", "ErrorAgrupacionDistintosNucleos");
        Confirmando = false;
        LBSectorizacionFallidaSACTA.Text = (string)GetGlobalResourceObject("Espaniol", "Sectorizacion");
        TBSectorizacionFallidaSACTA.Text = LblIdSectorizacion.Text;
        Panel11.Visible = false;
        Panel12.Visible = true;
    }

    // 20210826 #4806
    protected void BtnAceptarSectorizarSacta_Click(object sender, EventArgs e)
    {
        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
        /*if (SACTA_GET_SECTORIZATION != (int)ServiceServiciosCD40.ControladoresGetSectorizationSacta())
        {
            SectorizacionSactaFallida();
            return;
        }*/ 
        Confirmando = false;
        Panel11.Visible = false;
        RecuperaIdSectorizacionActiva(s.Value);
        MuestraEstadoSectorizacion();
    }
    // 20210826 #4806
    protected void BtnCancelarSectorizarSacta_OnClick(object sender, EventArgs e)
    {
        Confirmando = false;
        Panel11.Visible = false;
    }

    protected void BtnCerrar_OnClick(object sender, EventArgs e)
    {
        var sc = (SessionsControl)Application["SessionsControl"];
        if (sc != null)
        {
            sc.Logout(Request, Response, (userout) =>
            {
                Response.Redirect("~/Login.aspx", false);
            });
            Response.Redirect("~/Login.aspx", false);
        }
    }


	public void OnSelectedIndexChange_LBSectorizaciones(object sender, EventArgs e)
	{
		if (LBSectorizaciones.SelectedItem != null)
		{
			LblIdSectorizacion.Text = LBSectorizaciones.SelectedItem.Text;
			RecuperaSectoresSectorizacion(LblIdSectorizacion.Text);
		}
	}

    public void OnSelectedIndexChange_LBEliminar(object sender, EventArgs e)
    {
        if (LBEliminar.SelectedItem != null)
        {

            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"]; 

            // 20210323 #4757
            if (LBEliminar.SelectedValue == IdSectorizacionActiva)
            {
                LBNoBorrarActiva.Text = (string)GetGlobalResourceObject("Espaniol", "TLBNoBorrarActiva");
                ModalPopupExtender1.Show();
                return;
            }
            TextBoxSector.Text = LBEliminar.SelectedValue;
            LBEliminarSectorizacion.Text = (string)GetGlobalResourceObject("Espaniol", "TLBEliminarSectorizacion");
            Panel5.Visible = true;
        }
    }

	public void RBCheckedChanged(object sender, EventArgs e)
	{
        RadioButton rbTop = (RadioButton)sender;
        if (Confirmando == true)
        {
            if (rbTop != null)
            {
                if (NumTopSeleccionado != Convert.ToInt32(rbTop.ID.Remove(0, 11)))
                {
                    rbTop.Checked = false;
                }
            }
            return;
        }
        // Obtener número de fila a partir del id del radiobuttonXX
        NumTopSeleccionado = Convert.ToInt32(rbTop.ID.Remove(0, 11));
        CheckBox cb = (CheckBox)Table1.Rows[NumTopSeleccionado - 1].Cells[1].FindControl("CBTop" + NumTopSeleccionado);
        if (cb != null)
        {
            TextBoxTopDestinoAgrupacion.Text = cb.Text;
            LBTopSeleccionado.Text = (string)GetGlobalResourceObject("Espaniol", "TTopSeleccionado");
            LBAgrupacion.Text = (string)GetGlobalResourceObject("Espaniol", "TLBAgrupacion");
            Confirmando = true;
            Panel7.Visible = true;
        }
    }

    protected void BtnAceptarAgrupar_Click(object sender, EventArgs e)
    {
        AgrupaSectoresEnTop(NumTopSeleccionado);
        Confirmando = false;
        NumTopSeleccionado = 0;
        Panel7.Visible = false;
        LiberaCheckTodos();
    }

    protected void BtnCancelarAgrupar_OnClick(object sender, EventArgs e)
    {
        RadioButton rbTop = (RadioButton)Table1.Rows[NumTopSeleccionado - 1].Cells[0].FindControl("RadioButton" + NumTopSeleccionado);
        if (rbTop != null)
        {
            rbTop.Checked = false;
        }
        rbTop.Checked = false;
        Confirmando = false;
        NumTopSeleccionado = 0;
        Panel7.Visible = false;
    }
    
    public void AgrupaSectoresEnTop(int numTopSel)
    {
        // RQF-4
        string idNucleoDestino = string.Empty;
        string idSectorDestino = string.Empty;
        bool rechazaAgrupacion = false;
        bool destinoenFS = false;

        int numTop = numTopSel;
        RadioButton rbTop = (RadioButton)Table1.Rows[numTopSel - 1].Cells[0].FindControl("RadioButton" + numTopSel);
		CheckBoxList lbTop = (CheckBoxList)Table1.Rows[numTop - 1].Cells[4].FindControl("CBLSectores" + numTop);
        // RQF-4
        // Validación Núcleo destino es el mismo que el nucleo origen o **FS**
        for (int i = 0; i < NumTops; i++)
        {
            if (i != numTop - 1)
            {
                CheckBoxList lbSectores = (CheckBoxList)Table1.Rows[i].Cells[4].FindControl("CBLSectores" + (i + 1));
                if (lbSectores != null)
                {
                    // RQF-4
                    // Validación Núcleo destino es el mismo que el nucleo origen o **FS**
                    foreach (ListItem sector in lbSectores.Items)
                    {
                        if (sector.Selected)
                        {
                            if (!destinoenFS)
                            {
                                if (Table1.Rows[i].Cells[2].Text != Table1.Rows[numTop - 1].Cells[2].Text)
                                {
                                    if (Table1.Rows[numTop - 1].Cells[3].Text != STR_SECTOR_FS)
                                    {
                                        rechazaAgrupacion = true;
                                    }
                                    else
                                    {
                                        destinoenFS = true;
                                        idNucleoDestino = Table1.Rows[i].Cells[2].Text;
                                        Table1.Rows[numTop - 1].Cells[2].Text = Table1.Rows[i].Cells[2].Text; 
                                    }
                                }
                            }
                            else
                            {
                                if (Table1.Rows[i].Cells[2].Text != idNucleoDestino)
                                {
                                    rechazaAgrupacion = true;
                                }
                            }

                        }
                    }
                }
            }
        }

        if (rechazaAgrupacion)
        {
            AgrupacioncionFallida();
            return;
        }

		for (int i = 0; i < NumTops; i++)
		{
			if (i != numTop-1)
			{
				CheckBoxList lbSectores = (CheckBoxList)Table1.Rows[i].Cells[4].FindControl("CBLSectores" + (i + 1));
				if (lbSectores != null)
				{
					// Añadir a la top destino
					foreach (ListItem sector in lbSectores.Items)
					{
						if (sector.Selected)
						{
                            if (sector.Value == "M" && lbTop.Items.Count > 0)
                                
                            {
                                rbTop.Checked = false;

                                ModalPopupExtender2.Show();

                                return;
                            }
                            //20210803 #4863
                            if (lbTop.Items.FindByValue("M") != null)
                            {
                                lbTop.Items.Remove(lbTop.Items.FindByValue("M"));
                            }

							lbTop.Items.Add(sector);
							sector.Selected = false;
						}
					}

					foreach (ListItem sector in lbTop.Items)
					{
						lbSectores.Items.Remove(lbSectores.Items.FindByText(sector.Text));
					}
					// Actualizar el núcleo el top destino
					if (Table1.Rows[numTop - 1].Cells[2].Text == string.Empty)
						Table1.Rows[numTop - 1].Cells[2].Text = Table1.Rows[i].Cells[2].Text;
					// Actualizar el núcleo el top origen
                    // ERROR EN FUERA DE SECTORIZACION? OJOOOOOOOOOO
					//if (lbSectores.Items.Count == 0)
					//	Table1.Rows[i].Cells[2].Text = string.Empty;
				}
			}
		}

		rbTop.Checked = false;

		LblIdSectorizacion.Text = IdentificaSectorizacion();

		if (LblIdSectorizacion.Text == string.Empty)
		{
			RegeneraSectorizacion();	
            RecuperaSectoresSectorizacion("TEMPORARY_CONTROLLER_SCTZ");
        }
		else
			RecuperaSectoresSectorizacion(LblIdSectorizacion.Text);
	}

    private void LiberaCheckTodos()
    {
        int i = 0;
        foreach (TableRow tr in Table1.Rows)
        {
            CheckBox cb = (CheckBox)tr.Cells[5].FindControl("CBTodos" + (i + 1));
            if (cb != null)
            {
               if (cb.Checked == true)
                  cb.Checked = false;
               i++;
            }
        }
    }

	private string IdentificaSectorizacion()
	{
		bool seguir=true;
		string idSectorizacion=string.Empty;
        try
        {
            foreach (KeyValuePair<string, Dictionary<string, System.Collections.Generic.List<string>>> s in ListaDeSectorizaciones)
            {
                seguir = true;
                idSectorizacion = s.Key;

                for (int i = 0; i < NumTops; i++)
                {
                    CheckBoxList cbl = (CheckBoxList)Table1.Rows[i].Cells[4].FindControl("CBLSectores" + (i + 1));
                    CheckBox cb = (CheckBox)Table1.Rows[i].Cells[1].FindControl("CBTop" + (i + 1));

                    if (cb != null && (s.Value[cb.Text].Count != cbl.Items.Count && s.Value[cb.Text].Count > 0 && s.Value[cb.Text][0] != "**FS**"))
                        seguir = false;
                    else if (cbl.Items.Count > 0)
                    {
                        foreach (ListItem item in cbl.Items)
                        {
                            seguir = null != s.Value[cb.Text].Find(delegate(string obj) { return (obj == item.Text); });
                            if (!seguir)
                                break;
                        }
                    }
                    if (!seguir)
                        break;
                }
                if (seguir)
                    break;
            }
        }
        catch (Exception ex)
        {
            logDebugView.Error("(IdentificaSectorizacion()):", ex);
            seguir = false;
        }


		return seguir ? idSectorizacion : string.Empty;
	}

    #region ReordenaCheckBoxList
    /*
	private bool ReordenaCheckBoxList(CheckBoxList lbSectores, string idNucleo, bool desbloqueadoATS)
	{
        bool bloquea_checkall = false;
        System.Text.StringBuilder listaUsuarios = new System.Text.StringBuilder();
        foreach (ListItem s in lbSectores.Items)
            listaUsuarios.AppendFormat("'{0}',", s.Text);

        if (listaUsuarios.Length == 0)
            return bloquea_checkall;

        listaUsuarios = listaUsuarios.Remove(listaUsuarios.Length - 1, 1);
        lbSectores.Items.Clear();

        // Ordenar la lista por el IDSacta
        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement sistema = config.AppSettings.Settings["Sistema"];
         // 20210317 #4749
        if (!ControlDependenciasATS)
        {
            System.Data.DataSet d = ServiceServiciosCD40.SectoresNumSactaSorted(sistema.Value, idNucleo, listaUsuarios.ToString());
            if (d != null && d.Tables.Count > 0)
            {
                foreach (System.Data.DataRow ds in d.Tables[0].Rows)
                {
                    ListItem i = new ListItem((string)ds[0], (string)ds[1]);
                    i.Attributes.Add("style", (string)ds[1] == "M" ? "color:Blue;" : ((string)ds[1] == "R" ? "color:LightGreen;" : "color:orange;"));
                    lbSectores.Items.Add(i);
                }
            }
        }
        else
        {
            System.Data.DataSet d = ServiceServiciosCD40.SectoresDependenciaATS(sistema.Value, idNucleo, listaUsuarios.ToString());
            if (d != null && d.Tables.Count > 0)
            {
                foreach (System.Data.DataRow ds in d.Tables[0].Rows)
                {
                    ListItem i = new ListItem((string)ds[0], (string)ds[1]);
                    i.Attributes.Add("style", (string)ds[1] == "M" ? "color:Blue;" : ((string)ds[1] == "R" ? "color:LightGreen;" : "color:orange;"));
                    if ((ds["IdDependenciaATS"] != DBNull.Value) && (!ListaDeDepATSAutorizados.Contains((string)ds[2])))
                    {
                        i.Enabled = false;
                        if (desbloqueadoATS)
                            bloquea_checkall = true;
                    }
                    lbSectores.Items.Add(i);
                }
            }
        }
        return bloquea_checkall;
	}

    */
    #endregion ReordenaCheckBoxList

    #region ReordenaCheckBoxListNucleos   
	private bool ReordenaCheckBoxList(CheckBoxList lbSectores, string idNucleo, bool desbloqueadoNucleo)
	{
        bool bloquea_checkall = false;
        System.Text.StringBuilder listaUsuarios = new System.Text.StringBuilder();
        foreach (ListItem s in lbSectores.Items)
            listaUsuarios.AppendFormat("'{0}',", s.Text);

        if (listaUsuarios.Length == 0)
            return bloquea_checkall;

        listaUsuarios = listaUsuarios.Remove(listaUsuarios.Length - 1, 1);
        lbSectores.Items.Clear();

        // Ordenar la lista por el IDSacta
        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement sistema = config.AppSettings.Settings["Sistema"];
         // 20210317 #4749
        if (!ControlNucleos)
        {
            System.Data.DataSet d = ServiceServiciosCD40.SectoresNumSactaSorted(sistema.Value, idNucleo, listaUsuarios.ToString());
            if (d != null && d.Tables.Count > 0)
            {
                foreach (System.Data.DataRow ds in d.Tables[0].Rows)
                {
                    ListItem i = new ListItem((string)ds[0], (string)ds[1]);
                    i.Attributes.Add("style", (string)ds[1] == "M" ? "color:Blue;" : ((string)ds[1] == "R" ? "color:LightGreen;" : "color:orange;"));
                    lbSectores.Items.Add(i);
                }
            }
        }
        else
        {
            System.Data.DataSet d = ServiceServiciosCD40.SectoresNumSactaSorted(sistema.Value, idNucleo, listaUsuarios.ToString());
            if (d != null && d.Tables.Count > 0)
            {
                foreach (System.Data.DataRow ds in d.Tables[0].Rows)
                {
                    ListItem i = new ListItem((string)ds[0], (string)ds[1]);
                    i.Attributes.Add("style", (string)ds[1] == "M" ? "color:Blue;" : ((string)ds[1] == "R" ? "color:LightGreen;" : "color:orange;"));
                    if (!ListaDeNucleosAutorizados.Contains((string)idNucleo))
                    {
                        i.Enabled = false;
                        if (desbloqueadoNucleo)
                            bloquea_checkall = true;
                    }
                    lbSectores.Items.Add(i);
                }
            }
        }
        return bloquea_checkall;
	}
    #endregion ReordenaCheckBoxListNucleos

    private void RegeneraSectorizacion()
	{
		int i = 1;
		Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
		KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];

		string[] listaNucleos = new string[NumTops];
		string[] listaTops=new string[NumTops];
		string[][] listaSectores = new string[NumTops][];

		foreach (TableRow tr in Table1.Rows)
		{
			int sctr = 0;
			CheckBoxList lista = ((CheckBoxList)tr.Cells[4].FindControl("CBLSectores" + i));
			int cuantos = lista.Items.Count;
			string[] listaSector = new string[cuantos];
			foreach (ListItem ls in lista.Items)
			{
				listaSector[sctr++] = ls.Text;
			}
            // Nucleo
			listaNucleos[i - 1] = tr.Cells[2].Text;
			CheckBox cb = (CheckBox)tr.Cells[1].FindControl("CBTop" + i);
			if (cb != null)
				listaTops[i - 1] = cb.Text;
			listaSectores[i - 1] = listaSector;
			i++;
		}

        ServiceServiciosCD40.ControladoresRegeneraSectorizacion(s.Value, listaTops, listaSectores, listaNucleos);
    }

	protected void BtnAceptarGuardar_Click(object sender, EventArgs e)
	{
		Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
		KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
		DateTime fechaActivacion = DateTime.Now;

        if (TBNomSectorizacion.Text == IdSectorizacionActiva)
        {
            ModalPopupExtender3.Show();
            return;
        }

        LblIdSectorizacion.Text = TBNomSectorizacion.Text;
      
		ServiceServiciosCD40.ControladoresRenombraSectorizacion(s.Value, LblIdSectorizacion.Text);

        ServiceServiciosCD40.ComunicaSectorizacionActiva(config.AppSettings.Settings["ListenIP"].Value, s.Value, LblIdSectorizacion.Text, ref fechaActivacion);

        RecuperaSectorizaciones(s.Value);
        RecuperaSectoresSectorizaciones(s.Value);
        RecuperaIdSectorizacionActiva(s.Value);
        MuestraEstadoSectorizacion();

        Panel1.Visible = false;
    }

	protected void BtnGuardarSectorizacion_Click(object sender, EventArgs e)
	{
		Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
		KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];

        if (TBNomSectorizacion.Text == IdSectorizacionActiva)
        {
            ModalPopupExtender3.Show();
            return;
        }
        if (TBNomSectorizacion.Text == string.Empty)                             
        {
            return;
        }

        if (!Regex.IsMatch(TBNomSectorizacion.Text, @"^[\w-]*$"))
        {
            TBNomSectorizacion.Text = string.Empty;
            return;
        }

        //Se comprueba que la sectorización no tenga las palabras reservadas SACTA, SCV y Temporal
        //if (string.Compare(TBNomSectorizacion.Text, ID_SECTORIZACION_SACTA) != 0 &&
        //    string.Compare(TBNomSectorizacion.Text, ID_SECTORIZACION_SCV) != 0 &&
        //     string.Compare(TBNomSectorizacion.Text, ID_SECTOR_TEMPORAL_OPERADOR) != 0) // 20210427 #4710
        //Se comprueba que la sectorización no tenga las palabras reservadas SACTA, SCV y Temporal
        if (TBNomSectorizacion.Text == ID_SECTORIZACION_SACTA  ||
             TBNomSectorizacion.Text ==  ID_SECTORIZACION_SCV ||
             TBNomSectorizacion.Text ==  ID_SECTOR_TEMPORAL_OPERADOR) // 20210427 #4710
        {
            TBNomSectorizacion.Text = string.Empty;
            return;
        }

		LblIdSectorizacion.Text = TBNomSectorizacion.Text;
		ServiceServiciosCD40.ControladoresRenombraSectorizacion(s.Value, LblIdSectorizacion.Text);

        RecuperaSectorizaciones(s.Value);
        RecuperaSectoresSectorizaciones(s.Value);

        Panel1.Visible = false;
    }

	protected void BtnGuardar_Click(object sender, EventArgs e)
	{
        if (LblIdSectorizacion.Text != string.Empty)
        {
            return;
        }
        Panel1.Visible = true;
        LBGuardarSectorizacion.Text = (string)GetGlobalResourceObject("Espaniol", "TLBGuardarSectorizacion");
        LBIdentificadorSectorizacion.Text = (string)GetGlobalResourceObject("Espaniol", "TLBIdentificadorSectorizacion");
        BtnGuardarSectorizacion.Visible = true;

		BtnAceptarGuardar.Visible = false;
	}

	protected void BtnAceptarBorrar_Click(object sender, EventArgs e)
	{
        if (TextBoxSector.Text != null)
        {
            ServiciosCD40.Sectorizaciones n = new ServiciosCD40.Sectorizaciones();
            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];

            n.IdSectorizacion = TextBoxSector.Text;
            n.IdSistema = s.Value;

            if (ServiceServiciosCD40.DeleteSQL(n) >= 0)
            {
                RecuperaSectorizaciones(n.IdSistema);
                RecuperaSectoresSectorizaciones(n.IdSistema);
            }    
        }
        Panel5.Visible = false;
    }

	protected void CBTodos_CheckedChanged(object sender, EventArgs e)
	{
        if (Confirmando == true)
        {
            CheckBox cf = (CheckBox)sender;
            if (cf != null)
            {
                cf.Checked = false;
            }
            return;
        }

		CheckBox cbTodos = (CheckBox)sender;
        // Obtener número de fila a partir del id del checkbox TodosXX
		int numTop = Convert.ToInt32(cbTodos.ID.Remove(0, 7));
		CheckBoxList lbTop = (CheckBoxList)Table1.Rows[numTop - 1].Cells[4].FindControl("CBLSectores" + numTop);
		foreach (ListItem i in lbTop.Items)
		{
			i.Selected = cbTodos.Checked;
		}
	}

	protected void CBTop_CheckedChanged(object sender, EventArgs e)
	{
        if (Confirmando == true)
        {
            CheckBox cf = (CheckBox)sender;
            if (cf != null)
            {
                cf.Checked = false;
            }
            return;
        }

		CheckBox cb = (CheckBox)sender;
        if (TopSeleccionada == string.Empty || TopSeleccionada == cb.Text)
        {
            TopSeleccionada = cb.Checked ? cb.Text : string.Empty;
            RecuperaSectoresSectorizacion(LblIdSectorizacion.Text == string.Empty ? "TEMPORARY_CONTROLLER_SCTZ" : LblIdSectorizacion.Text);
        }
        else
        {
            Confirmando = true;
            TextBoxTopOrigen.Text = TopSeleccionada;
            TextBoxTopDestino.Text = TopSeleccionadaDestino = cb.Text;
            LBIntercambio.Text = (string)GetGlobalResourceObject("Espaniol", "TLBIntercambio");
            LBTopOrigen.Text = (string)GetGlobalResourceObject("Espaniol", "TLBTopOrigen");
            LBTopDestino.Text = (string)GetGlobalResourceObject("Espaniol", "TLBTopDestino");
            Panel6.Visible = true;
        }
	}

	private void IntercambiaTops(string top)
	{
		int i = 1;
		CheckBoxList cbl1 = null;
		CheckBoxList cbl2 = null;
        int iTopSeleccionada = 0;
        int iTop = 0;

		// Buscar los sectores de la TopSeleccionada
		foreach (TableRow tr in Table1.Rows)
		{
			CheckBox cb1 = (CheckBox)tr.Cells[1].FindControl("CBTop" + i);

            if (cb1 != null && cb1.Text == TopSeleccionada)
            {
                cbl1 = (CheckBoxList)tr.Cells[4].FindControl("CBLSectores" + i);
                iTopSeleccionada = i;
            }
            else if (cb1 != null && cb1.Text == top)
            {
                cbl2 = (CheckBoxList)tr.Cells[4].FindControl("CBLSectores" + i);
                iTop = i;
            }

			i++;
		}

		ListItem[] s1 = new ListItem[cbl1.Items.Count];
		ListItem[] s2 = new ListItem[cbl2.Items.Count];

		cbl1.Items.CopyTo(s1, 0);
		cbl2.Items.CopyTo(s2, 0);
		cbl1.Items.Clear();
		cbl1.Items.AddRange(s2);
		cbl2.Items.Clear();
		cbl2.Items.AddRange(s1);
	}

    protected void BtnAceptarIntercambiar_Click(object sender, EventArgs e)
	{
        Confirmando = false;
        if (TopSeleccionada != string.Empty && TopSeleccionadaDestino != string.Empty)
        {
            // Intercambiar TOP
            IntercambiaTops(TopSeleccionadaDestino);

            int i = 1;
            TopSeleccionada = string.Empty;

            foreach (TableRow tr in Table1.Rows)
            {
                CheckBox cbox = (CheckBox)tr.Cells[1].FindControl("CBTop" + i);
                if (cbox != null)
                {
                    cbox.Checked = false;
                }

                i++;
            }

            LblIdSectorizacion.Text = IdentificaSectorizacion();

            if (LblIdSectorizacion.Text == string.Empty)
            {
                RegeneraSectorizacion();
                RecuperaSectoresSectorizacion("TEMPORARY_CONTROLLER_SCTZ");
            }
            else
                RecuperaSectoresSectorizacion(LblIdSectorizacion.Text);
            TopSeleccionadaDestino = TopSeleccionada = string.Empty;
            Panel6.Visible = false;
        }
            
    }

    protected void BtnCancelarIntercambiar_OnClick(object sender, EventArgs e)
	{
        int i = 0;
        foreach (TableRow tr in Table1.Rows)
        {
            CheckBox cbox = (CheckBox)tr.Cells[1].FindControl("CBTop" + i);
            if (cbox != null)
            {
                cbox.Checked = false;
            }

            i++;
        }

        TopSeleccionadaDestino = TopSeleccionada = string.Empty;
        Panel6.Visible = false;
        Confirmando = false;
    }

    protected void BtnCancelarMantto_Click(object sender, EventArgs e)
    {
        ModalPopupExtender2.Hide();

        RecuperaSectoresSectorizacion(LblIdSectorizacion.Text);
    }

    protected void BtnCancelarBorradoActiva_Click(object sender, EventArgs e)
    {
        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];  
        ModalPopupExtender1.Hide();
        RecuperaSectorizaciones(s.Value);
        RecuperaSectoresSectorizaciones(s.Value);
    }

	protected void LnkBCancelar_OnClick(object sender, EventArgs e)
	{
        Panel1.Visible = false;
	}

    protected void BtnNoRenActiva_Click(object sender, EventArgs e)
    {
        TBNomSectorizacion.Text = string.Empty;
        ModalPopupExtender3.Hide();
    }
    
    protected void BtnCancelarBorrar_OnClick(object sender, EventArgs e)
    {
        Panel5.Visible = false;
        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
        RecuperaSectorizaciones(s.Value);
        RecuperaSectoresSectorizaciones(s.Value);
    }

    private void OnCallBackCompleted(IAsyncResult result)
    {
        try
        {
            ServiceServiciosCD40.EndControladoresRegeneraSectorizacion(result);
            Application["Sectorizando"] = false;
        }
        catch (System.Web.Services.Protocols.SoapException )
        {
        }
    }
		
    public string GetCallbackResult()
	{
        int EstadoEnlaceSacta = (int)ServiceServiciosCD40.GetEstadoSacta();
        if (EstadoEnlaceSacta == SACTA_OFF || EstadoEnlaceSacta == SACTA_OFF_E)
        {
            EstadoEnlaceSacta = SACTA_OFF;
        }
        else if (EstadoEnlaceSacta == SACTA_ON || EstadoEnlaceSacta == SACTA_ON_E)
        {
            EstadoEnlaceSacta = SACTA_ON;
        }
        if (oldstateSACTA != EstadoEnlaceSacta)
        {
            oldstateSACTA = EstadoEnlaceSacta;
            RecuperaInformacionSacta();
        }
        return string.Format("{0}", EstadoEnlaceSacta);
	}

	public void RaiseCallbackEvent(string eventArgument)
	{
		_Command = eventArgument;
	}

    // 20210826 #4806
    private void RecuperaInformacionSacta()
    {
        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
        RecuperaIdSectorizacionActiva(s.Value);
        MuestraEstadoSectorizacion();
    }
 
    // 20210317 #4749
    private void RecuperaTopsAutorizados(string IdOperador, string IdSistema)
    {
        try
        {
            System.Data.DataSet ds = ServiceServiciosCD40.ControladoresRecuperaTopsAutorizados(IdSistema, IdOperador);
            ListaDeTopAutorizados.Clear();
            ListaDeDepATSAutorizados.Clear();
            if (ds != null && ds.Tables.Count > 0)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    ListaDeTopAutorizados.Add((string)dr["IdTop"]);
                    if (!ListaDeDepATSAutorizados.Contains((string)dr["IdDependenciaATS"]))
                    {
                        ListaDeDepATSAutorizados.Add((string)dr["IdDependenciaATS"]);
                    }
                }
            }
        }
        catch (Exception ex)
        {

        }
    }

    // 20210317 #4749
    private void RecuperaNucleosAutorizados(string IdOperador, string IdSistema)
    {
        try
        {
            System.Data.DataSet ds = ServiceServiciosCD40.ControladoresRecuperaNucleosAutorizados(IdSistema, IdOperador);
            ListaDeNucleosAutorizados.Clear();
        
            if (ds != null && ds.Tables.Count > 0)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    ListaDeNucleosAutorizados.Add((string)dr["IdNucleo"]);
                }
            }
        }
        catch (Exception ex)
        {

        }
    }

    // 20210317 #4749
    public bool SistemaConDependenciasATS()
    {
        const string CONF_KEY_CON_DEPATS = "DependenciasATS";
        bool bConDepATS = false;

        Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement objConf = null;

        //Se obtiene el parametro que indica si se debe o no visualizar las Dependencias ATS
        objConf = config.AppSettings.Settings[CONF_KEY_CON_DEPATS];

        if ((objConf != null) && (!string.IsNullOrEmpty(objConf.Value) && string.Compare(objConf.Value, "SI", true) == 0))
        {
            //El sistema está configurado con gestión de dependencias ATS
            bConDepATS = true;
        }

        return bConDepATS;
    }

    private void Traduce()
    {

        LinkButton1.Text = (string)GetGlobalResourceObject("Espaniol", "TBTCER");
        LBtnGuardar.Text = (string)GetGlobalResourceObject("Espaniol", "TBTGUAR");
        LBtnEliminar.Text = (string)GetGlobalResourceObject("Espaniol", "TBTELIM");
        LBtnSectorizaciones.Text = (string)GetGlobalResourceObject("Espaniol", "TBTSEC");
        BtnCargaSectorizacion.Text = (string)GetGlobalResourceObject("Espaniol", "TBTCARSEC");
        BtnCargaSectorizacionSacta.Text = (string)GetGlobalResourceObject("Espaniol", "TBTCARSECSACTA");

        LBSEC.Text = (string)GetGlobalResourceObject("Espaniol", "TLBSEC");
        LBSAC.Text = (string)GetGlobalResourceObject("Espaniol", "TLBSAC");
        LBEST.Text = (string)GetGlobalResourceObject("Espaniol", "TLBEST");
        THCAGR.Text = (string)GetGlobalResourceObject("Espaniol", "TTHCAGR");
        THCNUC.Text = (string)GetGlobalResourceObject("Espaniol", "TTHCNUC");
        THCPOS.Text = (string)GetGlobalResourceObject("Espaniol", "TTHCPOS");
        THCSEC.Text = (string)GetGlobalResourceObject("Espaniol", "TTHCSEC");
    }

}
