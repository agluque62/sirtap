using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Text;
using System.Text.RegularExpressions;
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

public partial class Operadores : PageBaseCD40.PageCD40	// System.Web.UI.Page
{
    // RQF-28
    private static int TIMEOUTSESION = 30;
    private static int TIMEOUTSESIONMIN = 5;
    private static int TIMEOUTSESIONMAX = 60;
    private static int _SesionActiva = 0;
    private static int _SesionEstado = 0;
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

    /// <summary>
    /// 
    /// </summary>
    private static bool ControlDependenciasATS = false;

    private static bool bConfiguracionOffline = false;

    /// <summary>
    /// 
    /// </summary>
    private enum accion { alta = 0, baja = 1, modificacion = 2, presentacion = 3 };


	protected new void Page_Load(object sender, EventArgs e)
    {
		base.Page_Load(sender, e);

		cMsg = (Mensajes.msgBox)this.Master.FindControl("MsgBox1");

		if (Context.Request.IsAuthenticated)
		{
			// retrieve user's identity from httpcontext user 
			FormsIdentity ident = (FormsIdentity)Context.User.Identity;
			string perfil = ident.Ticket.UserData;
			if (perfil != "3")	// Sólo el perfil 3 (Técnico 3) tiene acceso a Operadores.
			{
                Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO", false);
				return;
			}
		}

        if (!IsPostBack)
        {
            // Servicio.NoTransaction();
            BtnAceptar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "AceptarCambios");
            BtnCancelar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "CancelarCambios");
            
            //Se lee la variable de sesión idsistema
            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            Session["idsistema"] = s.Value;

            string sOffline = config.AppSettings.Settings["ConfiguracionOffline"].Value;
            if ((sOffline != null) && (!string.IsNullOrEmpty(sOffline) && sOffline.Contains("SI")))
            {
                //El sistema está configurado en modo OFF LINE
                bConfiguracionOffline = true;
            }


            IndexListBox1 = -1;

            //20210317 #4749
            // RQF-4 solo nucleos y se relacionan
            ControlDependenciasATS = false; // SIRTAP
            //OcultaInformacionUsuario(); //SIRTAP
           
            if (!ControlDependenciasATS)
            {
                OcultaDependenciasATS();
            }
            else
            {
                CargaDatosDependenciasATSNucleo(DameDatosDependenciasATSNucleo());
            }

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

    private void PresentaDependenciasATS()
    {
        LabelDepATS.Visible = true;
        PanelDepATS.Visible = true;
        IButAsignar.Visible = true;
        IButQuitar.Visible = true;
        ListAsignadas.Visible = true;
        ListExistentes.Visible = true;
        LabelAsig.Visible = true;
        LabelExist.Visible = true;
    }

    private void OcultaDependenciasATS()
    {
        PanelDepATS.Visible = false;
        LabelDepATS.Visible = false;
        IButAsignar.Visible = false;
        IButQuitar.Visible = false;
        ListAsignadas.Visible = false;
        ListExistentes.Visible = false;
        LabelAsig.Visible = false;
        LabelExist.Visible = false;
    }

	private void CancelarOperador()
	{
		if (!Modificando)
		{
			if (LBOperadores.Items.Count > 0)
                LBOperadores.SelectedIndex = IndexListBox1 != -1 ? IndexListBox1 : 0;
            
            MostrarElemento();
		}

		TerminaActualizacion();
	}


	private void EliminaOperador()
	{
		ServiciosCD40.Operadores op = new ServiciosCD40.Operadores();
		op.IdSistema = (string)Session["idsistema"];
		op.IdOperador = TBUsuario.Text;

		if (Servicio.DeleteSQL(op) > 0)
		{
            EliminaAsignacionDependenciasATS();
			cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ElementoEliminado"));
			TerminaActualizacion();
		}
	}


