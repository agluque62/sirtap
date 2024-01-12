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
using log4net;
using log4net.Config;

public partial class DestinosTelefonia : PageBaseCD40.PageCD40	// System.Web.UI.Page
{
    //System.Data.DataSet listaDestino;
    static bool Modificando;
    private static ILog _logDebugView;
    /// <summary>
    /// 
    /// </summary>
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
	static bool PermisoSegunPerfil;
    /// <summary>
    /// 
    /// </summary>
	static ServiciosCD40.ServiciosCD40 ServiceServiciosCD40 = new ServiciosCD40.ServiciosCD40();
    /// <summary>
    /// 
    /// </summary>
	private AsyncCallback CallbackCompletado;
    /// <summary>
    /// 
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
			if (perfil == "0")
			{
                Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO", false);
				return;
			}

			PermisoSegunPerfil = perfil != "1";
		}

		if (CallbackCompletado == null)
			CallbackCompletado = new AsyncCallback(OnCallBackCompleted);

		if (!IsPostBack)
        {
            //logDebugView.Debug("Entrando en DestinosTelefonia....");

            IndexListBox1 = -1;

            BAnadir_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "AceptarCambios");
            BCancelar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "CancelarCambios");
            
            MuestraDatos();
			CargaDDL();
			BNuevo.Visible = PermisoSegunPerfil;
		}
        else
        {
            if (Request.Form["SoloEliminaDeTFT"] == "1")
            {
                Request.Form["SoloEliminaDeTFT"].Replace("1", "0");

                EliminaDestinoDeTFT();
                EliminarElemento(true);
            }
        }
    }
    /// <summary>
    /// 
    /// </summary>

    /// <summary>
    /// 
    /// </summary>
    private void MuestraDatos()
    {
        try
        {
            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            Session["idsistema"] = s.Value;

			LBDestinos.DataSource = ServiceServiciosCD40.DestinosDeTelefoniaEnElSistema(s.Value).Tables[0];
			LBDestinos.DataTextField = "IdDestino";
			LBDestinos.DataBind();

			if (LBDestinos.Items.Count > 0)
			{
				ActualizaWebPadre(true);

                if (LBDestinos.Items.FindByText(NewItem) != null)
                {
                    LBDestinos.Items.FindByText(NewItem).Selected = true;
                    IndexListBox1 = LBDestinos.SelectedIndex;
                    NewItem = string.Empty;
                }
                else
                    LBDestinos.SelectedIndex = IndexListBox1 != -1 && IndexListBox1 < LBDestinos.Items.Count ? IndexListBox1 : 0;

				BModificar.Visible = BEliminar.Visible = PermisoSegunPerfil;

				MuestraElemento();
			}
			else
			{
				BModificar.Visible = BEliminar.Visible = false;
			}
        }
        catch (Exception e)
        {
            logDebugView.Error("(DestinosTelefonia-MuestraDatos):", e);
        }
    }


    /// <summary>
    /// 
    /// </summary>
    private void CargaDDL()
    {
        try
        {
            ServiciosCD40.Redes r = new ServiciosCD40.Redes();
            r.IdSistema = (string)Session["idsistema"];
			DDLPrefijo.DataSource = ServiceServiciosCD40.DataSetSelectSQL(r);
            DDLPrefijo.DataTextField = "IdRed";
            DDLPrefijo.DataValueField = "IdPrefijo";
            DDLPrefijo.DataBind();

            ServiciosCD40.GruposTelefonia gTelefonia = new ServiciosCD40.GruposTelefonia();
            gTelefonia.IdSistema = (string)Session["idsistema"];
			DDLGrupo.DataSource = ServiceServiciosCD40.DataSetSelectSQL(gTelefonia);
            DDLGrupo.DataTextField = "IdGrupo";
            DDLGrupo.DataBind();
        }
        catch (Exception e)
        {
            logDebugView.Error("(DestinosTelefonia-CargaDDL):", e);
        }
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BNuevo_Click(object sender, EventArgs e)
    {
        Modificando = false;
        NuevoDestino(true);
        RequiredFieldValidator1.Visible = true;
        RequiredFieldValidator2.Visible = true;
        ValidationSummary1.Visible = true;
        TBRecurso.Text = "";
        DDLRecursos.Visible = false;
        TBRecurso.Visible = false;
        LblRecurso.Visible = false;
        LblRecursosLibres.Visible = false;
        IndexListBox1 = LBDestinos.SelectedIndex;
        // 2021111 #3945
        LBSector.Visible = false;
        LBATS.Visible = false;
        LBNumero.Visible = false;
        TBSector.Visible = false;
        TBATS.Visible = false;
        TBNumero.Visible = false;
        MostrarValidacion();
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="habilita"></param>
    private void NuevoDestino(bool habilita)
    {
        if (habilita)
        {
            LBDestinos.Enabled = false;
            CBSeguro.Enabled = true;
            //Si se está dando de alta un nuevo destino
            if (!Modificando)
            {
                TBDestino.Text = "";
                TBDestino.ReadOnly = false;
                CBSeguro.Checked = false;
                DDLPrefijo.SelectedValue = "-1";
                TBGrupo.Text = "";
                DDLGrupo.SelectedValue = "-1";
                TBAbonado.Text = "";
                if (DDLPrefijo.SelectedValue != "1" && DDLPrefijo.SelectedValue != "3" && DDLPrefijo.SelectedValue != "-1")
                {
                    LbGrupo.Visible = true;
                    TBGrupo.Visible = true;
                    DDLGrupo.Visible = true;
                }
                else
                {
                    LbGrupo.Visible = false;
                    TBGrupo.Visible = false;
                    DDLGrupo.Visible = false;
                }
            }
            else
            {
				RequiredFieldValidator3.Visible = TBRecurso.Visible = LblRecursosLibres.Visible = 
				LblRecurso.Visible = DDLRecursos.Visible = DDLPrefijo.SelectedValue == "32" || DDLPrefijo.SelectedValue == "1";

                if (DDLPrefijo.SelectedValue != "1" && DDLPrefijo.SelectedValue != "3")
                {
                    DDLGrupo.Visible = true;
                    TBGrupo.Enabled = true;
                }
                else
                {
                    DDLGrupo.Visible = false;
                    TBGrupo.Enabled = false;
                }

            }

            Panel1.Enabled = true;
            DDLPrefijo.Enabled = !Modificando;
            BNuevo.Visible = false;
            BEliminar.Visible = false;
            BAnadir.Visible = true;
            BCancelar.Visible = true;
            BModificar.Visible = false;
            DDLRecursos.Enabled = true;
            TBAbonado.Enabled = DDLPrefijo.SelectedValue != "1";
        }
        else
        {
            LBDestinos.Enabled = true;
            TBDestino.Text = "";
            TBDestino.ReadOnly = true;
            DDLPrefijo.SelectedValue = "-1";
            TBGrupo.Text = "";
            DDLGrupo.SelectedValue = "-1";
            TBAbonado.Text = "";
            TBAbonado.Enabled = false;

            BNuevo.Visible = PermisoSegunPerfil;
			BEliminar.Visible = PermisoSegunPerfil && LBDestinos.Items.Count > 0;
            BAnadir.Visible = false;
            BCancelar.Visible = false;
            BModificar.Visible = PermisoSegunPerfil && LBDestinos.Items.Count > 0;
            LBDestinos.SelectedIndex = -1;

            Panel1.Enabled = false;

            if (TBGrupo.Visible)
                TBGrupo.Enabled = false;

            if (DDLGrupo.Visible)
                DDLGrupo.Visible = false;

            DDLPrefijo.Enabled = false;
            DDLRecursos.Visible = false;
            DDLRecursos.Enabled = false;
            LblRecurso.Visible = false;
            LblRecursosLibres.Visible = false;
            TBRecurso.Visible = false;
            RequiredFieldValidator3.Enabled = false;
        }
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BAnadir_Click(object sender, EventArgs e)
    {
        string StrSistema = (string)Session["idsistema"];

        if (!Modificando && TBDestino.Enabled && bIdentificadorAsignado(StrSistema, TBDestino.Text))
        {
            //Existe otro destino radio o de telefonia con el mismo identificaodr
            cMsg.alert((string)GetLocalResourceObject("DestinoExiste.Text"));
            return;
        }

        GuardarCambios();
    }
    /// <summary>
    /// 
    /// </summary>
    private void GuardarCambios()
    {
        NewItem = TBDestino.Text;

        if (Modificando)
        {
            IndexListBox1 = LBDestinos.SelectedIndex;
            ModificaDestino();
        }
        else
        {
            AnadeDestino();

            ActualizaWebPadre(true);
        }

        try
        {
            // Llamada asíncrona para regenerar todas las sectorizaciones.
            Session.Add("Sectorizando", true);
            ServiceServiciosCD40.BeginRegeneraSectorizaciones((string)Session["idsistema"], true, false, true, CallbackCompletado, null);
        }
        catch (Exception ex)
        {
            logDebugView.Error("(TFTRadio-BtLiberar_Click): ", ex);
        }

        NuevoDestino(false);
        MuestraDatos();
        //EsconderMenu();
        //BModificar.Visible = false;
        //BEliminar.Visible = false;
        OcultarValidacion();
    }
    /// <summary>
    /// 
    /// </summary>
    protected void AnadeDestino()
    {
        try
        {

            ServiciosCD40.DestinosExternos dExterno = new ServiciosCD40.DestinosExternos();
            dExterno.IdSistema = (string)Session["idsistema"];
            dExterno.IdDestino = TBDestino.Text;
            dExterno.TipoDestino = 1;
            dExterno.IdPrefijo = Convert.ToUInt32(DDLPrefijo.SelectedValue);
            dExterno.IdAbonado = (TBAbonado.Text.Length > 0) ? TBAbonado.Text : null;
            
            /**
             * AGL 2012.06.15. ID.125. "No se anade el grupo a la tabla de Destinos de Telefonía"
             *      nota: siguiendo el código, no veo donde se hace el mencionado "base.update()"
             */
            // Aunque IdGrupo no es de la tabla DestinosExternos, la clase hereda de 
            // DestinosTelefonía y se hace una llamada a base.Insert()
			dExterno.IdGrupo = (TBGrupo.Text.Length > 0) ? TBGrupo.Text : null;
            dExterno.Seguro = CBSeguro.Checked;        
			ServiceServiciosCD40.AnadeDestino(dExterno, TBRecurso.Text, TBGrupo.Text.Length > 0 && DDLGrupo.Items.FindByText(TBGrupo.Text) == null);
    }
        catch (Exception e)
        {
            logDebugView.Error("(DestinosTelefonia-AnadeDestino):", e);
        }
    }
    /// <summary>
    /// 
    /// </summary>
	protected void ModificaDestino()
	{
        // *** DestinoExternoTelefonía => Modificación en DestinosTelefonía
		ServiciosCD40.DestinosExternos dExterno = new ServiciosCD40.DestinosExternos();
		dExterno.IdSistema = (string)Session["idsistema"];
		dExterno.IdDestino = TBDestino.Text;
		dExterno.TipoDestino = 1;
		dExterno.IdPrefijo = Convert.ToUInt32(DDLPrefijo.SelectedValue);
		dExterno.IdAbonado = (TBAbonado.Text.Length > 0) ? TBAbonado.Text : null;

        /**
         * AGL 2012.06.15. ID.125. "No se anade el grupo a la tabla de Destinos de Telefonía"
         *      nota: siguiendo el código, no veo donde se hace el mencionado "base.update()"
         */
        // Aunque IdGrupo no es de la tabla DestinosExternos, 
        // la clase hereda de DestinosTelefonía y se hace una llamada a base.Update()
		dExterno.IdGrupo = (TBGrupo.Text.Length > 0) ? TBGrupo.Text : null;
        dExterno.Seguro = CBSeguro.Checked;
		ServiceServiciosCD40.ActualizaDestino(dExterno, TBRecurso.Text, TBGrupo.Text.Length > 0 && DDLGrupo.Items.FindByText(TBGrupo.Text) == null);    
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BEliminar_Click(object sender, EventArgs e)
    {
        if (LBDestinos.SelectedValue != "")
        {
            //string texto = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarDestino"), LBDestinos.SelectedValue);
            IndexListBox1 = LBDestinos.SelectedIndex;
            Session["elemento"] = LBDestinos.SelectedValue;
            EliminarElemento(false);
            //cMsg.confirm(texto, "eliminaelemento");
        }
    }

    /// <summary>
    /// 
    /// </summary>
    private void EliminarElemento(bool forced)
    {
        if (LBDestinos.SelectedIndex >= 0 && Session["idsistema"]!=null)
        {
            ServiciosCD40.Destinos d = new ServiciosCD40.Destinos();

            uint iPrefijo= Convert.ToUInt32(DDLPrefijo.SelectedValue);
            string strSistema=string.Empty;
            System.Text.StringBuilder strMsg = new System.Text.StringBuilder();

            strSistema=(string)Session["idsistema"];

            d.IdSistema = strSistema;
            d.IdDestino = TBDestino.Text;
            d.TipoDestino = 1;

            if (iPrefijo == 1 && DestinoLCENAsignadoPanelLC(strSistema, TBDestino.Text, iPrefijo, ref strMsg))
            {
                    cMsg.confirm(strMsg.ToString(), "aceptparam");
                    return;
            }

            if (forced || !DestinoAsignadoATft(strSistema,TBDestino.Text))
            {
                // Liberar el destino del recurso
                if (iPrefijo== 1)   // Si el destino es LCEN
                {
                    ServiciosCD40.RecursosLCEN rLc = new ServiciosCD40.RecursosLCEN();
                    rLc.IdSistema = (string)Session["idsistema"];
                    rLc.IdDestino = TBDestino.Text;
                    rLc.TipoDestino = 1;
                }
                else if (iPrefijo >= 32)
                {
                    ServiciosCD40.RecursosTF rTf = new ServiciosCD40.RecursosTF();
                    rTf.IdSistema = (string)Session["idsistema"];
                    rTf.IdDestino = TBDestino.Text;
                    rTf.TipoDestino = 1;
                }

                if (ServiceServiciosCD40.DeleteSQL(d) < 0)
                    logDebugView.Warn("(DestinosTelefonia-EliminarElemento): No se han borrado los elementos(DeleteSQL)");
                else
                {
                    //Comprobamos si todavía existe el destino de telefonía
                    //En alguna ocasión, me he entrado el destino de telefonía sin el destino correspondiente configurado
                    //Por esta razón, si el registro de la tabla destinostelefonía sigue existiendo se elimina.
                    ServiciosCD40.DestinosTelefonia destTelf = new ServiciosCD40.DestinosTelefonia();
                    destTelf.IdSistema = (string)Session["idsistema"];
                    destTelf.IdDestino = TBDestino.Text;
                    destTelf.TipoDestino = 1;
                    destTelf.IdPrefijo = iPrefijo;

                    ServiciosCD40.Tablas[] lista = ServiceServiciosCD40.ListSelectSQL(destTelf);

                    if (lista != null && lista.Length > 0)
                    {
                        //Borramos el destino
                        ServiceServiciosCD40.DeleteSQL(destTelf);
                    }
                   
                }

                NuevoDestino(false);
                MuestraDatos();

                // Regenerar todas las sectorizaciones excepto la activa porque puede que este destino 
                // estuviera presente en alguna sectorización
                ServiceServiciosCD40.BeginRegeneraSectorizaciones((string)Session["idsistema"], true, true, true, CallbackCompletado, null);
            }
            else
            {
                cMsg.confirm((string)GetGlobalResourceObject("Espaniol", "PanelesConDestinoAsignado"), "SoloEliminaDeTFT");
            }
		}
    }



    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BCancelar_Click(object sender, EventArgs e)
    {
        CancelarCambios();
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
		NuevoDestino(false);
        BAnadir.Visible = false;
        BCancelar.Visible = false;
		BEliminar.Visible = BModificar.Visible = PermisoSegunPerfil && LBDestinos.Items.Count > 0;
		BNuevo.Visible = PermisoSegunPerfil;
        RequiredFieldValidator1.Visible = false;
        RequiredFieldValidator2.Visible = false;
        RequiredFieldValidator3.Visible = false;
        ValidationSummary1.Visible = false;
        OcultarValidacion();
        if (LBDestinos.Items.Count > 0)
            LBDestinos.SelectedIndex = IndexListBox1 != -1 ? IndexListBox1 : 0;

		MuestraDatos();
    }

    /// <summary>
    /// 
    /// </summary>
	private void MuestraElemento()
	{
		try
		{
            uint iIdPrefijo=0;
            // 2021111 #3945
            LBSector.Visible = false;
            LBATS.Visible = false;
            LBNumero.Visible = false;
            TBSector.Visible = false;
            TBATS.Visible = false;
            TBNumero.Visible = false;
            CBSeguro.Enabled = false;
			Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
			KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
			Session["idsistema"] = s.Value;
			System.Data.DataRow dr = ServiceServiciosCD40.DestinosDeTelefoniaEnElSistema(s.Value).Tables[0].Rows[LBDestinos.SelectedIndex];

            BEliminar_ConfirmButtonExtender.ConfirmText = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarDestino"), LBDestinos.SelectedValue);
            
            //DestinosTelefonia dt = new DestinosTelefonia();
			TBDestino.Text = (string)dr["IdDestino"];

            iIdPrefijo=Convert.ToUInt32(dr["IdPrefijo"]);

			DDLPrefijo.SelectedValue = Convert.ToString((uint)dr["IdPrefijo"]);
            
            CBSeguro.Checked = Convert.ToBoolean(dr["Seguro"]);

            if (iIdPrefijo != 1 && iIdPrefijo != 3)
            {
                LbGrupo.Visible = true;
                TBGrupo.Visible = true;
                if (dr["IdGrupo"] != System.DBNull.Value)
                {
                    TBGrupo.Text = (string)dr["IdGrupo"];

                    if (DDLGrupo.Items.FindByText(TBGrupo.Text)!=null)
                        DDLGrupo.SelectedValue = TBGrupo.Text;
                }
                else
                {
                    TBGrupo.Text = "";
                    DDLGrupo.SelectedValue = "-1";
                }
            }
            else
            {
                LbGrupo.Visible = false;
                TBGrupo.Visible = false;
                TBGrupo.Text = "";
                DDLGrupo.SelectedValue = "-1";
            }

			if (dr["IdAbonado"] != System.DBNull.Value)
				TBAbonado.Text = (string)dr["IdAbonado"];
			else
				TBAbonado.Text = "";

			// Buscar recurso asignado al destino
            if (iIdPrefijo == 32)   // PP
			{
				DDLRecursos.Visible = true;
				LblRecursosLibres.Visible = true;
				LblRecurso.Visible = true;
				TBRecurso.Visible = true;
				TBAbonado.Visible = false;
				Label5.Visible = false;

				CompletaDDLRecursos(32);

				ServiciosCD40.RecursosTF rt = new ServiciosCD40.RecursosTF();
				rt.IdSistema = (string)Session["idsistema"];
				rt.IdDestino = TBDestino.Text;
				rt.IdPrefijo = 32;

				ServiciosCD40.Tablas[] recurso = ServiceServiciosCD40.ListSelectSQL(rt);
				if (recurso.Length > 0)
				{
					TBRecurso.Text = ((ServiciosCD40.RecursosTF)recurso[0]).IdRecurso;
				}
				else
				{
					DDLRecursos.SelectedIndex = 0;
					TBRecurso.Text = "";
				}
			}
			else
                if (iIdPrefijo == 1) // LCEN
				{
                    // 2021111 #3945
                    LBSector.Visible = true;
                    LBATS.Visible = true;
                    LBNumero.Visible = true;
                    TBSector.Visible = true;
                    TBATS.Visible = true;
                    TBNumero.Visible = true;

					DDLRecursos.Visible = true;
					LblRecurso.Visible = true;
					LblRecursosLibres.Visible = true;
					TBRecurso.Visible = true;
					TBAbonado.Visible = false;
					Label5.Visible = false;

					CompletaDDLRecursos(1);

					ServiciosCD40.RecursosLCEN rt = new ServiciosCD40.RecursosLCEN();
					rt.IdSistema = (string)Session["idsistema"];
					rt.IdDestino = TBDestino.Text;
					rt.IdPrefijo = 1;

					ServiciosCD40.Tablas[] recurso = ServiceServiciosCD40.ListSelectSQL(rt);
					if (recurso.Length > 0)
					{
						TBRecurso.Text = ((ServiciosCD40.RecursosLCEN)recurso[0]).IdRecurso;
                        bDestinoLCENASector(TBDestino.Text);
					}
					else
					{
						DDLRecursos.SelectedIndex = 0;
						TBRecurso.Text = "";
					}
				}
				else
				{
					TBAbonado.Visible = true;
					Label5.Visible = true;
					DDLRecursos.Visible = false;
					LblRecursosLibres.Visible = false;
					LblRecurso.Visible = false;
					TBRecurso.Visible = false;
					RequiredFieldValidator3.Enabled = false;
				}
		}
		catch (Exception ex)
		{
			logDebugView.Error("(DestinosTelefonia-LBDestinos_IndexChanged):", ex);
		}
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void LBDestinos_IndexChanged(object sender, EventArgs e)
    {
        MuestraElemento();
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void DDLGrupo_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (DDLGrupo.SelectedIndex != 0)
            TBGrupo.Text = DDLGrupo.SelectedValue;
        else
            TBGrupo.Text = "";
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BModificar_Click(object sender, EventArgs e)
    {
        IndexListBox1 = LBDestinos.SelectedIndex;

        Modificando = true;
        NuevoDestino(true);
        MostrarValidacion();
	}

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void DDLPrefijo_SelectedIndexChanged(object sender, EventArgs e)
    {
        string strPrefijo=((DropDownList)sender).SelectedValue;

        switch(strPrefijo)
        {
            case "32":   //PP
                DDLRecursos.Visible = true;
                LblRecurso.Visible = true;
                LblRecursosLibres.Visible = true;
                TBRecurso.Visible = true;
                RequiredFieldValidator3.Enabled = true;
                TBRecurso.Text = "";
                TBAbonado.Visible = false;
                Label5.Visible = false;

                CompletaDDLRecursos(32);
                break;

            case "1": // LCEN
                DDLRecursos.Visible = true;
                LblRecurso.Visible = true;
                LblRecursosLibres.Visible = true;
                TBRecurso.Visible = true;
                RequiredFieldValidator3.Enabled = true;
                TBRecurso.Text = "";
                TBAbonado.Visible = false;
                Label5.Visible = false;

                CompletaDDLRecursos(1);
                break;

            default:
                TBAbonado.Enabled = true;
                DDLRecursos.Visible = false;
                LblRecurso.Visible = false;
                LblRecursosLibres.Visible = false;
                TBRecurso.Visible = false;
                RequiredFieldValidator3.Enabled = false;
                TBAbonado.Visible = true;
                Label5.Visible = true;
                break;
        }

        if (strPrefijo == "3" || strPrefijo == "1" || strPrefijo == "-1")
        {
            LbGrupo.Visible = false;
            TBGrupo.Visible = false;
            DDLGrupo.Visible = false;
            if (TBGrupo.Text.Length > 0)
            {
                TBGrupo.Text = String.Empty;
                DDLGrupo.SelectedIndex = -1;
            }
        }
        else
        {
            LbGrupo.Visible = true;
            TBGrupo.Visible = true;
            DDLGrupo.Visible = true;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="prefijo"></param>
    private void CompletaDDLRecursos(int prefijo)
    {
        try
        {
            ServiciosCD40.DestinosTelefonia dTelefonia = new ServiciosCD40.DestinosTelefonia();

            while (DDLRecursos.Items.Count != 1)
            {
                DDLRecursos.Items.RemoveAt(1);
            }
            //DDLRecursos.Items.Add(new ListItem((string)GetLocalResourceObject("ListItemResource5"),"-1"));
			DDLRecursos.DataSource = ServiceServiciosCD40.RecursosSinAsignarAEnlaces((string)Session["idsistema"], (prefijo == 32) ? 1 : 2,false); // TF o LCEN // 20240110 SIRTAP
            DDLRecursos.DataTextField = "IDRecurso";
            DDLRecursos.DataBind();
        }
        catch (Exception e)
        {
            logDebugView.Error("(DestinosTelefonia-CompletaDDLRecursos):", e);
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void DDLRecursos_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (DDLRecursos.SelectedIndex != 0)
            TBRecurso.Text = DDLRecursos.SelectedValue;
        else
            TBRecurso.Text = "";
    }

    /// <summary>
    /// 
    /// </summary>
    private void MostrarMenu()
    {

		DDLGrupo.Visible = true;

    }

    /// <summary>
    /// 
    /// </summary>
    private void EsconderMenu()
    {	
		DDLGrupo.Visible = false;
    }

    /// <summary>
    /// 
    /// </summary>
    private void MostrarValidacion()
    {
        ValidationSummary1.Visible = true;
        RequiredFieldValidator1.Visible = true;
        RequiredFieldValidator2.Visible = true;
        RequiredFieldValidator3.Visible = true;
    }
    /// <summary>
    /// 
    /// </summary>
    private void OcultarValidacion()
    {
        ValidationSummary1.Visible = false;
        RequiredFieldValidator1.Visible = false;
        RequiredFieldValidator2.Visible = false;
        RequiredFieldValidator3.Visible = false;    
    }

    private void EliminaDestinoDeTFT()
    {
        Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];

        ServiciosCD40.DestinosExternosSector drs = new ServiciosCD40.DestinosExternosSector();
        drs.IdSistema = s.Value;
        drs.IdDestino = TBDestino.Text;

        // Eliminar de todos los TFT el destino
        ServiceServiciosCD40.DeleteSQL(drs);
    }


    private bool DestinoAsignadoATft(string strSistema,string destino)
    {
        ServiciosCD40.DestinosExternosSector drs = new ServiciosCD40.DestinosExternosSector();
        drs.IdSistema = strSistema;
        drs.IdDestino = destino;

        ServiciosCD40.Tablas[] lista = ServiceServiciosCD40.ListSelectSQL(drs);

        if (lista != null && lista.Length > 0)
            return true;

        return false;
    }

    private bool DestinoLCENAsignadoPanelLC(string strSistema,string strDestino,uint idPrefijo, ref System.Text.StringBuilder strMsg)
    {
        //Devuelve true si el destino LCEN está asignado a algún destino en el panel de LC
        //En principio, sólo puede estar asignado a destinos ATS
        bool bExiste = false;
        try
        {
            ServiciosCD40.DestinosExternosSector dExtSec = new ServiciosCD40.DestinosExternosSector();
            dExtSec.IdSistema = strSistema;
            dExtSec.IdDestinoLCEN = strDestino;
            dExtSec.IdPrefijoDestinoLCEN = idPrefijo;
            dExtSec.TipoAcceso = "IA";       //Panel de LC

            ServiciosCD40.Tablas[] lista = ServiceServiciosCD40.ListSelectSQL(dExtSec);

            if (lista != null && lista.Length > 0)
            {

                bExiste = true;
                string strDestinoATS= ((ServiciosCD40.DestinosExternosSector)lista[0]).IdDestino;
                string strSector= ((ServiciosCD40.DestinosExternosSector)lista[0]).IdSector;

                //Componemos el mensaje  a mostrar en pantalla
                System.Text.StringBuilder strMsgFormato= new System.Text.StringBuilder();

                if (GetGlobalResourceObject("Espaniol", "DestinoLCENAsignadoEnPanelLC")!=null)
                    strMsgFormato.Append((string)GetGlobalResourceObject("Espaniol", "DestinoLCENAsignadoEnPanelLC"));
                else
                    strMsgFormato.Append("El destino de línea caliente externa {0} se encuentra asignado al destino ATS {1} en el panel de línea caliente del sector {2}. Por favor, libere el destino {0} antes de eliminarlo");
                
                if (strMsg.Length>0)
                    strMsg.Clear();

                strMsg.AppendFormat(strMsgFormato.ToString(),strDestino,strDestinoATS,strSector);
                strMsgFormato.Clear();
            }

        }
        catch (Exception)
        {
        }

        return bExiste;
    }


	// Respuesta a la regeneración de las sectorizaciones
    /// <summary>
    /// 
    /// </summary>
    /// <param name="result"></param>
	private void OnCallBackCompleted(IAsyncResult result)
	{
		try
		{
			int retorno = ServiceServiciosCD40.EndRegeneraSectorizaciones(result);
			Session.Add("Sectorizando", false);
		}
		catch (System.Web.Services.Protocols.SoapException soapException)
		{
			logDebugView.Error("(DestinosTelefonia-OnCallBackCompleted): ", soapException);
		}
	}
    //Devuelve true si ya existe algun destino en el sistema con el mismo identificador. En caso contrario, false.
    private bool bIdentificadorAsignado(string strIdSistema, string strIdentificador)
    {
        bool bExiste = false;
        int iExiste = -1;

        // Se comprueba que no existe otro destino radio o de telefonía con el mismo identificador 
        iExiste = ServiceServiciosCD40.CheckIdentificadorAsignado("D", strIdSistema, strIdentificador);

        if (iExiste > 0)
            bExiste = true;
        else if (iExiste < 0)
        {
            System.Text.StringBuilder strMsgError = new System.Text.StringBuilder();
            strMsgError.AppendFormat("(DestinosTelefonia-bIdentificadorAsignado): el servicio servicioCD40.CheckIdentificadorAsignado('D', '{0}', '{1}') ha devuelto el codigo {2}", strIdSistema, strIdentificador, iExiste);
            logDebugView.Error(strMsgError.ToString());
            strMsgError.Clear();
        }

        return bExiste;
    }

    // 2021111 #3945
    private bool bDestinoLCENASector(string id_DestinoLCEN)
    {
        bool basociado = false;
       /* try
        {

            string id_Sistema = string.Empty;
            id_Sistema = (string)Session["idsistema"];

            DataSet drs = ServiceServiciosCD40.GetDestinoLCENASector(id_Sistema, id_DestinoLCEN);

            if (null != drs && drs.Tables.Count > 0 && drs.Tables[0].Rows.Count < 2)
            {
                basociado = true;
                TBSector.Text = (string)drs.Tables[0].Rows[0]["IdSector"];
                TBATS.Text = (string)drs.Tables[0].Rows[0]["IdDestino"];
                TBNumero.Text = (string)drs.Tables[0].Rows[0]["IdAbonado "];
            }
            else
            {
                TBSector.Text = " ";
                TBATS.Text = " ";
                TBNumero.Text = " ";
            }
        }
        catch (System.Web.Services.Protocols.SoapException error)
        {
            // Populate the table with the exception details.
            string s1 = error.Code.Namespace;
            string s2 = error.Code.Name;
           string s3 = error.Actor;
           string s4 = error.Message;
        }*/
        return basociado;
    }

    protected void CBSeguro_OnCheckedChanged(object sender, EventArgs e)
    {

    }

}
