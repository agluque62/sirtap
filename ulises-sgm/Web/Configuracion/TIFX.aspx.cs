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

/// <summary>
/// 
/// </summary>
public partial class TIFX_Cfg : PageBaseCD40.PageCD40	// System.Web.UI.Page
{

    const string MODO_SINCRO_NTP = "2";

    private Mensajes.msgBox cMsg; 
    /// <summary>
    /// 
    /// </summary>
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
    private static ServiciosCD40.Tablas[] datos;
    /// <summary>
    /// 
    /// </summary>
	static bool PermisoSegunPerfil;
    /// <summary>
    /// 
    /// </summary>
	private static ServiciosCD40.ServiciosCD40 ServicioCD40 = new ServiciosCD40.ServiciosCD40();
    /// <summary>
    /// 
    /// </summary>
	private static int NumPaginaActiva = 0;
    /// <summary>
    /// 
    /// </summary>
	private static bool Modificando = false;
    /// <summary>
    /// 
    /// </summary>
	private static bool Editando = false;

    /// <summary>
    /// Puerto Rstp por defecto
    /// </summary>
    private const int RTSP_PORT = 554;
    private const int RTSP_PORT_MIN = 1;
    private const int RTSP_PORT_MAX = 65535;

    static int old_index_rec1 = 0;
    static string old_rstp_rec1 = "554";
    static int old_index_rec2 = 0;
    static string old_rstp_rec2 = "554";