	private void AnadirOperador()
	{
		ServiciosCD40.Operadores op = new ServiciosCD40.Operadores();
		op.IdSistema = (string)Session["idsistema"];
		op.IdOperador = TBUsuario.Text;
		op.Clave = HFClave.Value;
		op.NivelAcceso = Convert.ToUInt32(DDLPerfil.SelectedValue);
		op.Nombre = TBNombre.Text;
		op.Apellidos = TBApellidos.Text;
		op.Telefono = TBTelefono.Text;
        op.FechaUltimoAcceso = DateTime.Today;
        op.SesionActiva = 0;
        op.SesionEstado = 0;
        if (!EsValidoUsuarioClave())
            return;
        // RQF-28
        int timeoutvalido = 0;
        if (!TimeoutSesionValido(out timeoutvalido))
        {
            return;
        }
        op.TimeoutSesion = timeoutvalido;

        NewItem = TBUsuario.Text;

		if (Servicio.InsertSQL(op) > 0)
		{
            InsertaAsignacionDependenciasATS();
			cMsg.alert((string)GetGlobalResourceObject("Espaniol", "OperadorDadoDeAlta"));
			TerminaActualizacion();

			ActualizaWebPadre(true);
        }
	}


	private void ModificarOperador()
	{
		ServiciosCD40.Operadores op = new ServiciosCD40.Operadores();
		op.IdSistema = (string)Session["idsistema"];
		op.IdOperador = TBUsuario.Text;
		op.Clave = HFClave.Value;
		op.NivelAcceso = Convert.ToUInt32(DDLPerfil.SelectedValue);
		op.Nombre = TBNombre.Text;
		op.Apellidos = TBApellidos.Text;
		op.Telefono = TBTelefono.Text;
        op.FechaUltimoAcceso = DateTime.Today;
        // RQF-28
        int timeoutvalido = 0;
        if (!TimeoutSesionValido(out timeoutvalido))
        {
            return;
        }
        op.TimeoutSesion = timeoutvalido;
        op.SesionActiva = _SesionActiva;
        op.SesionEstado = _SesionEstado;

        IndexListBox1 = LBOperadores.SelectedIndex;

		if (Servicio.UpdateSQL(op) > 0)
		{
            InsertaAsignacionDependenciasATS();
			cMsg.alert((string)GetGlobalResourceObject("Espaniol", "OperadorModificado"));

			TerminaActualizacion();
		}
	}

    private bool TimeoutSesionValido(out int TimeoutSesion)
    {
        //Se comprueba que el valor TimeoutSesion es correcto
       
        String strMsgAux = String.Empty;
        int iTimeoutSesion = 0;
        bool timeoutvalido = false;

        TimeoutSesion = TIMEOUTSESION;
        //El valor debe estar en el rango 10..120 Por defecto son 30 minutos
        if (Int32.TryParse(TBTimeoutSesion.Text, out iTimeoutSesion) &&
            (iTimeoutSesion >= TIMEOUTSESIONMIN && iTimeoutSesion <= TIMEOUTSESIONMAX))
        {
            TimeoutSesion = iTimeoutSesion;
            timeoutvalido = true;
        }
        else
        {
            //El valor TimeoutSesion no es correcto
            strMsgAux = string.Format((string)GetGlobalResourceObject("Espaniol", "RangoTimeoutSesion"));
            cMsg.confirm(strMsgAux, "aceptparam");
            timeoutvalido = false;
        }
        return timeoutvalido;
    }

	private ServiciosCD40.Tablas[] DameDatos()
	{
		try
		{
			ServiciosCD40.Operadores t = new ServiciosCD40.Operadores();
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
			logDebugView.Error("(Operadores-DameDatos):", e);
		}
		return null;
	}


	private void MuestraDatos(ServiciosCD40.Tablas[] nu)
	{
		LBOperadores.Items.Clear();
		if (nu != null)
			for (int i = 0; i < nu.Length; i++)
				LBOperadores.Items.Add(((ServiciosCD40.Operadores)nu[i]).IdOperador);
		if (LBOperadores.Items.Count > 0)
		{
            if (LBOperadores.Items.FindByText(NewItem) != null)
                LBOperadores.Items.FindByText(NewItem).Selected = true;
            else
                LBOperadores.SelectedIndex = IndexListBox1 != -1 && IndexListBox1 < LBOperadores.Items.Count ? IndexListBox1 : 0;

			BtnModificar.Visible = BtnEliminar.Visible = true;
			MostrarElemento();
            BtnEliminar_ConfirmButtonExtender.ConfirmText = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarOperador"), LBOperadores.SelectedValue);
        }
		else
			BtnModificar.Visible = BtnEliminar.Visible = false;
	}

