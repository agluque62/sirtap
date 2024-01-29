using System;
using System.Data;
using System.Text;
using System.Text.RegularExpressions;
using System.Configuration;
using System.Collections.Generic;
using System.Web.Configuration;
using System.Web.Security;
using System.Web.SessionState;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using log4net;
using log4net.Config;

public partial class Conferencias : PageBaseCD40.PageCD40
{
    const int PUERTO_SIP_MIN = 1;
    const string SPUERTO_SIP_DEFECTO = "5060";
    const int PUERTO_SIP_MAX= 65535;

    const int NUM_MAX_CONFERENCIAS = 15;
    const int NUM_MAX_PARTICIPANTES = 40;
    const int NUM_MIN_PARTICIPANTES = 2;
    const int NUM_MAX_POSICIONES = 15;
    const int PREFIJO_ATS = 3;
    const string FORMATO_LB_RECURSOS = "{0, -20} {1, -15}";

    private static bool Editando = false;
    private static bool NuevaConferencia = false;
	static bool PermisoSegunPerfil;
    private static bool EditandoExternos = false;
    private static List<string> ListOfAllParticipantes = new List<string>();

   // private static DataSet datos;
    private static ServiciosCD40.ServiciosCD40 serviceServiciosCD40;
    private static ServiciosCD40.Tablas[] datos;
    private static ServiciosCD40.ServiciosCD40 ServicioCD40 = new ServiciosCD40.ServiciosCD40();
    private enum Tipo_Identificador : int { IDCONFERENCIA, IDSALABKK, IDUSUARIO };

    public class Participante
    {
        private string _SipUri;
        public string SipUri
        {
            get { return _SipUri; }
            set { _SipUri = value; }
        }

        private string _Descripcion;
        public string Descripcion
        {
            get { return _Descripcion; }
            set { _Descripcion = value; }
        }
    }

    public class ParticipanteExterno
    {
        private string _Usuario;
        public string Usuario
        {
            get { return _Usuario; }
            set { _Usuario = value; }
        }

        private string _DirIP;
        public string DirIP
        {
            get { return _DirIP; }
            set { _DirIP = value; }
        }

        private string _Puerto;
        public string Puerto
        {
            get { return _Puerto; }
            set { _Puerto = value; }
        }

        private string _Descripcion;
        public string Descripcion
        {
            get { return _Descripcion; }
            set { _Descripcion = value; }
        }
    }
    

    private static List<Participante> lParticipantes;
    private static List<Participante> lInternos;
    private static ParticipanteExterno participanteBCK;
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

        if (serviceServiciosCD40 == null)
            serviceServiciosCD40 = new ServiciosCD40.ServiciosCD40();