    /// <summary>
    /// 
    /// </summary>
    private Ulises5000Configuration.ToolsUlises5000Section UlisesToolsVersion;

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
				Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO");
				return;
			}

            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            Ulises5000Configuration.ToolsUlises5000Section UlisesTools = Ulises5000Configuration.ToolsUlises5000Section.Instance;

            UlisesToolsVersion = UlisesTools;

            PermisoSegunPerfil = (perfil == "3");
		}

		if (!IsPostBack)
		{
            IndexListBox1 = -1;
            NumPaginaActiva = 0;
			

            IBPropiedadesGenerales.CssClass = "buttonImageSelected";
            IBProtocoloSIP.CssClass = "buttonImage";
            IBProtocoloSNMP.CssClass = "buttonImage";
            IBRecursos.CssClass = "buttonImage";
            if (UlisesToolsVersion.Tools["GrabacionRecursoRadio"] == null)
            {
                TblRecorders.Visible = false;
                TblRecorders.Disabled = true;
            }
            CargaDDLGrabadores();
            

            BtAceptar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "AceptarCambios");
            BtCancelar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "CancelarCambios");
            MuestraDatos(DameDatos());
        }
        else
        {
            //Si se ha recargado la p�gina, las variables datos y la variable de session tienen valor nulo es porque 
            // si ha cambiado la sesi�n del servidor, bien por conmutaci�n o reinicio
            //por lo que se va a la p�gina de login
            if (datos == null || Session["idsistema"] == null)
            {
                ScriptManager.RegisterStartupScript(this, typeof(Page), "redirect", "<Script language = 'Javascript'> window.parent.location='../Login.aspx' ; </Script>", false);
            }

            LError.Visible = false;
            MultiView1.ActiveViewIndex = NumPaginaActiva;
            LblIp1Existente.Visible = LblIp2Existente.Visible = LblIpVirtualExistente.Visible = false;

            LblIp1Existente.Visible = LblIp2Existente.Visible = LblIpVirtualExistente.Visible = false;

        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="edicion"></param>
	private void ModoEdicion(bool edicion)
	{
		Editando = edicion;

		ListBox1.Enabled = !edicion;
		TxtIdTIFX.ReadOnly = !edicion;
		TxtIdTIFX.Enabled = edicion;
		TxtIP1.ReadOnly = !edicion;
		TxtIP2.ReadOnly = !edicion;
        TxtIpVirtual.ReadOnly = !edicion;
		TxtTimeSupervision.ReadOnly = !edicion;
		TxtSNMPTraps.ReadOnly = !edicion;
		TxtSNMPPortRemoto.ReadOnly = !edicion;
		TxtSNMPPortLocal.ReadOnly = !edicion;

		TxtMaster.ReadOnly = !edicion;
		DListArranque.Enabled = edicion;
        //MVO. 20170707: El modo de sincronizaci�n debe aparecer deshabilitado y con el valor NTP seleccionado. El puerto SIP local tomar� siempre el valor 5060 y no se puede modificar 
		//DListModoSincro.Enabled = edicion;
        //TxtPortSIPLocal.ReadOnly = !edicion;
		BtAceptar.Visible = BtCancelar.Visible = edicion;
        // 20220111 #3272 RFQ
        CBGrabacionED137.Enabled = edicion;
        CBGrabacionED137.Enabled = edicion;
        DDLRecorder1.Enabled = edicion;
        DDLRecorder2.Enabled = edicion;
        TBRtspPort.Enabled = edicion;
        TBRtspPort1.Enabled = edicion;

        if (CBGrabacionED137.Checked)
        {
            PresentaGrabacionED137(true);
        }
        else
        {
            PresentaGrabacionED137(false);
        }

        BtNuevo.Visible = !edicion && PermisoSegunPerfil;
		BtModificar.Visible = BtEliminar.Visible = ListBox1.Items.Count > 0 && !edicion && PermisoSegunPerfil;

        //VMG 18/02/2019
        // Enable/disable GTW elements
        DDLSupervisionLanGW.Enabled = edicion;
        TxtTmMaxSupervLanGW.ReadOnly = !edicion;
        if (DDLSupervisionLanGW.SelectedIndex == 0)
            LBTmMaxSupervLanGW.Visible = TxtTmMaxSupervLanGW.Visible = true;
        else
            LBTmMaxSupervLanGW.Visible = TxtTmMaxSupervLanGW.Visible = false;
	}
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtNuevo_Click(object sender, EventArgs e)
    {
		LimpiarMenu();
        IndexListBox1 = ListBox1.SelectedIndex;
        
        CBGrabacionED137.Checked = false;
        TBRtspPort.Text = RTSP_PORT.ToString();
        TBRtspPort1.Text = RTSP_PORT.ToString();

        MostrarValidacion();

		ModoEdicion(true);
        //20210803  #4765
        // Si se est� en la vista de asignaci�n de recursos a GW,
        // se pasa a la vista de par�metros generales
//        if (MultiView1.ActiveViewIndex == 2)
            OnButton_Click(IBPropiedadesGenerales, null);
	}
    /// <summary>
    /// 
    /// </summary>
    private void MostrarElemento()
    {
        BtModificar.Visible = PermisoSegunPerfil;
        /**
         * AGL 2010.06.19 ID.115
         * 
         * */
        BtNuevo.Visible = PermisoSegunPerfil;
        /**/

        TxtIdTIFX.ReadOnly = true;
        TxtIP1.ReadOnly = true;
        TxtIP2.ReadOnly = true;
        TxtIdTIFX.Enabled = true;
        TxtSNMPPortLocal.ReadOnly = true;
        TxtSNMPPortRemoto.ReadOnly = true;
        TxtSNMPTraps.ReadOnly = true;
        TxtTimeSupervision.ReadOnly = true;
        TxtPortSIPLocal.ReadOnly = true;
        TxtIpVirtual.ReadOnly = true;

        BtAceptar.Visible = false;
        BtCancelar.Visible = false;

        CBGrabacionED137.Enabled = false;
        DDLRecorder1.Enabled = false;
        DDLRecorder2.Enabled = false;
        TBRtspPort.Enabled = false;
        TBRtspPort1.Enabled = false;


        if (null != datos)
        {
            for (int i = 0; i < datos.Length; i++)
            {
                if (String.Compare((((ServiciosCD40.TifX)datos[i]).IdTifx), ListBox1.SelectedValue) == 0)
                {
                    BtEliminar_ConfirmButtonExtender.ConfirmText = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarTIFX"), ListBox1.SelectedValue);

                    TxtIdTIFX.Text = ((ServiciosCD40.TifX)datos[i]).IdTifx;
                    TxtIP1.Text = ((ServiciosCD40.TifX)datos[i]).IpRed1;
                    TxtIP2.Text = ((ServiciosCD40.TifX)datos[i]).IpRed2;
                    TxtSNMPPortLocal.Text = (((ServiciosCD40.TifX)datos[i]).SNMPPortLocal).ToString();
                    TxtSNMPPortRemoto.Text = (((ServiciosCD40.TifX)datos[i]).SNMPPortRemoto).ToString();
                    TxtSNMPTraps.Text = (((ServiciosCD40.TifX)datos[i]).SNMPTraps).ToString();
                    TxtTimeSupervision.Text = (((ServiciosCD40.TifX)datos[i]).TimeSupervision).ToString();
                    TxtPortSIPLocal.Text = (((ServiciosCD40.TifX)datos[i]).SIPPortLocal).ToString();

                    //VMG 18/02/2019
                    DDLSupervisionLanGW.SelectedValue = (((ServiciosCD40.TifX)datos[i]).iSupervLanGW).ToString();
                    TxtTmMaxSupervLanGW.Text = (((ServiciosCD40.TifX)datos[i]).itmmaxSupervLanGW).ToString();
                    if (DDLSupervisionLanGW.SelectedValue == "1")
                        LBTmMaxSupervLanGW.Visible = TxtTmMaxSupervLanGW.Visible = TxtTmMaxSupervLanGW.ReadOnly = true;
                    else
                        LBTmMaxSupervLanGW.Visible = TxtTmMaxSupervLanGW.Visible = false;

                    if (((ServiciosCD40.TifX)datos[i]).Grabador1 == null || ((ServiciosCD40.TifX)datos[i]).Grabador1 == string.Empty)
                    {
                        DDLRecorder1.SelectedIndex = 0;
                        TBRtspPort.Text = RTSP_PORT.ToString();
                    }
                    else
                    {
                        DDLRecorder1.SelectedValue = ((ServiciosCD40.TifX)datos[i]).Grabador1;
                        TBRtspPort.Text = ((ServiciosCD40.TifX)datos[i]).RtspGrabador1.ToString();
                    }

                    if (((ServiciosCD40.TifX)datos[i]).Grabador2 == null || ((ServiciosCD40.TifX)datos[i]).Grabador2 == string.Empty)
                    {
                        DDLRecorder2.SelectedIndex = 0;
                        TBRtspPort1.Text = RTSP_PORT.ToString();
                    }
                    else
                    {
                        DDLRecorder2.SelectedValue = ((ServiciosCD40.TifX)datos[i]).Grabador2;
                        TBRtspPort1.Text = ((ServiciosCD40.TifX)datos[i]).RtspGrabador2.ToString();
                    }

                    if (((ServiciosCD40.TifX)datos[i]).GrabacionED137 == null)
                        CBGrabacionED137.Checked = false;
                    else
                        CBGrabacionED137.Checked = (((ServiciosCD40.TifX)datos[i]).GrabacionED137 == 1);
                    // Salva los valores presentados...
                    old_index_rec1 = DDLRecorder1.SelectedIndex;
                    old_rstp_rec1 = TBRtspPort.Text;
                    old_index_rec2 = DDLRecorder2.SelectedIndex;
                    old_rstp_rec2 = TBRtspPort1.Text;

                    // Habilitar grabadores y presentarlos
                    PresentaGrabacionED137(CBGrabacionED137.Checked); 
                    CargarIpVirtual(TxtIdTIFX.Text);

                    DListArranque.Text = ((ServiciosCD40.TifX)datos[i]).ModoArranque;
                    DListModoSincro.Text = ((ServiciosCD40.TifX)datos[i]).ModoSincronizacion.ToString();
                    if (DListModoSincro.SelectedValue == "0")
                    {
                        Label15.Visible = false;
                        TxtMaster.Visible = false;
                        TxtMaster.ReadOnly = false;
                        TxtMaster.Text = "";
                    }
                    else
                    {
                        Label15.Visible = true;
                        TxtMaster.Visible = true;
                        TxtMaster.ReadOnly = true;
                        TxtMaster.Text = ((ServiciosCD40.TifX)datos[i]).Master;
                    }
                }
            }
        }
    }
    /// <summary>
    /// 
    /// </summary>
    private void LimpiarMenu()
    {
        TxtIdTIFX.Text = "";
        TxtIP1.Text = "";
        TxtIP2.Text = "";
        TxtIpVirtual.Text = "";
        TxtSNMPPortLocal.Text = "161";
        TxtSNMPPortRemoto.Text = "161";
        TxtSNMPTraps.Text = "162";
        TxtTimeSupervision.Text = "90";
        TxtPortSIPLocal.Text = "5060";
        TxtMaster.Text = "";

        DDLRecorder1.SelectedIndex = DDLRecorder2.SelectedIndex = 0;

        DDLSupervisionLanGW.SelectedIndex = 1;
        TxtTmMaxSupervLanGW.Text = "1";
        //MVO. 20170707: El modo de sincronizaci�n debe aparecer deshabilitado y con el valor NTP seleccionado 
        DListModoSincro.SelectedValue = MODO_SINCRO_NTP;

		LimpiaTablaTifx();
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
            BtEliminar_ConfirmButtonExtender.ConfirmText = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarTIFX"), ListBox1.SelectedValue);
            //BtEliminar.Enabled = PermisoSegunPerfil;
			CargaRecursosDeLaTifx(ListBox1.SelectedItem.Text);
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
            ServiciosCD40.TifX t = new ServiciosCD40.TifX();
            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            t.IdSistema = s.Value;
            Session["idsistema"] = s.Value;

			ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(t);
            datos = d;
            return d;
        }
        catch (Exception ex)
        {
            logDebugView.Error("(TIFX-DameDatos): ", ex);
        }
        return null;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="nu"></param>
    private void MuestraDatos(ServiciosCD40.Tablas[] nu)
    {
        ListBox1.Items.Clear();
        for (int i = 0; i < nu.Length; i++)
            ListBox1.Items.Add(((ServiciosCD40.TifX)nu[i]).IdTifx);

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

            CargaRecursosDeLaTifx(ListBox1.SelectedItem.Text);
			MostrarElemento();
		}
		else
		{
			LimpiarMenu();
		}
	
		ModoEdicion(false);
	}

    private void CargarIpVirtual(string idVirtual)
    {
        ServiciosCD40.GwActivas gw = new ServiciosCD40.GwActivas();
        gw.IdSistema = (string)Session["idsistema"];
        gw.IdTifx = idVirtual;

        TxtIpVirtual.Text = string.Empty;

        ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(gw);
        if (d.Length > 0)
            TxtIpVirtual.Text = ((ServiciosCD40.GwActivas)d[0]).IpRed;
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
		ModoEdicion(false);
		Modificando = false;
        if (ListBox1.Items.Count > 0)
            ListBox1.SelectedIndex = IndexListBox1 != -1 ? IndexListBox1 : 0;

        EsconderValidacion();

		MuestraDatos(DameDatos());
    }
    /// <summary>
    /// 
    /// </summary>
    private void GuardarCambios(string strIdSistema)
    {
        try
        {
            ServiciosCD40.TifX n = new ServiciosCD40.TifX();
            ServiciosCD40.GwActivas gw = new ServiciosCD40.GwActivas();

            gw.IdSistema = n.IdSistema = strIdSistema;
            if (!Modificando) //TIFX nueva, se guarda en may�sculas
                gw.IdTifx = n.IdTifx = TxtIdTIFX.Text.ToUpper();
            else
            {
                gw.IdTifx = n.IdTifx = ListBox1.SelectedValue;
                IndexListBox1 = ListBox1.SelectedIndex;
            }

            NewItem = gw.IdTifx;

            n.ModoSincronizacion = UInt16.Parse(DListModoSincro.SelectedValue);
            n.SNMPPortLocal = UInt16.Parse(TxtSNMPPortLocal.Text);
            n.SNMPPortRemoto = UInt16.Parse(TxtSNMPPortRemoto.Text);
            n.SNMPTraps = UInt16.Parse(TxtSNMPTraps.Text);
            n.TimeSupervision = UInt16.Parse(TxtTimeSupervision.Text);
            n.iSupervLanGW = Byte.Parse(DDLSupervisionLanGW.SelectedValue);
            n.itmmaxSupervLanGW = Byte.Parse(TxtTmMaxSupervLanGW.Text);

            //VMG 18/02/2019 Error de rango Modify
            if (n.itmmaxSupervLanGW <= 0 || n.itmmaxSupervLanGW > 5)
            {
                String strMsgAux = String.Empty;
                strMsgAux = string.Format((string)GetGlobalResourceObject("Espaniol", "TxtTmMaxSupervLanGWResource"));
                cMsg.confirm(strMsgAux, "aceptparam");
                return;
            }

            if (!EsPuertoValido(TBRtspPort) || !EsPuertoValido(TBRtspPort1))
            {
                String strMsgAux = String.Empty;
                strMsgAux = string.Format((string)GetGlobalResourceObject("Espaniol", "ErrorRangoPuertoRtsp"));
                cMsg.confirm(strMsgAux, "aceptparam");
                return;
            }

            if (n.ModoSincronizacion != 0)
            {
                if (!string.IsNullOrWhiteSpace(TxtMaster.Text))
                    n.Master = TxtMaster.Text;
                else
                    n.Master = null;

            }
            n.SIPPortLocal = UInt16.Parse(TxtPortSIPLocal.Text);

            n.GrabacionED137 = (CBGrabacionED137.Checked) ? 1 : 0;
            
            if (DDLRecorder1.SelectedIndex == 0 && DDLRecorder2.SelectedIndex == 0)
            {
                n.GrabacionED137 = 0;
                CBGrabacionED137.Checked = false;
            }

            n.Grabador1 = DDLRecorder1.SelectedIndex != 0 ? DDLRecorder1.SelectedValue : null;
            n.Grabador2 = DDLRecorder2.SelectedIndex != 0 ? DDLRecorder2.SelectedValue : null;
            n.RtspGrabador1 = Convert.ToInt32(TBRtspPort.Text);
            n.RtspGrabador2 = Convert.ToInt32(TBRtspPort1.Text);

            n.IpRed1 = TxtIP1.Text;
            n.IpRed2 = TxtIP2.Text;
            n.ModoArranque = DListArranque.SelectedValue;

            gw.IpRed = TxtIpVirtual.Text;

			// Comprobar que ninguna de las IPs existen ya dadas de alta en el sistema.            
			if (ServicioCD40.ExisteIP(n.IdSistema, n.IpRed1, n.IdTifx))
			{
				LblIp1Existente.Visible = true;
				return;
			}
            else if (!string.IsNullOrEmpty(n.IpRed2) && ServicioCD40.ExisteIP(n.IdSistema, n.IpRed2, n.IdTifx))
            {
                LblIp2Existente.Visible = true;
                return;
            }
            else if (string.Compare(n.IpRed1,TxtIpVirtual.Text)!=0 && (ServicioCD40.ExisteIP(n.IdSistema, TxtIpVirtual.Text, n.IdTifx)))
            {
                LblIpVirtualExistente.Visible = true;
                return;
            }

            if (!Modificando) //TIFX nueva
            {
                if (ServicioCD40.InsertSQL(n) < 0)
                    logDebugView.Warn("(TIFX-GuardarCambios): No se ha podido guardar la TIFX");
                else if (ServicioCD40.InsertSQL(gw) < 0)
                    logDebugView.Warn("(TIFX-GuardarCambios): No se ha podido guardar GWActiva");

                ActualizaWebPadre(true);
            }
            else
            {
                if (ServicioCD40.UpdateSQL(n) < 0)
                    logDebugView.Warn("(TIFX-GuardarCambios): No se ha podido actualizar la TIFX");
                else
                {
                    ServicioCD40.DeleteSQL(gw);
                    ServicioCD40.InsertSQL(gw);
                }
            }
		
			Modificando = false;
		}
        catch (Exception ex)
        {
            logDebugView.Error("(TIFX-GuardarCambios): ", ex);
        }

        ListBox1.Enabled = true;
        BtNuevo.Enabled = PermisoSegunPerfil;
        //BtEliminar.Enabled = false;
		IBRecursos.Visible = true; ;
		BtAceptar.Visible = false;
		BtCancelar.Visible = false;
		BtModificar.Visible = PermisoSegunPerfil;

		ModoEdicion(false);
        EsconderValidacion();
        MuestraDatos(DameDatos());
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="valor"></param>

    private bool EsPuertoValido(TextBox valor)
    {
        string sCampo = valor.Text.Trim();
        double dCampo;
        if (!double.TryParse(sCampo, out dCampo) || (dCampo < RTSP_PORT_MIN || dCampo > RTSP_PORT_MAX))
            return false;
        return true;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtCancelar_Click(object sender, EventArgs e)
    {
        CancelarCambios();
        //cMsg.confirm((string)GetGlobalResourceObject("Espaniol", "CancelarCambios"), "cancelparam");
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtAceptar_Click(object sender, EventArgs e)
    {
        //MVO 20170714: Si la Si la configuraci�n de la CPU es:
        //                  * Simple: se deben configurar los campos IP CPU-0, IP de la CPU-1 e IP de comunicaciones con la misma direcci�n (TxtIP1==TxtIP2 && TxtIP1==TxtIpVirtual) 
        //                  * Dual:  se deben configurar los campos IP CPU-0, IP CPU-1 e IP de Comunicaciones con direcciones distintas (TxtIP1<>TxtIP2 y TxtIP2<>TxtIpVirtual y TxtIP1<>TxtIpVirtual).
        // Los controles TxtIP1, TxtIP2 y TxtIpVirtual ya son requeridos. 

        string StrSistema = (string)Session["idsistema"];

        if ((TxtIP1.Text.Equals(TxtIP2.Text) && TxtIP1.Text.Equals(TxtIpVirtual.Text)) ||
            (!string.IsNullOrEmpty(TxtIP2.Text) && string.Compare(TxtIP1.Text, TxtIP2.Text) != 0 && 
              string.Compare(TxtIP1.Text, TxtIpVirtual.Text) != 0 && string.Compare(TxtIP2.Text, TxtIpVirtual.Text) != 0))
        {
            //Configuraci�n CPU dual o Simple correcta 
            // El identificador de la pasarela se visualiza en may�sculas, pero internamente se almacena como lo haya insertado
            // Por lo que hay que convertirlo a may�sculas para almacenarlo y buscarlo (aunque la Bd no distingue m�y�sculas y min�sculas en las b�squedas)
            if (!Modificando && TxtIdTIFX.Enabled && bIdentificadorAsignado(StrSistema, TxtIdTIFX.Text.ToUpper()))
            {
                //Si se est� insertando una pasarela nueva y existe otra con el mismo identificador, se informa al usuario para que indique otro identificador
                //que se �nico para los elementos f�sicos
                cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "ErrorIdentificadorEltoFisicoExiste"), TxtIdTIFX.Text.ToUpper()));

            }
            else if ((0 != DDLRecorder2.SelectedIndex) && (0 == DDLRecorder1.SelectedIndex))
            {
                //No se permite configurar el grabador 2, si previamente no se ha configurado el grabador1
                cMsg.confirm((string)GetGlobalResourceObject("Espaniol", "AvisoConfGrabadorEnTop"), "aceptparam");
            }
            else if ((0 != DDLRecorder1.SelectedIndex) && (0 != DDLRecorder2.SelectedIndex) && (DDLRecorder1.SelectedValue == DDLRecorder2.SelectedValue))
            {
                //Si se configuran los dos grabadores tienen que ser distintos
                cMsg.confirm((string)GetGlobalResourceObject("Espaniol", "AvisoConfGrabadorDistintos"), "aceptparam");
            }
            else
                GuardarCambios(StrSistema);
        }
        else
        {
            //Configuraci�n CPU Simple o dual err�nea
            cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "ErrorConfiguracionCPUPasarela"), " "));
        }
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtModificar_Click(object sender, EventArgs e)
    {
		ModoEdicion(true);
		TxtIdTIFX.Enabled = false;
		Modificando = true;
        IndexListBox1 = ListBox1.SelectedIndex;

        CBGrabacionED137.Enabled = true;
        DDLRecorder1.Enabled = true;
        DDLRecorder2.Enabled = true;
        TBRtspPort.Enabled = true;
        TBRtspPort1.Enabled = true;
        PresentaGrabacionED137(CBGrabacionED137.Checked);

		// Si se est� en la vista de asignaci�n de recursos a GW, como no se puede modificar esta asignaci�n
		// se pasa a la vista de par�metros generales
        if (MultiView1.ActiveViewIndex == 3) // #4579 ANTES 3
			OnButton_Click(IBPropiedadesGenerales, null);

        MostrarValidacion();
	}
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void BtEliminar_Click(object sender, EventArgs e)
    {
        if (ListBox1.SelectedValue != "")
        {
			// Para que se pueda dar de baja una GW, esta ha de estar sin recursos asignados.
			ServiciosCD40.Recursos recurso = new ServiciosCD40.Recursos();
			recurso.IdSistema = (string)Session["idsistema"];
			recurso.IdTifX = ListBox1.SelectedValue;

			ServiciosCD40.Tablas[] listaRecursosEnTifX = ServicioCD40.ListSelectSQL(recurso);
			if (listaRecursosEnTifX.Length > 0)
			{
				//PasarelaNoVacia
				cMsg.alert((string)GetGlobalResourceObject("Espaniol", "PasarelaNoVacia"));
			}
			else
			{
				//string texto = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarTIFX"), ListBox1.SelectedValue);
                IndexListBox1 = ListBox1.SelectedIndex;
				Session["elemento"] = ListBox1.SelectedValue;
                EliminarElemento();
				//cMsg.confirm(texto, "eliminaelemento");
			}
        }
    }
    /// <summary>
    /// 
    /// </summary>
    private void EliminarElemento()
    {
        try
        {
			ServiciosCD40.TifX n = new ServiciosCD40.TifX();
            n.IdSistema = (string)Session["idsistema"];
            n.IdTifx = (string)Session["elemento"];
			if (ServicioCD40.DeleteSQL(n) < 0) logDebugView.Warn("(TIFX-EliminarElemento): No se ha podido eliminar la TIFX.");
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ElementoEliminado"));
        }
        catch (Exception ex)
        {
            logDebugView.Error("(TIFX-EliminarElemento): ", ex);
        }
        MuestraDatos(DameDatos());
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void DListModoSincro_TextChanged(object sender, EventArgs e)
    {
        if (DListModoSincro.Text != "0")
        {
            Label15.Visible = true;
            TxtMaster.Visible = true;
            TxtMaster.ReadOnly = false;
			//RequiredFieldMaster.Visible = true;
        }
        else
        {
            Label15.Visible = false;
            TxtMaster.Visible = false;
            TxtMaster.ReadOnly = true;
			//RequiredFieldMaster.Visible = false;
        }
    }
    /// <summary>
    /// 
    /// </summary>
    private void EsconderValidacion()
    {
        RequiredFieldIdTIFX.Visible = false;
//        RequiredFieldMaster.Visible = false;
        RequiredFieldPortLocalSNMP.Visible = false;
        //20201007 #4579        RequiredFieldSIP.Visible = false;
        RequiredFieldSNMPRemoto.Visible = false;
        RequiredFieldTimeSupervision.Visible = false;
        RequiredFieldTraps.Visible = false;
		//RequiredFieldIPRed1.Visible = false;
		//RequiredFieldIP2.Visible = false;
        RangeValidatorLocalSNMP.Visible = false;
        RangeValidatorSIPLocal.Visible = false;
        RangeValidatorSNMPRemoto.Visible = false;
        RangeValidatorTimeSupervision.Visible = false;
        RangeValidatorTraps.Visible = false;
        RegularExpressionValidator1.Visible = false;
        RegularExpressionValidator2.Visible = false;
        RegularExpressionValidator3.Visible = false;
        ValidationSummary1.Visible = false;
    }
    /// <summary>
    /// 
    /// </summary>
    private void MostrarValidacion()
    {
        RequiredFieldIdTIFX.Visible = true;
//        RequiredFieldMaster.Visible = true;
        RequiredFieldPortLocalSNMP.Visible = true;
        RequiredFieldSIP.Visible = true;
        RequiredFieldSNMPRemoto.Visible = true;
        RequiredFieldTimeSupervision.Visible = true;
        RequiredFieldTraps.Visible = true;
		//RequiredFieldIPRed1.Visible = true;
		//RequiredFieldIP2.Visible = true;
        RangeValidatorLocalSNMP.Visible = true;
        RangeValidatorSIPLocal.Visible = true;
        RangeValidatorSNMPRemoto.Visible = true;
        RangeValidatorTimeSupervision.Visible = true;
        RangeValidatorTraps.Visible = true;
        RegularExpressionValidator1.Visible = true;
        RegularExpressionValidator2.Visible = true;
        RegularExpressionValidator3.Visible = true;
        ValidationSummary1.Visible = true;
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
                IBPropiedadesGenerales.CssClass = "buttonImageSelected";
                IBProtocoloSIP.CssClass = "buttonImage";
                IBProtocoloSNMP.CssClass = "buttonImage";
                IBRecursos.CssClass = "buttonImage";
				MultiView1.ActiveViewIndex = NumPaginaActiva = 0;
				Panel2.Height = 390;
                Panel2.Width = 460;
				break;
               
			case "IBProtocoloSIP":
                IBPropiedadesGenerales.CssClass = "buttonImage";
                IBProtocoloSIP.CssClass = "buttonImageSelected";
                IBProtocoloSNMP.CssClass = "buttonImage";
                IBRecursos.CssClass = "buttonImage";
				MultiView1.ActiveViewIndex = NumPaginaActiva = 1;
				Panel2.Height = 260;
                Panel2.Width = 460;
				break; 
			case "IBProtocoloSNMP":
                IBPropiedadesGenerales.CssClass = "buttonImage";
                IBProtocoloSIP.CssClass = "buttonImage";
                IBProtocoloSNMP.CssClass = "buttonImageSelected";
                IBRecursos.CssClass = "buttonImage";
				MultiView1.ActiveViewIndex = NumPaginaActiva = 2;
				Panel2.Height = 180;
                Panel2.Width = 460;
				break;
			case "IBRecursos":
                IBPropiedadesGenerales.CssClass = "buttonImage";
                IBProtocoloSIP.CssClass = "buttonImage";
                IBProtocoloSNMP.CssClass = "buttonImage";
                IBRecursos.CssClass = "buttonImageSelected";
				MultiView1.ActiveViewIndex = NumPaginaActiva = 3;
				Panel2.Height = 185;
                Panel2.Width = 565;
				break;
		}
	}
    /// <summary>
    /// 
    /// </summary>
    /// <param name="idTifx"></param>
	private void CargaRecursosDeLaTifx(string idTifx)
	{
        int[] NumSlotsPorTipoInterface ={ 1, 1, 1, 1, 1, 1, 1, 4, 2, 16, 1, 1, 1, 1, 1 };
		try
		{
			System.Drawing.Color[] colores ={System.Drawing.Color.BlueViolet,
                                            System.Drawing.Color.Firebrick,
                                            System.Drawing.Color.ForestGreen,
                                            System.Drawing.Color.ForestGreen,
                                            System.Drawing.Color.ForestGreen,
                                            System.Drawing.Color.ForestGreen,
                                            System.Drawing.Color.ForestGreen,
                                            System.Drawing.Color.SteelBlue,
                                            System.Drawing.Color.Salmon,
                                            System.Drawing.Color.Chocolate,
                                            System.Drawing.Color.ForestGreen,
                                            System.Drawing.Color.ForestGreen,
                                            System.Drawing.Color.ForestGreen,
                                            System.Drawing.Color.GreenYellow,
                                            System.Drawing.Color.GreenYellow
                                            };

			// Limpiar contenido tabla tifx
			LimpiaTablaTifx();

			ServiciosCD40.Recursos recurso = new ServiciosCD40.Recursos();
			recurso.IdSistema = (string)Session["IdSistema"];
			recurso.IdTifX = idTifx;

			ServiciosCD40.Tablas[] listaRecursosEnTifX = ServicioCD40.ListSelectSQL(recurso);

			for (int i = 0; i < listaRecursosEnTifX.Length; i++)
			{
				int slot = ((ServiciosCD40.Recursos)listaRecursosEnTifX[i]).SlotPasarela;
				int dispositivo = ((ServiciosCD40.Recursos)listaRecursosEnTifX[i]).NumDispositivoSlot;
				TextBox tbSlot = (TextBox)(TTifx.Rows[dispositivo].Cells[slot].FindControl("TextBox" + (dispositivo + 1) + (slot + 1)));
				CheckBox cbSlot = (CheckBox)(TTifx.Rows[dispositivo].Cells[slot].FindControl("CheckBox" + (dispositivo + 1) + (slot + 1)));
				if (tbSlot != null && cbSlot != null)
				{
                    // Eliminar la clase 'textbox' para que muestre los colores seg�n el tipo de recurso
                    tbSlot.CssClass = tbSlot.CssClass.Replace("textbox", "");

					for (int j = 0; j < NumSlotsPorTipoInterface[(int)((ServiciosCD40.Recursos)listaRecursosEnTifX[i]).Interface]; j++)
					{
						int fila = dispositivo + (j % 4);
						int col = slot + (j / 4);
						tbSlot = (TextBox)(TTifx.FindControl("TextBox" + (fila + 1) + (col + 1)));
						cbSlot = (CheckBox)(TTifx.FindControl("CheckBox" + (fila + 1) + (col + 1)));
						if (tbSlot != null && cbSlot != null)
						{
							cbSlot.Visible = false;
							tbSlot.Visible = true;
							tbSlot.ForeColor = System.Drawing.Color.Black;
							tbSlot.Text = ((ServiciosCD40.Recursos)listaRecursosEnTifX[i]).IdRecurso;
							tbSlot.BackColor = colores[(int)((ServiciosCD40.Recursos)listaRecursosEnTifX[i]).Interface];
						}
					}
				}
			}
		}
		catch (Exception ex)
		{
            logDebugView.Error("(TIFX-CargaRecursosDeLaTifx): ", ex);
		}
	}
    /// <summary>
    /// 
    /// </summary>
	private void LimpiaTablaTifx()
	{
		for (int filas = 0; filas < 4; filas++)
		{
			for (int colum = 0; colum < 4; colum++)
			{
				TextBox tbSlot = (TextBox)(TTifx.Rows[filas].Cells[colum].FindControl("TextBox" + (filas + 1) + (colum + 1)));
				CheckBox cbSlot = (CheckBox)(TTifx.Rows[filas].Cells[colum].FindControl("CheckBox" + (filas + 1) + (colum + 1)));
				if (tbSlot != null && cbSlot != null)
				{
					cbSlot.Text = "";
					cbSlot.Visible = true;
					cbSlot.Checked = false;
					tbSlot.Visible = false;
					tbSlot.Text = "";
					tbSlot.BackColor = System.Drawing.Color.White;
				}
			}
		}
	}


    private void CargaDDLGrabadores()
    {
        Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
        KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
        Session["idsistema"] = s.Value;

        ServiciosCD40.EquiposEU tifx = new ServiciosCD40.EquiposEU();
        string strFirstItemRecorder = string.Empty;

        strFirstItemRecorder = GetLocalResourceObject("DDLRecorderItem1").ToString();

        tifx.IdSistema = s.Value;
        tifx.TipoEquipo = (uint)ServiciosCD40.Tipo_Elemento_HW.TEH_GRABADOR;

        DDLRecorder1.Items.Clear();
        //Se a�ade el item 0 que indica que no se selecciona ning�n grabador.
        //DDLRecorder1.Items.Add("< Ninguno >");
        DDLRecorder1.Items.Add(strFirstItemRecorder);
        DDLRecorder1.SelectedIndex = -1;
        DDLRecorder1.DataSource = ServicioCD40.DataSetSelectSQL(tifx);
        DDLRecorder1.DataTextField = "IdEquipos";
        DDLRecorder1.DataValueField = "IdEquipos";
        DDLRecorder1.DataBind();

        DDLRecorder2.Items.Clear();
        //Se a�ade en el item 0, DDLRecorder2.Items.Add("< Ninguno >");
        DDLRecorder2.Items.Add(strFirstItemRecorder);
        DDLRecorder2.SelectedIndex = -1;
        DDLRecorder2.DataSource = ServicioCD40.DataSetSelectSQL(tifx);
        DDLRecorder2.DataTextField = "IdEquipos";
        DDLRecorder2.DataValueField = "IdEquipos";
        DDLRecorder2.DataBind();
        
        DDLRecorder1.SelectedIndex = 0;
        DDLRecorder2.SelectedIndex = 0;
    }


    //Devuelve True si ya existe algun elmento f�sico en el sistema con el mismo identificador. En caso contrario, false.
    private bool bIdentificadorAsignado(string strIdSistema,string strIdentificador)
    {
        bool bExiste = false;
        int iExiste = -1;
      
        //Si se est� dando de alta una nueva pasarela hay que comprobar 
        // que no existe un elemento f�sico (Pasarela, top, equipo externo o encaminamiento) con el mismo identificador 
        iExiste=ServicioCD40.CheckIdentificadorAsignado("EF", strIdSistema, strIdentificador);

        if (iExiste > 0)
            bExiste = true;
        else if (iExiste < 0)
        {
            System.Text.StringBuilder strMsgError = new System.Text.StringBuilder();
            strMsgError.AppendFormat("(TIFX-bIdentificadorAsignado): el servicio servicioCD40.CheckIdentificadorAsignado('EF', '{0}', '{1}') ha devuelto el codigo {2}", strIdSistema, strIdentificador, iExiste);
            logDebugView.Error(strMsgError.ToString());
            strMsgError.Clear();
        }

        return bExiste;
    }

    // VMG 18/02/2019
    /// <summary>
    /// Habilita el campo num�rico o lo deshabilita en funci�n
    /// de si es SI o No el dropBox.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    protected void DDLSupervisionLanGW_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (DDLSupervisionLanGW.SelectedIndex == 0)//Viene de Si(Index=0) a No(Index=1) 
            LBTmMaxSupervLanGW.Visible = TxtTmMaxSupervLanGW.Visible = true;
        else
            LBTmMaxSupervLanGW.Visible = TxtTmMaxSupervLanGW.Visible = false;
    }


    protected void CBGrabacionED137_OnCheckedChanged(object sender, EventArgs e)
    {
        if (CBGrabacionED137.Checked)
        {
            // Habilitar grabadores y presentarlos
            PresentaGrabacionED137(CBGrabacionED137.Checked);
        }
        else
        {
            if (RecursoActivoGrabacionED137() == true)
            {
                // Presenta Recurso con grabaci�n ED-137 Activo
                // Habilitar grabadores y presentarlos
                CBGrabacionED137.Checked = true;
                cMsg.alert((string)GetGlobalResourceObject("Espaniol", "RecursoActivoGrabacionED137"));
            }
            else
            {
                // Deshabilitar grabadorer, inicializarlos y ocultarlos.
                PresentaGrabacionED137(CBGrabacionED137.Checked);
            }
        }
    }

    private bool RecursoActivoGrabacionED137()
    {
        // Para que se pueda dar de baja una GW, esta ha de estar sin recursos asignados.
        ServiciosCD40.Recursos recurso = new ServiciosCD40.Recursos();
        recurso.IdSistema = (string)Session["idsistema"];
        recurso.IdTifX = ListBox1.SelectedValue;
        ServiciosCD40.Tablas[] listaRecursosEnTifX = ServicioCD40.ListSelectSQL(recurso);

        if (listaRecursosEnTifX.Length > 0)
        {
            foreach (ServiciosCD40.Recursos recTifX in listaRecursosEnTifX)
            {
                if (recTifX.Diffserv == true)
                    return true;
            }
        }
        return false;
    }


    protected void PresentaGrabacionED137(bool Visible)
    {
        if (Visible)
        {
            // Habilita grabadores y presenta
            TblRecorders.Visible = true;
            // Recupera los valores guardados...
            DDLRecorder1.SelectedIndex = old_index_rec1;
            TBRtspPort.Text = old_rstp_rec1;
            DDLRecorder2.SelectedIndex = old_index_rec2;
            TBRtspPort1.Text = old_rstp_rec2;
        }
        else
        {
            // Salva los valores presentados...
            old_index_rec1 = DDLRecorder1.SelectedIndex;
            old_rstp_rec1 = TBRtspPort.Text;
            old_index_rec2 = DDLRecorder2.SelectedIndex;
            old_rstp_rec2 = TBRtspPort1.Text;

            // Deshabilita grabadores, inicializa y oculta.
            TblRecorders.Visible = false;
            DDLRecorder1.SelectedIndex = 0;
            DDLRecorder2.SelectedIndex = 0;
            TBRtspPort.Text = RTSP_PORT.ToString();
            TBRtspPort1.Text = RTSP_PORT.ToString();
        }
    }

}