    // RQF-4
    private ServiciosCD40.Tablas[] DameDatosDependenciasATSNucleo()
    {
        try
        {
            ServiciosCD40.Nucleos da = new ServiciosCD40.Nucleos();
            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            da.IdSistema = s.Value;
            Session["idsistema"] = s.Value;

            ServiciosCD40.Tablas[] d = Servicio.ListSelectSQL(da);
            datos = d;
            return d;
        }
        catch (Exception e)
        {
            logDebugView.Error("(Operadores-DameDatosDependenciasATS):", e);
        }
        return null;
    }

    // RQF-4
    private void CargaDatosDependenciasATSNucleo(ServiciosCD40.Tablas[] nu)
    {
        ListExistentes.Items.Clear();
        if (nu != null)
        {
            for (int i = 0; i < nu.Length; i++)
            {
                if (((ServiciosCD40.Nucleos)nu[i]).IdSistema != (string)Session["idsistema"])
                {
                    continue;
                }
                ListExistentes.Items.Add(((ServiciosCD40.Nucleos)nu[i]).IdNucleo);
            }
        }
    }


	private void MostrarElemento()
	{
		if (LBOperadores.Items.Count > 0 && null!=datos)
		{
			ActualizaWebPadre(true);

			int elemento = LBOperadores.SelectedIndex;
			TBUsuario.Text = ((ServiciosCD40.Operadores)datos[elemento]).IdOperador;
			HFClave.Value = ((ServiciosCD40.Operadores)datos[elemento]).Clave;
			DDLPerfil.SelectedValue = ((ServiciosCD40.Operadores)datos[elemento]).NivelAcceso.ToString();
			TBNombre.Text = ((ServiciosCD40.Operadores)datos[elemento]).Nombre;
			TBApellidos.Text = ((ServiciosCD40.Operadores)datos[elemento]).Apellidos;
			TBTelefono.Text = ((ServiciosCD40.Operadores)datos[elemento]).Telefono;
            // RQF-28
            TBTimeoutSesion.Text = ((ServiciosCD40.Operadores)datos[elemento]).TimeoutSesion.ToString();
            _SesionActiva = ((ServiciosCD40.Operadores)datos[elemento]).SesionActiva;
            _SesionEstado = ((ServiciosCD40.Operadores)datos[elemento]).SesionEstado;

            // 20210317 #4749
            try
            {
                if (ControlDependenciasATS)
                {
                    if (((ServiciosCD40.Operadores)datos[elemento]).NivelAcceso == 0)
                    {
                        PresentaDependenciasATS();
                        MostrarDependenciasAST(accion.presentacion);
                    }
                    else
                    {
                        OcultaDependenciasATS();
                    }
                }
            }
            catch (ArgumentOutOfRangeException)
            {
                //DListDepATS.SelectedIndex = 0;
            }
		}
	}

	protected void BtnModificar_Click(object sender, EventArgs e)
	{
		Modificando = true;
		Panel1.Enabled = true;
		BtnAceptar.Visible = BtnCancelar.Visible = true;
		BtnEliminar.Visible = BtnNuevo.Visible = BtnModificar.Visible = false;
		LBOperadores.Enabled = false;
		TBUsuario.Enabled = false;
		LkBCambiarClave.Visible = true;
    
        IndexListBox1 = LBOperadores.SelectedIndex;
        // 20210317 #4749
        if (ControlDependenciasATS)
        {
            MostrarDependenciasAST(accion.modificacion);
        }
    }

	private void ResetItems()
	{
        TBUsuario.Text = string.Empty;
		TBClave.Text=string.Empty;
		TBConfirmaClave.Text = string.Empty;
		TBNombre.Text = string.Empty;
		TBApellidos.Text = string.Empty;
		TBTelefono.Text = string.Empty;
        TBTimeoutSesion.Text = string.Empty;
		DDLPerfil.SelectedIndex = 0;
	}

	protected void BtnNuevo_Click(object sender, EventArgs e)
	{
        ResetItems();
		Modificando = false;
		Panel1.Enabled = true;
		Label8.Visible = TBConfirmaClave.Visible = true;
		Label2.Visible = TBClave.Visible = true;
		BtnAceptar.Visible = BtnCancelar.Visible = true;
		BtnEliminar.Visible = BtnNuevo.Visible = BtnModificar.Visible = false;
		LBOperadores.Enabled = false;
		TBUsuario.Enabled = true;
        // RQF-18
        TBTimeoutSesion.Enabled = true;
        TBTimeoutSesion.Text = TIMEOUTSESION.ToString();
        // 20210317 #4749
        if (ControlDependenciasATS)
        {
            if (DDLPerfil.SelectedIndex == 0)
            {
                PresentaDependenciasATS();
                MostrarDependenciasAST(accion.alta);
            }
        }
        TBUsuario.Text = string.Empty;
        HFClave.Value = string.Empty;
	}

