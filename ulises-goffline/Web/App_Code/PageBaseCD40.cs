using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using ServiciosCD40;
/// <summary>
/// Summary description for PageBaseCD40
/// </summary>
namespace PageBaseCD40
{
	public class PageCD40 : System.Web.UI.Page, System.Web.UI.ICallbackEventHandler
	{
		string _Command;
		protected static bool Sectorizando = false;
		protected static bool RecargarDatosEnPagina = false;
        protected static int IndexListBox1 = -1;
        protected static int IndexListBox2 = -1; // RQF-34

		protected Mensajes.msgBox cMsg;
        private static ServiciosCD40.ServiciosCD40 ServicioParaTransaccion = new ServiciosCD40.ServiciosCD40();
		protected static int EstadoEnlaceSacta = 0;
		private static string UltimaVersionSectorizacion = string.Empty;
        protected static string NewItem = string.Empty;

        protected static bool bSactaActivoEnCnf; //Indica si por configuraci�n, se ha activado la funcionalidad SACTA en la Web de Configuraci�n
        protected static bool bVisualizandoActiva; //Almacena si el usuario est� visualizando la sectorizaci�n activa

		//private static bool ContinuarConLaTransaccion = false;

		private AsyncCallback CallbackCompletado;


		//static int TiempoPendienteTransaccion = 0;

		public PageCD40()
		{
		}

		protected override void OnInit(System.EventArgs e)
		{
			BuildPage(this.Form);
			//BuildPage(GenerateHtmlForm());
			base.OnInit(e);
		}

		protected void ActualizaWebPadre(bool habilita)
		{
			string webPadreScript = "<script language='JavaScript'>" +
				"HabilitaBotonSiguiente(" + (habilita ? "false" : "true") + ")</script>";

			if (!ClientScript.IsClientScriptBlockRegistered("WebPadreScript"))
				ClientScript.RegisterStartupScript(this.GetType(), "WebPadreScript", webPadreScript, false);
		}