        if (!IsPostBack)
        {
            BtAceptar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "AceptarCambios");
            BtCancelar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "CancelarCambios");
            BtEliminar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "TXTEliminarElemento");
            IndexListBox1 = -1;
            if (Session["NombreConferencia"] != null && !string.IsNullOrEmpty((string)Session["NombreConferencia"]))
            {
                NewItem = (string)Session["NombreConferencia"];
                Session["NombreConferencia"] = null;
            }

            MuestraDatosInternos(DameDatosInternos());
            IniciaGestionConferencias();
          
            ActualizaWebPadre(true);
        }
        else
        {
            if (datos == null || Session["idsistema"]==null)
            {
                ScriptManager.RegisterStartupScript(this, typeof(Page), "redirect", "<Script language = 'Javascript'> window.parent.location='../Login.aspx' ; </Script>", false);
            }
        }
    }

    private ServiciosCD40.Tablas[] DameDatosInternos()
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

    private bool AlcanzadoMaximoNumeroConferencias()
    {
        bool alcanzado = false;
        if (LBConferencias.Items.Count >= NUM_MAX_CONFERENCIAS)
        {
            String strMsgAux = String.Empty;
            strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoNumeroMaximoConferencias"));
            cMsg.confirm(strMsgAux, "aceptparam");
            alcanzado = true;
        }
        return alcanzado;
    }

    private void MuestraDatosInternos(ServiciosCD40.Tablas[] nu)
    {
        if (lInternos != null)
            lInternos.Clear();
        else
            lInternos = new List<Participante>();
        if (participanteBCK == null)
            participanteBCK = new ParticipanteExterno();
        LBSectores.Items.Clear();
        if (nu != null)
            for (int i = 0; i < nu.Length; i++)
            {
                if (((ServiciosCD40.Sectores)nu[i]).IdSector != "**FS**")
                {
                    ListItem lbItem = new ListItem();
                    Participante part = new Participante();
                    int abonado = DameAbonado(((ServiciosCD40.Sectores)nu[i]).IdSistema, ((ServiciosCD40.Sectores)nu[i]).IdSector, ((ServiciosCD40.Sectores)nu[i]).IdNucleo);
                    if (abonado == 0)
                        continue;
                    part.SipUri = lbItem.Value = abonado.ToString();
                    StringBuilder strTexto = new StringBuilder();
                    strTexto.AppendFormat(FORMATO_LB_RECURSOS, abonado, ((ServiciosCD40.Sectores)nu[i]).IdSector);
                    strTexto.Replace(" ", "\u00A0");
                    lbItem.Text = strTexto.ToString();
                    LBSectores.Items.Add(lbItem);
                    part.Descripcion = ((ServiciosCD40.Sectores)nu[i]).IdSector;
                    lInternos.Add(part);
                }
            }
    }

    private int DameAbonado(string sistema, string sector, string nucleo)
    {
        List<int> abonados = new List<int>();
        int abonado = 0;
        try
        {
            DataSet d = serviceServiciosCD40.RedesUsuariosAbonados(sistema, nucleo, sector);
            if (null != d && d.Tables.Count > 0)
            {
                foreach (System.Data.DataRow ds in d.Tables[0].Rows)
                {
                    if (ds["IdPrefijo"] != DBNull.Value &&  (uint)ds["IdPrefijo"] == PREFIJO_ATS)
                    {
                        if (ds["IdAbonado"] != DBNull.Value)
                        {
                            abonados.Add(Int32.Parse((string)ds["IdAbonado"]));
                        }
                    }
                }
            }
            if (abonados.Count >0)
                abonado = abonados.Max();
        }
        catch (Exception ex)
        {
            logDebugView.Error("(Conferencias-CargarAbonados): ", ex);

        }
        return abonado;     
    }

    private ServiciosCD40.Tablas[] DameDatos()
    {
        try
        {
            ServiciosCD40.Conferencias c = new ServiciosCD40.Conferencias();
            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            c.IdSistema = s.Value;
            Session["idsistema"] = s.Value;

            ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(c);
            datos = d;
            return d;
        }
        catch (Exception e)
        {
            logDebugView.Error("(Conferencias-DameDatos):", e);
        }
        return null;
    }

    private ServiciosCD40.Tablas[] DameDatosParticipantes()
    {
        try
        {
            ServiciosCD40.ConferenciasParticipantes cp = new ServiciosCD40.ConferenciasParticipantes();
            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            cp.IdSistema = s.Value;
            cp.IdConferencia = TxtIdConferencia.Text;

            ServiciosCD40.Tablas[] d = ServicioCD40.ListSelectSQL(cp);
            datos = d;
            return d;
        }
        catch (Exception e)
        {
            logDebugView.Error("(Conferencias-DameDatos):", e);
        }
        return null;
    }

    private void MuestraDatos(ServiciosCD40.Tablas[] co)
    {
        RellenaPosicionHMI();
        LBConferencias.Items.Clear();
        if (co != null)
        {
            for (int i = 0; i < co.Length; i++)
            {
                LBConferencias.Items.Add(((ServiciosCD40.Conferencias)co[i]).IdConferencia);
                if (((ServiciosCD40.Conferencias)co[i]).PosHMI != 0)
                    DDLPosHMI.Items.Remove(((ServiciosCD40.Conferencias)co[i]).PosHMI.ToString());
            }
        }
        if (LBConferencias.Items.Count > 0)
        {
            ActualizaWebPadre(true);

            if (LBConferencias.Items.FindByText(NewItem) != null)
            {
                LBConferencias.Items.FindByText(NewItem).Selected = true;
                IndexListBox1 = LBConferencias.SelectedIndex;
                NewItem = string.Empty;
            }
            else
            {
                LBConferencias.SelectedIndex = IndexListBox1 != -1 && IndexListBox1 < LBConferencias.Items.Count ? IndexListBox1 : 0;
            }


            MostrarElemento();
        }
        HabilitaBotonesGestionConferencia();
	}

    private void HabilitaBotonesGestionConferencia()
    {
        if (LBConferencias.Items.Count > 0)
        {
            BtNuevo.Visible = BtModificar.Visible = BtEliminar.Visible = PermisoSegunPerfil;
            BtCancelar.Visible = false;
            BtAceptar.Visible = false;
            LBConferencias.Enabled = true;
        }
        else
        {
            BtNuevo.Visible = PermisoSegunPerfil;
            BtAceptar.Visible = BtCancelar.Visible = BtModificar.Visible = BtEliminar.Visible = false;
        }
    }

    private void RellenaPosicionHMI()
    {
        DDLPosHMI.Items.Clear();
        for (int i = 0; i < NUM_MAX_POSICIONES+1; i++) 
        {
            ListItem poshmi = new ListItem();
            poshmi.Text = poshmi.Value = i.ToString();
            DDLPosHMI.Items.Add(poshmi);
        }
    }

    private void MostrarElemento()
    {
        if (LBConferencias.Items.Count > 0 && null != datos)
        {
            ActualizaWebPadre(true);

            int elemento = LBConferencias.SelectedIndex;           
            InicializaDatosConferencia();
            TxtIdConferencia.Text = ((ServiciosCD40.Conferencias)datos[elemento]).IdConferencia;
            TxtIdSalaConferencia.Text = ((ServiciosCD40.Conferencias)datos[elemento]).IdSalaBkk;
            DDModoConferencia.SelectedValue = ((ServiciosCD40.Conferencias)datos[elemento]).TipoConferencia.ToString();
            ListItem poshmi = new ListItem();
            poshmi.Text = poshmi.Value = (((ServiciosCD40.Conferencias)datos[elemento]).PosHMI).ToString();
            DDLPosHMI.Items.Add(poshmi);
            DDLPosHMI.SelectedValue = (((ServiciosCD40.Conferencias)datos[elemento]).PosHMI).ToString();
            TxtDescripcion.Text = ((ServiciosCD40.Conferencias)datos[elemento]).Descripcion;
            TxtAlias.Text = ((ServiciosCD40.Conferencias)datos[elemento]).Alias;
        }
    }

    private void MuestraDatosParticipantes(ServiciosCD40.Tablas[] pa)
    {
        LBParticipantes.Items.Clear();
        if (lParticipantes != null)
            lParticipantes.Clear();
        else
            lParticipantes = new List<Participante>();
        if (pa != null)
            for (int i = 0; i < pa.Length; i++)
            {
                    ListItem lbItem = new ListItem();
                    lbItem.Value = ((ServiciosCD40.ConferenciasParticipantes)pa[i]).SipUri;
                    if (((ServiciosCD40.ConferenciasParticipantes)pa[i]).SipUri.Contains("sip:"))
                    {
                        lbItem.Text = ((ServiciosCD40.ConferenciasParticipantes)pa[i]).SipUri;
                    }
                    else
                    {
                        StringBuilder strTexto = new StringBuilder();
                        strTexto.AppendFormat(FORMATO_LB_RECURSOS, ((ServiciosCD40.ConferenciasParticipantes)pa[i]).SipUri, ((ServiciosCD40.ConferenciasParticipantes)pa[i]).Descripcion);
                        strTexto.Replace(" ", "\u00A0");
                        lbItem.Text = strTexto.ToString();
                    }
                    LBParticipantes.Items.Add(lbItem);
                    Participante part = new Participante();
                    part.SipUri = ((ServiciosCD40.ConferenciasParticipantes)pa[i]).SipUri;
                    part.Descripcion = ((ServiciosCD40.ConferenciasParticipantes)pa[i]).Descripcion;
                    lParticipantes.Add(part);
            }
    }

    protected void BtCancelar_Click(object sender, EventArgs e)
    {
        CancelarCambios();
    }

    protected void BtAceptar_Click(object sender, EventArgs e)
    {       
        if (NuevaConferencia && !IdentificadorValido(Tipo_Identificador.IDCONFERENCIA , TxtIdConferencia.Text) ||
            NuevaConferencia && IdConferenciaDuplicado() ||
            !IdentificadorValido(Tipo_Identificador.IDSALABKK , TxtIdSalaConferencia.Text) ||
            !ListaDeParticipantesCorrecta() || 
            IdSalaConferenciaEsAbonado() ||
            IdSalaConferenciaEsInterno() ||
            SalaBkkDuplicada()
            )
        {
          return;
        }
        ValidaPosicionHMI();
        if (GuardarCambios() == true)
        {
            IniciaGestionConferencias();
        }
    }

    private bool GuardarCambios()
    {
        bool OperacionOK = false;
        try
        {
            string strSistema = (string)Session["idsistema"];
            // Guardamos datos conferencia
            ServiciosCD40.Conferencias cn = new ServiciosCD40.Conferencias();
            cn.IdSistema = strSistema;
            cn.IdConferencia = TxtIdConferencia.Text;
            cn.IdSalaBkk = TxtIdSalaConferencia.Text;
            cn.TipoConferencia =  Convert.ToInt32(DDModoConferencia.SelectedValue);
            cn.PosHMI = Convert.ToInt32(DDLPosHMI.SelectedValue);
            cn.Descripcion = TxtDescripcion.Text;
            cn.Alias = TxtAlias.Text;
            if (NuevaConferencia)
            {
                if (ServicioCD40.InsertSQL(cn) > 0)
                {
                    OperacionOK = true;
                }
            }
            else
            {
                if (ServicioCD40.UpdateSQL(cn) > 0)
                {
                    OperacionOK = true;
                }
            }
            if (OperacionOK)
            {
                ServiciosCD40.ConferenciasParticipantes cp = new ServiciosCD40.ConferenciasParticipantes();
                cp.IdSistema = strSistema;
                cp.IdConferencia = TxtIdConferencia.Text;
                if (ServicioCD40.DeleteSQL(cp) > 0)
                {
                    foreach (Participante par in lParticipantes)
                    {
                        ServiciosCD40.ConferenciasParticipantes nparticipante = new ServiciosCD40.ConferenciasParticipantes();
                        nparticipante.IdSistema = strSistema;
                        nparticipante.IdConferencia = TxtIdConferencia.Text;
                        nparticipante.SipUri = par.SipUri;
                        nparticipante.Descripcion = par.Descripcion;
                        if (ServicioCD40.InsertSQL(nparticipante) < 0)
                        {
                            OperacionOK = false;
                            logDebugView.Warn("(ConferenciasParticipantes-GuardarCambios): No se ha podido guardar Participante.");
                        }
                    }
                }
            }
        }
        catch (Exception e)
        {
            logDebugView.Error("(Conferencias-GuardarCambios):", e);
        }
        return OperacionOK;
    }

    private void IniciaGestionConferencias()
    {
        Panel1.Enabled = false;
        MuestraDatos(DameDatos());
        MuestraDatosParticipantes(DameDatosParticipantes());
        PresentaPanelInternos(true);
        HabilitaDatosExternos(false);
        PresentaPanelExternos(false);
    }
    private bool ListaDeParticipantesCorrecta()
    {
        bool correcta = false;
        if (LBParticipantes.Items.Count >= NUM_MIN_PARTICIPANTES && LBParticipantes.Items.Count < NUM_MAX_PARTICIPANTES)
            correcta = true;
        else
        {
          String strMsgAux = String.Empty;
          strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoListaNumeroParticipantes"));
          cMsg.confirm(strMsgAux, "aceptparam");         
        }

        return correcta;
    }

    private bool IdSalaConferenciaEsAbonado()
    {
        bool esAbonado = false;
        foreach (Participante participante in lParticipantes)
        {
            if (participante.SipUri.Contains(TxtIdSalaConferencia.Text))
            {
                esAbonado = true;
                break;
            }
        }
        if (esAbonado)
        {
          String strMsgAux = String.Empty;
          strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoSalaAbonadoInvalido"));
          cMsg.confirm(strMsgAux, "aceptparam");
        }
        return esAbonado;
    }
    private bool IdSalaConferenciaEsInterno()
    {
        bool esInterno = true;

        if (String.Empty == ObtenDescripcionInterno(TxtIdSalaConferencia.Text))
            esInterno = false;
        else
        {
          String strMsgAux = String.Empty;
          strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoSalaInternoInvalido"));
          cMsg.confirm(strMsgAux, "aceptparam");
        }
        return esInterno;
    }
    


    private bool SalaBkkDuplicada()
    {
        bool duplicada = false;
        string idSistema = (string)Session["idsistema"];
        if (ServicioCD40.SalaBKKExiste(idSistema, TxtIdConferencia.Text, TxtIdSalaConferencia.Text))
        {
            duplicada = true;
            String strMsgAux = String.Empty;
            strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoSalaConferenciaDuplicada"));
            cMsg.confirm(strMsgAux, "aceptparam");
        }
        return duplicada;
    }

    private void ValidaPosicionHMI()
    {
        if (Convert.ToInt32(DDLPosHMI.SelectedValue) == 0)
        {
            String strMsgAux = String.Empty;
            strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoPOSHMIDebeAsignarse"));
            cMsg.confirm(strMsgAux, "aceptparam");
        }

    }

    private bool IdConferenciaDuplicado()
    {
        bool esDuplicado = false;
        ListItem conferencia = new ListItem();
        conferencia.Value = TxtIdConferencia.Text;
        if (LBConferencias.Items.Contains(conferencia))
        {
            esDuplicado = true;
            String strMsgAux = String.Empty;
            strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoIdConferenciaDuplicado"));
            cMsg.confirm(strMsgAux, "aceptparam");
        }
        return esDuplicado;
    }
    
    protected void BtModificar_Click(object sender, EventArgs e)
    {
        IndexListBox1 = LBConferencias.SelectedIndex;
        NuevaConferencia = false;
        ModoEdicion(true);
        HabilitaDatosConferencia(true);
        Panel1.Enabled = true;
        LBConferencias.Enabled = false;
        BtNuevo.Visible = false;
        BtEliminar.Visible = false;
        BtModificar.Visible = false;
        BtAceptar.Visible = true;
        BtCancelar.Visible = true;
    }

    protected void BtEliminar_Click(object sender, EventArgs e)
    {
        if (LBConferencias.SelectedValue != "")
        {
            IndexListBox1 = LBConferencias.SelectedIndex;
            Session["elemento"] = LBConferencias.SelectedValue;
            EliminarElemento();
        }
    }

    private void EliminarElemento()
    {
        try
        {
            ServiciosCD40.Conferencias n = new ServiciosCD40.Conferencias();
            n.IdSistema = (string)Session["idsistema"];
            n.IdConferencia = (string)Session["elemento"];
            if (ServicioCD40.DeleteSQL(n) < 0)
            {
                logDebugView.Warn("(Conferencias-EliminarElemento): no se ha borrado ela conferencia");
                cMsg.alert(String.Format((string)GetGlobalResourceObject("Espaniol", "ErrorEliminarConferencia"), n.IdConferencia));
            }
            LBParticipantes.Items.Clear();
            LBConferencias.Items.Clear();
            lParticipantes.Clear();
            IniciaGestionConferencias();
        }
        catch (Exception e)
        {
            logDebugView.Error("(Conferencias-EliminarElemento):", e);
        }
    }


    private void ModoEdicion(bool edicion)
    {
        Editando = edicion;
        LBConferencias.Enabled = !edicion;
        BtNuevo.Visible = !edicion && PermisoSegunPerfil;
        BtModificar.Visible = BtEliminar.Visible = LBConferencias.Items.Count > 0 && !edicion;
        BtCancelar.Visible = edicion;

    }

    private void GestionaExternos()
    {
        if (TBUsuario.Text != String.Empty)
        {
            // Puedo modificar + puedo añadir y puedo seleccionar de participantes volver a internos..
            BtExternoNuevo.Enabled = true;
            BtExternoModificar.Enabled = true;
            BtExternoCancelar.Enabled = false;
        }
        else
        {
            // Solo puedo añadir seleccionar de participantes volver a internos
            BtExternoNuevo.Enabled = true;
            BtExternoModificar.Enabled = false;
            BtExternoCancelar.Enabled = false;
        }
    }

    /// Eliminar participante interno/externo
    protected void IButQuitar_Click(object sender, ImageClickEventArgs e)
    {
        if (LBParticipantes.SelectedValue.Contains("sip:"))
            InicializaDatosExternos();
        BajaParticiparte();  
    }

    protected void BajaParticiparte()
    {
        string id_participante = String.Empty;
        Participante partx = null;
        if (LBParticipantes.SelectedIndex >= 0)
        {
            foreach (Participante part in lParticipantes)
            {
                if (part.SipUri.Contains(LBParticipantes.SelectedValue))
                {
                    partx = part;
                    break;
                }
            }
            lParticipantes.Remove(partx);
            LBParticipantes.Items.RemoveAt(LBParticipantes.SelectedIndex);
        }
    }



    /// Añadir participante interno 
    protected void IButAsignar_Click(object sender, ImageClickEventArgs e)
    {
        if (BtExternos.Enabled)
        {
            if (LBSectores.SelectedIndex >= 0)
            {
                AltaParticipanteInterno();
            }
        }
        else 
        {
            if (EditandoExternos == true)
                BajaParticiparte();

            if (AltaParticipanteExterno())
                IniciaGestionExternos();
        }
        HabilitaDatosConferencia(true);
    }

    protected bool AltaParticipanteInterno()
    {
        String strMsgAux = String.Empty;
        bool AltaOK = false;
        bool AvisoExiste = false;
        if (LBSectores.SelectedIndex >= 0)
        {
            for (int i = 0; i < LBSectores.Items.Count; i++)
            {
                if (LBSectores.Items[i].Selected)
                {
                    if (InternoNoExisteEnParticipantes(LBSectores.Items[i].Value))
                    {
                        if (NoAlcanzadoMaximoParticipantes())
                        {
                            Participante par = new Participante();
                            par.SipUri = LBSectores.Items[i].Value;
                            ListItem item = LBSectores.Items[i];
                            par.Descripcion = ObtenDescripcionInterno(par.SipUri);
                            lParticipantes.Add(par);
                            LBParticipantes.Items.Add(LBSectores.Items[i]);
                            LBSectores.SelectedItem.Selected = false;
                            AltaOK = true;
                        }
                        else
                            break;
                    }
                    else
                        AvisoExiste = true;

                }
            }
        }
        if (AvisoExiste)
        {
            strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoInternoExisteEnParticipantes"));
            cMsg.confirm(strMsgAux, "aceptparam");
        }
        LBSectores.SelectedIndex = -1;
        return AltaOK;
    }

    private string ObtenDescripcionInterno(string sipUri)
    {
        string descripcion = String.Empty;

        Participante  p = lInternos.Find(delegate(Participante i) { return i.SipUri == sipUri; });
        if (p != null)
            descripcion = p.Descripcion;
        return descripcion;
    }

    protected bool NoAlcanzadoMaximoParticipantes()
    {
        bool NoAlcanzado = true;
        String strMsgAux = String.Empty;
        if (LBParticipantes.Items.Count >= NUM_MAX_PARTICIPANTES)
        {
            strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoNumeroMaximoParticipantes"));
            cMsg.confirm(strMsgAux, "aceptparam");
            NoAlcanzado = false;
        }
        return NoAlcanzado;
    }

    protected bool InternoNoExisteEnParticipantes(string participante)
    {
        bool NoExiste = true;
        if(LBParticipantes.Items.Count >0)
            for (int i = 0; i< LBParticipantes.Items.Count; i++)
                if (LBParticipantes.Items[i].Value.Contains(participante))
                {
                    NoExiste = false;  
                }
        return NoExiste;
    }


    private bool AltaParticipanteExterno()
    {
        string externo = String.Empty;
        bool altaRealizada = false;
        String strMsgAux = String.Empty;
        if (IdentificadorValido(Tipo_Identificador.IDUSUARIO, TBUsuario.Text) && IPValida(TBIP.Text) && PuertoValido(TBPort.Text))
        {
            if (NoAlcanzadoMaximoParticipantes())
            {
                if (TBUsuario.Text != TxtIdSalaConferencia.Text)
                {
                    externo = ExternoNoExisteEnParticipantes(TBUsuario.Text, TBIP.Text, TBPort.Text);
                    if (externo != String.Empty)
                    {
                        InsertaExterno(externo, TxtDescripcionExt.Text);
                        EditandoExternos = false;
                        InicializaDatosExternos();
                        GestionaExternos();
                        altaRealizada = true;
                    }
                    else
                    {
                        strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoExternoExisteEnParticipantes"));
                        cMsg.confirm(strMsgAux, "aceptparam");
                    }
                }
                else
                {
                    strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoAbonadoSalaInvalido"));
                    cMsg.confirm(strMsgAux, "aceptparam");
                }
            }
        }
        return altaRealizada;
    }

    protected string ExternoNoExisteEnParticipantes(string usuario, string ip, string puerto )
    {
        string sUri = String.Empty;
        sUri = String.Format("sip:{0}@{1}:{2}", usuario, ip, puerto);

        foreach (Participante  par in lParticipantes)
        {
            if (par.SipUri.Contains(sUri) || par.SipUri.Contains(usuario))
            {
               sUri = String.Empty;
               break;
            }
        }
        return sUri;
    }

    private void InsertaExterno(string externo, string descripcion)
    {
        Participante par = new Participante();
        par.SipUri = externo;
        par.Descripcion = descripcion;
        lParticipantes.Add(par);

        ListItem lbItem = new ListItem();
        lbItem.Value = externo;
        lbItem.Text = externo.ToString();
        LBParticipantes.Items.Add(lbItem);
        LBParticipantes.SelectedIndex = -1;
    }
  
    protected void LBConferencias_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (LBConferencias.SelectedIndex >= 0)
        {
            BtEliminar.Visible = PermisoSegunPerfil;
            IndexListBox1 = LBConferencias.SelectedIndex;
            MuestraDatos(DameDatos());
            MuestraDatosParticipantes(DameDatosParticipantes());
        }
    }


    protected void LBParticipantes_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (LBParticipantes.SelectedIndex >= 0)
        {
            if (LBParticipantes.SelectedValue.Contains("sip:"))
            {
                ExtraeDatosExternos(LBParticipantes.SelectedValue);
                if (Editando)
                {
                    PresentaPanelExternos(true);
                    PresentaPanelInternos(false);
                    GestionaExternos();
                }
            }
        }
    }

    private void ExtraeDatosExternos(string valorexternos)
    {
        int inicio = 0;
        int fin = 0;
        inicio = valorexternos.IndexOf("sip:") + 4;
        fin = valorexternos.IndexOf("@");
        TBUsuario.Text = valorexternos.Substring(inicio, fin - inicio);

        inicio = valorexternos.IndexOf("@", 0, valorexternos.Length) + 1;
        fin = valorexternos.IndexOf(":", 4);
        TBIP.Text = valorexternos.Substring(inicio, fin - inicio);

        inicio = valorexternos.IndexOf(":" , 4);
        TBPort.Text = valorexternos.Substring(inicio + 1);
        if (LBParticipantes.SelectedIndex < lParticipantes.Count)
            TxtDescripcionExt.Text = lParticipantes[LBParticipantes.SelectedIndex].Descripcion;
        
        participanteBCK.Usuario = TBUsuario.Text;
        participanteBCK.DirIP = TBIP.Text;
        participanteBCK.Puerto = TBPort.Text;
        participanteBCK.Descripcion = TxtDescripcionExt.Text;
    }


    /// Nueva Conferencia
    protected void BtNuevo_Click(object sender, EventArgs e)
    {
        if (AlcanzadoMaximoNumeroConferencias())
            return;
        NuevaConferencia = true;
        Panel1.Enabled = true;
        HabilitaDatosConferencia(true);
        InicializaDatosConferencia();
        InicializaDatosExternos();
        InicializaDatosParticipantes();
        ModoEdicion(true);
            
        LBConferencias.Enabled = false;
        BtNuevo.Visible = false;
        BtEliminar.Visible = false;
        BtModificar.Visible = false;
        BtAceptar.Visible = true;
        BtCancelar.Visible = true;
       
    }
    protected override void CancelarCambios()
    {
        Panel1.Enabled = false;
        BtNuevo.Visible = PermisoSegunPerfil;
        LBConferencias.Enabled = true;
        BtCancelar.Visible = false;
        BtAceptar.Visible = false;
        NuevaConferencia = false;
        IniciaGestionConferencias();      
    }

    protected void BtInternos_Click(object sender, EventArgs e)
    {
        PresentaPanelInternos(true);
        PresentaPanelExternos(false);
    }

    protected void BtExternos_Click(object sender, EventArgs e)
    {
        PresentaPanelInternos(false);
        PresentaPanelExternos(true);
        GestionaExternos();
    }

    protected void BtExternoNuevo_Click(object sender, EventArgs e)
    {
        EditandoExternos = false;
        HabilitaDatosConferencia(false);
        HabilitaDatosExternos(true);
        HabilitaBtnGestionExternos(true);
        InicializaDatosExternos();
    }

    private void HabilitaBtnGestionExternos(bool habilita)
    {
        BtExternoModificar.Enabled = BtExternoNuevo.Enabled = !habilita;
        BtExternoCancelar.Enabled = habilita;
        IButQuitar.Enabled = BtInternos.Enabled = BtExternos.Enabled = !habilita;      
    }

    protected void BtExternoModificar_Click(object sender, EventArgs e)
    {
        EditandoExternos = true;
        HabilitaDatosConferencia(false);
        HabilitaDatosExternos(true);
        HabilitaBtnGestionExternos(true);
        BajaParticiparte();
    }

    private void RestauraDatosExterno()
    {
        TBUsuario.Text = participanteBCK.Usuario;
        TBIP.Text = participanteBCK.DirIP;
        TBPort.Text = participanteBCK.Puerto;
        TxtDescripcionExt.Text = participanteBCK.Descripcion;
        AltaParticipanteExterno();
    }

    private bool IdentificadorValido( Tipo_Identificador tipo, string identificador)
    {
        bool IdentificadorOK = true;
        String strMsgAux = String.Empty;
        if (!Regex.IsMatch(identificador, @"^[a-zA-Z0-9_]+$"))
        {
            switch (tipo)
            {
                case Tipo_Identificador.IDCONFERENCIA:
                    strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoConferenciaInvalida"));
                    break;
                case Tipo_Identificador.IDSALABKK:
                    strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoSalaInvalida"));
                    break;
                case Tipo_Identificador.IDUSUARIO:
                    strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoUsuarioInvalido"));
                    break;
                default:
                    strMsgAux = "TIPE ERROR";
                    break;
            }
            cMsg.confirm(strMsgAux, "aceptparam");
            IdentificadorOK = false;
        }
        if (IdentificadorOK && tipo == Tipo_Identificador.IDSALABKK)
        {
            long valornumerico = 0;
            if (long.TryParse(identificador, out valornumerico))
            {
                strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoSalaInvalidaNumerica"));
                cMsg.confirm(strMsgAux, "aceptparam");
                IdentificadorOK = false;
            }
        }
        return IdentificadorOK;
    }

    private bool IPValida(string sIP)
    {
        bool valida = true;
        byte Conversiontemp;
        String strMsgAux = String.Empty;

        if (String.IsNullOrWhiteSpace(sIP))
        {
            valida =  false;
        }
        string[] puntos = sIP.Split('.');
        if (puntos.Length != 4)
        {
             valida =   false;
        }
       
        if ( false ==puntos.All(r => byte.TryParse(r, out Conversiontemp)))
        {
             valida = false;
        }
        if (!valida)
        {
            strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoIPInvalida"));
            cMsg.confirm(strMsgAux, "aceptparam");
        }
        return valida;
    }

    private bool PuertoValido(string puerto)
    {
        bool puertoOK = false;
        String strMsgAux = String.Empty;
        if (puerto != String.Empty && Int32.Parse(puerto) > PUERTO_SIP_MIN && Int32.Parse(puerto) < PUERTO_SIP_MAX )
        {
            puertoOK = true;
        }
        else
        {
            strMsgAux = string.Format((string)GetLocalResourceObject("TXTAvisoPuertoSIPInvalido"));
            cMsg.confirm(strMsgAux, "aceptparam");
        }


        return puertoOK;
    }

    protected void BtExternoCancelar_Click(object sender, EventArgs e)
    {
        if (Editando && EditandoExternos)
        {
            RestauraDatosExterno();
        }
        IniciaGestionExternos();
    }

    private void IniciaGestionExternos()
    {
        HabilitaDatosConferencia(true);
        HabilitaDatosExternos(false);
        HabilitaBtnGestionExternos(false);
        InicializaDatosExternos();
        PresentaPanelExternos(true);
        GestionaExternos();
    }

    private void HabilitaDatosConferencia(bool habilita)
    {
        if (habilita && NuevaConferencia)
            TxtIdConferencia.Enabled = habilita;
        else if ((habilita && !NuevaConferencia) || !habilita)
                TxtIdConferencia.Enabled = false;
        TxtIdSalaConferencia.Enabled = habilita;
        DDModoConferencia.Enabled = habilita;
        DDLPosHMI.Enabled = habilita;
        TxtDescripcion.Enabled = habilita;
        TxtAlias.Enabled = habilita;
    }

    private void InicializaDatosConferencia()
    {
        TxtIdConferencia.Text = String.Empty;
        TxtIdSalaConferencia.Text = String.Empty;
        DDModoConferencia.SelectedIndex = 0;
        DDLPosHMI.SelectedIndex = -1;
        TxtDescripcion.Text = String.Empty;
        TxtAlias.Text = String.Empty;
    }

    private void InicializaDatosParticipantes()
    {
        LBParticipantes.Items.Clear();
        if (lParticipantes != null)
            lParticipantes.Clear();
    }

    private void HabilitaDatosExternos(bool habilita)
    {
        TBUsuario.Enabled = habilita;
        TBIP.Enabled = habilita;
        TBPort.Enabled = habilita;
        TxtDescripcionExt.Enabled = habilita;
    }

    private void InicializaDatosExternos()
    {
        TBUsuario.Text = String.Empty;
        TBIP.Text = String.Empty;
        TBPort.Text = SPUERTO_SIP_DEFECTO;
        TxtDescripcionExt.Text = String.Empty;
    }

    private void PresentaPanelExternos(bool presenta)
    {
        PanelExternos.Enabled = presenta;
        LblExternos.Visible = presenta;
        PanelExternos.Visible = presenta;
        BtExternos.Visible =  BtInternos.Visible = true;
        BtExternos.Enabled = !presenta;
        BtInternos.Enabled = presenta;
    }

    private void PresentaPanelInternos(bool presenta)
    {
        LblSectores.Visible = presenta;
        LBSectores.Visible = presenta;
        BtExternos.Visible = BtInternos.Visible = true;
        BtExternos.Enabled = presenta;
        BtInternos.Enabled = !presenta;
    }
}