	protected void BtnCancelar_Click(object sender, EventArgs e)
	{
        CancelarOperador();
		cMsg.confirm((string)GetGlobalResourceObject("Espaniol", "CancelarCambios"), "cancelaElemento");
	}

	private void TerminaActualizacion()
	{
		LBOperadores.Enabled = true;
		LblErrorClaveAntigua.Visible = false;
		LblErrorConfirmacion.Visible = false;
		LblMensajeConfirmacion.Visible = false;
		Modificando = false;
		Panel1.Enabled = false;
		Label8.Visible = TBConfirmaClave.Visible = false;
		Label2.Visible = TBClave.Visible = false;
		Label9.Visible = TBClaveAntigua.Visible = false;
		BtnEliminar.Visible = BtnEliminar.Visible = LBOperadores.Items.Count > 0;
		BtnNuevo.Visible = true;
		BtnAceptar.Visible = BtnCancelar.Visible = false;
		LkBCambiarClave.Visible = false;

		MuestraDatos(DameDatos());
	}

	protected void BtnAceptar_Click(object sender, EventArgs e)
	{
		if (Modificando && TBClaveAntigua.Visible && HFClave.Value != TBClaveAntigua.Text)
		{
			LblErrorClaveAntigua.Visible = true;
			LblMensajeConfirmacion.Text = (string)GetLocalResourceObject( "MsgClaveAntiguaVacia");
			LblMensajeConfirmacion.Visible = true;
		}
		else if (TBClave.Visible && TBClave.Text != TBConfirmaClave.Text)
		{
			LblErrorConfirmacion.Visible = true;
            LblMensajeConfirmacion.Text = (string)GetLocalResourceObject("MsgConfirmeClave");
			LblMensajeConfirmacion.Visible = true;
		}
        else if (TBClave.Visible && (TBClave.Text  == string.Empty || TBConfirmaClave.Text == string.Empty))
        {
            LblErrorConfirmacion.Visible = true;
            LblMensajeConfirmacion.Text = (string)GetLocalResourceObject("MsgClaveVacia");
            LblMensajeConfirmacion.Visible = true;
        }
		else
		{
            if (!Modificando || TBClave.Visible)
                HFClave.Value = TBClave.Text;
            if (EsValidoUsuarioClave())
            {
                if (Modificando)
                    ModificarOperador();
                else
                    AnadirOperador();
            }
		}
	}

	protected void LkBCambiarClave_Click(object sender, EventArgs e)
	{
		Label8.Visible = TBConfirmaClave.Visible = true;
		Label2.Visible = TBClave.Visible = true;
		Label9.Visible = TBClaveAntigua.Visible = true;
	}

    protected bool EsValidoUsuarioClave()
    {
        bool valido = true;
        if (Convert.ToUInt32(DDLPerfil.SelectedValue) == 5) // Perfil operador de misión
        {
            if (!Regex.IsMatch(TBUsuario.Text, @"^[0-9_@#]+$"))
            {
                cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ErrorIdentificadorNoValido"));
                valido = false;
            }

            if (!Regex.IsMatch(HFClave.Value, @"^[0-9_@#]+$"))
            {
                cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ErrorIdentificadorNoValido"));
                valido = false;
            }
        }
        else
        {
            if (!Regex.IsMatch(TBUsuario.Text, @"^[a-zA-Z0-9_@#.-\u00f1\u00d1\u00E0\u00FC-]+$"))
            {
                cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ErrorIdentificadorNoValido"));
                valido = false;
            }
        }

        return valido;
    }



	protected void LBOperadores_SelectedIndexChanged(object sender, EventArgs e)
	{
        if (LBOperadores.SelectedIndex >= 0)
        {
            BtnEliminar_ConfirmButtonExtender.ConfirmText = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarOperador"), LBOperadores.SelectedValue);
            MostrarElemento();
        }
	}

    protected void DDLPerfil_SelectedIndexChanged(object sender, EventArgs e)
	{
        // 20210317 #4749
        if (ControlDependenciasATS)
        {
            if (DDLPerfil.SelectedIndex == 0)
            {
                PresentaDependenciasATS();
                MostrarDependenciasAST(accion.alta);
            }
            else
            {
                OcultaDependenciasATS();
            }
        }

	}