		protected void BuildPage(HtmlForm form)
		{		
			this.Controls.AddAt(0, new LiteralControl(@"
            <script id='Base' type='text/javascript'>		
	            function HabilitaBotonSiguiente(argumento) {
		            var btnSiguiente = parent.document.getElementById('BtnSiguiente');
		            if (btnSiguiente) {
			            btnSiguiente.disabled=argumento;
		            }
	            }
            </script>"));
		}

		private HtmlForm GenerateHtmlForm()
		{
			HtmlForm form = new HtmlForm();
			form.ID = "FormBase"; form.Name = "FormBase";
			AddControlsFromDerivedPage(form);
			return this.Form;
		}

		private void AddControlsFromDerivedPage(HtmlForm form)
		{
			int count = this.Controls.Count;
			for (int i = 0; i < count; ++i)
			{
				System.Web.UI.Control ctrl = this.Controls[i];
				form.Controls.Add(ctrl);
				this.Controls.Remove(ctrl);
			}
		}

		protected void Page_Load(object sender, EventArgs e)
		{
			cMsg = (Mensajes.msgBox)this.Master.FindControl("MsgBox1");
			if (!ClientScript.IsClientScriptBlockRegistered("TransaccionEnCurso"))
			{
				string cbReference = ClientScript.GetCallbackEventReference(this, "arg", "GetTransactionStatus", "context", "ClientCallbackError", true);
				string callbackScript = "function CallServerTransaction(arg, context) {" + cbReference + "; }";
				ClientScript.RegisterStartupScript(this.GetType(), "TransaccionEnCurso", callbackScript, true);
				// RegisterClientScriptBlock(this.GetType(), "TransaccionEnCurso", callbackScript, true);
				//this.RegisterClientScriptBlock("TransaccionEnCurso", callbackScript);
			}

			if (CallbackCompletado == null)
				CallbackCompletado = new AsyncCallback(OnCallBackCompleted);


            if (Context.Session != null)
            {
                if (Session.IsNewSession)
                {
                    //Si se ha creado una nueva sesi�n, cuando la sesi�n ya estaba iniciado porque se ha conmutado de servidor
                    string cookieHeader = Request.Headers["Cookie"];
                    if ((null != cookieHeader) && (cookieHeader.IndexOf("ASP.NET_SessionId") >= 0))
                    {

                        Session.Abandon();

                        ScriptManager.RegisterStartupScript(this, typeof(Page), "redirect", "<Script language = 'Javascript'> window.parent.location='../Login.aspx' ; </Script>", false);

                        /*
                        string striframe = Request.QueryString["iframe"];


                        if (!string.IsNullOrEmpty(striframe) && striframe.Equals("true"))
                        {
                            //Si estamos dentro del frame del wizard
                            //Se abandona la sesi�n y se va a la p�gina de login
                            Session.Abandon();

                            ScriptManager.RegisterStartupScript(this, typeof(Page), "redirect", "<Script language = 'Javascript'> window.parent.location='../Login.aspx' ; </Script>", false);

                        }
                        else
                        {
                            //Si la p�gina est� dentro de una pantalla de la Web de configuraci�n
                            //Se va a la p�gina de login
                            //Response.Redirect("~/Login.aspx", true);
                            Session.Abandon();
                            ScriptManager.RegisterStartupScript(this, typeof(Page), "redirect", "<Script language = 'Javascript'> window.parent.location='../Login.aspx' ; </Script>", false);
                        }
                         
                         */ 

                        return;

                    }
                }
            }


			if (!IsPostBack)
			{
                //Se lee el par�metro iframe para conocer si la pantalla se est� invocando desde el Wizard (iframe=true) o desde las pantallas de configuraci�n 
                // para poder activar la cabecera y el �rbol con la lista de opciones correspondiente
                // Wizard --> DivWizard (lista de botones de cada paso)
                // Configuraci�n --> DivArbol (�rbol del men�), DivCabecera --> cabecera de la pantalla.

                string striframe = Request.QueryString["iframe"];
                if (!string.IsNullOrEmpty(striframe) && striframe.Equals("true"))
                {
                    // Se est� invocando desde el Wizard
                    //Se ocultan la cabecera y el �rbol de la pagina de configuraci�n
                    Control division = this.Master.FindControl("DivArbol");
                    if (division != null && division.Visible)
                        division.Visible = false;
                    division = this.Master.FindControl("DivCabecera");
                    if (division != null && division.Visible)
                        division.Visible = false;

                    //Se hace visible el arbol de men� del Wizard. El div de la cabecera del Wizard no es necesario
                    //porque no est� definido en la pagina base CD40.master, sino est� en la misma p�gina del wizard
                    division = this.Master.FindControl("DivWizard");
                    if (division != null && !division.Visible)
                        division.Visible = true;
                }
                else
                {
                    // Se est� invocando desde la pagina de configuracion
                    //Se oculta el �rbol del Wizard
                    Control division = this.Master.FindControl("DivWizard");
                    if (division != null && division.Visible)
                        division.Visible = false;

                    //Se hace visible la cabecera y el arbol de men� de configuraci�n
                    division = this.Master.FindControl("DivArbol");
                    if (division != null && !division.Visible)
                        division.Visible = true;
                    division = this.Master.FindControl("DivCabecera");
                    if (division != null && !division.Visible)
                        division.Visible = true;

                }

                bSactaActivoEnCnf = bSistemaConSactaHabilitadoPorCnf();
                bVisualizandoActiva = false;
			}
		}

        private void ShowMessageBox()
		{
			AjaxControlToolkit.ModalPopupExtender c = (AjaxControlToolkit.ModalPopupExtender)Master.FindControl("ModalPopupExtenderMessageBox");
			if (c != null)
				c.Show();
		}

		protected virtual void CancelarCambios() { }

		protected virtual void AceptarCambios() 
		{
			//ComunicaSectorizacion();
		}
        /*
		public void ComunicaSectorizacion()
		{
			ServiciosCD40.Sectorizaciones s = new ServiciosCD40.Sectorizaciones();

			s.IdSistema = (string)Session["idsistema"];
			s.Activa = true;

			ServiciosCD40.Tablas[] listaSectorizaciones = ServicioParaTransaccion.ListSelectSQL(s);
			if (listaSectorizaciones.Length > 0)
			{
				s.IdSectorizacion = ((ServiciosCD40.Sectorizaciones)listaSectorizaciones[0]).IdSectorizacion;
				DateTime fechaActiva = new DateTime();
				fechaActiva = DateTime.Now;
				ServicioParaTransaccion.ComunicaSectorizacionActiva(s.IdSistema, s.IdSectorizacion, ref fechaActiva);
			}
		}
        */

        private bool bSistemaConSactaHabilitadoPorCnf()
        {
            const string CONF_KEY_CON_SACTA = "SistemaConSACTA";
            bool bConSacta = false;

            //Se obtiene el parametro que indica si se debe o no visualizar la posici�n Sacta
            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement objConf = null;

            //Se obtiene el parametro que indica si se debe o no visualizar la posici�n Sacta
            objConf = config.AppSettings.Settings[CONF_KEY_CON_SACTA];

            if ((objConf != null) && (!string.IsNullOrEmpty(objConf.Value) && string.Compare(objConf.Value, "SI", true) == 0))
            {
                //El sistema est� configurado con el sistema SACTA
                bConSacta = true;
            }

            return bConSacta;
        }

        //20210317 #4749
        public bool SistemaConDependenciasATS()
        {
            const string CONF_KEY_CON_DEPATS = "DependenciasATS";
            bool bConDepATS = false;

            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement objConf = null;

            //Se obtiene el parametro que indica si se debe o no visualizar la gestion de dependencias ATS
            objConf = config.AppSettings.Settings[CONF_KEY_CON_DEPATS];

            if ((objConf != null) && (!string.IsNullOrEmpty(objConf.Value) && string.Compare(objConf.Value, "SI", true) == 0))
            {
                //El sistema est� configurado con gesti�n de dependencias ATS
                bConDepATS = true;
            }

            return bConDepATS;
        }

        // RQF-33
        public bool SistemaEnConfiguracionOffline()
        {
            const string CONF_KEY_CONFIGURACION_OFFLINE = "ConfiguracionOffline";
            bool bConfiguracionOffline = false;

            Configuration config = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement objConf = null;

            //Se obtiene el parametro que indica si se debe o no visualizar la gestion de dependencias ATS
            objConf = config.AppSettings.Settings[CONF_KEY_CONFIGURACION_OFFLINE];

            if ((objConf != null) && (!string.IsNullOrEmpty(objConf.Value) && string.Compare(objConf.Value, "SI", true) == 0))
            {
                //El sistema est� configurado en modo OFF LINE
                bConfiguracionOffline = true;
            }
            return bConfiguracionOffline;
        }



		#region ICallbackEventHandler Members
		// Respuesta a la regeneraci�n de las sectorizaciones
		private void OnCallBackCompleted(IAsyncResult result)
		{
			try
			{
				Session.Add("Sectorizando", false);
			}
			catch (System.Web.Services.Protocols.SoapException )
			{
				//Session.Add("Sectorizando", false);
			}
		}

		public string GetCallbackResult()
		{
			string sectorizando = "False";
            string versionActual = string.Empty;
            bool actualizar = false;

			if (Application["Sectorizando"] != null)
				sectorizando = ((bool)Application["Sectorizando"]).ToString();

			Sectorizando = sectorizando == "True";
			//TiempoPendienteTransaccion = ServicioParaTransaccion.TiempoPendienteDeTransaccion();		// -1= Transacci�n timeouted

            try
            {

            }
            catch (System.Web.Services.Protocols.SoapException )
            {

            }

			return string.Format("{0};{1};{2};{3};{4};{5}", sectorizando,
                                                bVisualizandoActiva,
												(string)GetGlobalResourceObject("Espaniol", "FinTransaccion"),
												EstadoEnlaceSacta, versionActual,
												actualizar);
		}

		public void RaiseCallbackEvent(string eventArgument)
		{
			_Command = eventArgument;
		}
		#endregion
	}
}