	protected void BtnEliminar_Click(object sender, EventArgs e)
	{
        EliminaOperador();
	}


    // 20210317 #4749
    private void MostrarDependenciasAST(accion procesa)
    {    
        if (procesa == accion.alta)
        {
            PanelDepATS.Enabled = true;
            IButAsignar.Enabled = true;
            IButQuitar.Enabled = true;
            ListAsignadas.Enabled = true;
            ListExistentes.Enabled = true;
            ListAsignadas.Items.Clear();
        }
        else 
        {
            CargaDependenciasATSOperador();
            if (procesa == accion.modificacion)
            {
                PanelDepATS.Enabled = true;
                IButAsignar.Enabled = true;
                IButQuitar.Enabled = true;
                ListAsignadas.Enabled = true;
                ListExistentes.Enabled = true;
            }
            else
            {
                PanelDepATS.Enabled = false;
                PanelDepATS.Enabled = false;
                IButAsignar.Enabled = false;
                IButQuitar.Enabled = false;
                ListAsignadas.Enabled = false;
                ListExistentes.Enabled = false;
            }
        }
    }

 
    // 20210317 #4749
    private void CargaDependenciasATSOperador()
    {
        ServiciosCD40.ServiciosCD40 g = new ServiciosCD40.ServiciosCD40();
        ServiciosCD40.PermisosOpeDepATS permATS = new ServiciosCD40.PermisosOpeDepATS();
        permATS.IdSistema = (string)Session["idsistema"];
        permATS.IdOperador = TBUsuario.Text;
        ServiciosCD40.Tablas[] d = g.ListSelectSQL(permATS);
        ListAsignadas.Items.Clear();

        if (d != null && d.Length > 0)
        {
            for (int i = 0; i < d.Length; i++)
            {
                if (((ServiciosCD40.PermisosOpeDepATS)d[i]).IdDependenciaATS == (string)Session["idsistema"])
                {
                    continue;
                }
                ListAsignadas.Items.Add(((ServiciosCD40.PermisosOpeDepATS)d[i]).IdDependenciaATS);
               
            }
        }
    }


    private void EliminaAsignacionDependenciasATS()
    {
        ServiciosCD40.ServiciosCD40 g = new ServiciosCD40.ServiciosCD40();
        ServiciosCD40.PermisosOpeDepATS permATS = new ServiciosCD40.PermisosOpeDepATS();
        permATS.IdSistema = (string)Session["idsistema"];
        permATS.IdOperador = TBUsuario.Text;
        Servicio.DeleteSQL(permATS);
    }


    private void InsertaAsignacionDependenciasATS()
    {
        EliminaAsignacionDependenciasATS();

        ServiciosCD40.PermisosOpeDepATS iOpeATS = new ServiciosCD40.PermisosOpeDepATS();
        iOpeATS.IdSistema = (string)Session["idsistema"];
        iOpeATS.IdOperador = TBUsuario.Text;
        for (int i = 0; i < ListAsignadas.Items.Count; i++)
		{
            iOpeATS.IdDependenciaATS = ListAsignadas.Items[i].Text;
            if (Servicio.InsertSQL(iOpeATS) < 0)
                logDebugView.Warn("(InsertaAsignacionDependenciasATS): No se ha podido insertar el elemento--> " + ListAsignadas.Items[i].Text);
        }
    }

    protected void IButQuitar_Click(object sender, ImageClickEventArgs e)
    {
        if (ListAsignadas.SelectedIndex >= 0)
        {
            ListAsignadas.Items.RemoveAt(ListAsignadas.SelectedIndex);
        }
    }

    protected void IButAsignar_Click(object sender, ImageClickEventArgs e)
    {
        if (ListExistentes.SelectedIndex >= 0)
        {
            if (!ListAsignadas.Items.Contains(ListExistentes.Items[ListExistentes.SelectedIndex]))
            {
                ListAsignadas.Items.Add(ListExistentes.Items[ListExistentes.SelectedIndex].Text);
            }
        }
    }

    private void OcultaInformacionUsuario()
    {
        Label4.Visible = Label5.Visible = Label7.Visible = LBLTBTimeoutSesion.Visible = false;
        TBNombre.Visible = TBApellidos.Visible = TBTelefono.Visible = TBTimeoutSesion.Visible = false;
    }
}
