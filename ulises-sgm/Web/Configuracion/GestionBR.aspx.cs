using System;
using System.IO;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Diagnostics;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Web.Configuration;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using log4net;
using log4net.Config;


public partial class GestionBR : PageBaseCD40.PageCD40
{
    /// 
    /// </summary>
    private Mensajes.msgBox cMsg;
    /// 
    /// </summary>
    private static ILog _logDebugView;
    /// 
    /// </summary>
    private static ServiciosCD40.ServiciosCD40 ServicioCD40 = new ServiciosCD40.ServiciosCD40();

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

    const uint INCIDENCIA_BACKUP = 204;
    const uint INCIDENCIA_RESTORE = 205;
    /// 
    /// </summary>
    private static bool selBackup;
    private static string dbAccesu;
    private static string dbAccesp;
    private static string pathBckRst;
    private static string sIdUsuario;
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
            sIdUsuario = ident.Name;
            if (perfil != "3")	// Sólo el perfil 3 (Técnico 3) tiene acceso a Gestión de Bacup/Restore.
            {
                Response.Redirect("~/Configuracion/Inicio.aspx?Permiso=NO", false);
                return;
            }
        }

        if (!IsPostBack)
        {
            //BtnAceptar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "AceptarCambios");
            BtnCancelar_ConfirmButtonExtender.ConfirmText = (string)GetGlobalResourceObject("Espaniol", "CancelarProceso");
            //Se lee la variable de sesión idsistema
            Configuration config = WebConfigurationManager.OpenWebConfiguration("~");
            KeyValueConfigurationElement s = config.AppSettings.Settings["Sistema"];
            Session["idsistema"] = s.Value;
            s = config.AppSettings.Settings["DBAccesU"];
            dbAccesu = s.Value;
            s = config.AppSettings.Settings["DBAccesP"];
            dbAccesp = s.Value;
            s = config.AppSettings.Settings["PathBckRst"];
            TBPATH.Text = pathBckRst = s.Value;
            TBPATH.Enabled = false;
            IndexListBox1 = -1;
            CargaDatosBR();
        }
        else
        {
            // Si se ha recargado la página y la variable de session tienen valor nulo es porque 
            // si ha cambiado la sesión del servidor, bien por conmutación o reinicio
            // por lo que se va a la página de login
            if (Session["idsistema"] == null)
            {
                ScriptManager.RegisterStartupScript(this, typeof(Page), "redirect", "<Script language = 'Javascript'> window.parent.location='../Login.aspx' ; </Script>", false);
                return;
            }
            MostrarElemento();
        }
    }


    private void CargaDatosBR()
    {
       try
       {
            LBIDBR.Items.Clear();
            var sqlFiles = Directory.EnumerateFiles(pathBckRst, "*.sql");
            if (sqlFiles != null)
            {
                    foreach (string currentFile in sqlFiles)
                    {
                        string fileName = currentFile.Substring(pathBckRst.Length + 1);
                       LBIDBR.Items.Add(fileName);
                    }
            }
            if (LBIDBR.Items.Count > 0)
            {
                if (LBIDBR.Items.FindByText(NewItem) != null)
                    LBIDBR.Items.FindByText(NewItem).Selected = true;
                else
                    LBIDBR.SelectedIndex = IndexListBox1 != -1 && IndexListBox1 < LBIDBR.Items.Count ? IndexListBox1 : 0;
                BtnEliminar.Visible = BtnRestore.Visible = true;
                MostrarElemento();
            }
            else
            {
                BtnEliminar.Visible = BtnRestore.Visible = false;
                TBIDBR.Text = TBIDBR.Text = string.Empty;
            }
        }
        catch (Exception e)
        {
            logDebugView.Error("(GestionBackupRestore-MuestraDatosBR):", e);
        }
    }

    private void MostrarElemento()
    {
        if (!selBackup)
        {
            if (LBIDBR.Items.Count > 0)
            {
                ActualizaWebPadre(true);
                TBIDBR.Text = LBIDBR.SelectedItem.Text;
            }
        }
    }


    protected void BtnAceptar_Click(object sender, EventArgs e)
    {
        if (selBackup)
        {
            if (TBIDBR.Text == string.Empty || !Regex.IsMatch(TBIDBR.Text, @"^[\w-.]*$"))
            {
                cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ErrorIdentificadorNoValido"));
                return;
            }
            RegistraProceso(INCIDENCIA_BACKUP, TBIDBR.Text);
            ProcesaBackup();
            TerminaActualizacion();
        }
        else
        {
            ProcesaRestore();
        }
    }


    protected void BtnBackup_Click(object sender, EventArgs e)
    {
        selBackup = true;
        Panel1.Enabled = true;
        BtnAceptar.Visible = BtnCancelar.Visible = true;
        BtnRestore.Visible = BtnEliminar.Visible = BtnBackup.Visible = BtnEliminar.Visible = false;
        LBIDBR.Enabled = false;
        TBIDBR.Enabled = true;
        ResetItems();
    }


    private void ResetItems()
    {
        if (selBackup)
            TBIDBR.Text = string.Empty;
    }


    protected void BtnRestore_Click(object sender, EventArgs e)
    {
        selBackup = false;
        Panel1.Enabled = true;
        BtnAceptar.Visible = BtnCancelar.Visible = true;
        BtnEliminar.Visible = BtnRestore.Visible = BtnBackup.Visible = false;
        LBIDBR.Enabled = false;
        TBIDBR.Enabled = false;
        IndexListBox1 = LBIDBR.SelectedIndex;
    }


    protected void BtnEliminar_Click(object sender, EventArgs e)
    {
        EliminaFicheroBackup();
    }


    private void EliminaFicheroBackup()
    {
        try
        {
            File.Delete(pathBckRst + "\\" + LBIDBR.SelectedItem.Text);
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ElementoEliminado"));
        }
        catch (Exception e)
        {
            logDebugView.Error("(GestionBackupRestore-EliminarFichero):", e);
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ElementoEliminadoError"));
        }
        TerminaActualizacion();
    }


    private void ProcesaBackup()
    {
        bool errorbackup = false;
        System.IO.DirectoryInfo DIR = new System.IO.DirectoryInfo(pathBckRst);
        if (!DIR.Exists)
            DIR.Create();
        string sFile = pathBckRst + "\\" + TBIDBR.Text;

        if (!sFile.Contains(".sql"))
            sFile += ".sql";

        System.IO.FileInfo fi = new System.IO.FileInfo(@sFile);
        if (fi.Exists)
            fi.Delete();

        string argumentos;
        argumentos = dbAccesu + " " + dbAccesp + " " + sFile;
        Process proceso_Backup;
        try
        {
            proceso_Backup = new Process();
            proceso_Backup.StartInfo.UseShellExecute = true;
            proceso_Backup.StartInfo.FileName = "backup_ulisesv5000.bat";
            proceso_Backup.StartInfo.WorkingDirectory = "c:\\inetpub\\wwwroot\\UlisesV5000\\U5kCfg\\Servicios";
            proceso_Backup.StartInfo.Arguments = argumentos;
            proceso_Backup.StartInfo.WindowStyle = ProcessWindowStyle.Minimized;
            proceso_Backup.Start();
            do
            {
                if (!proceso_Backup.HasExited)
                {
                    proceso_Backup.Refresh();
                }
            }
            while (!proceso_Backup.WaitForExit(1000));

            int retorno = proceso_Backup.ExitCode;
            if (retorno != 0)
            {
                // Proceso terminado con error.
                errorbackup = true;
            }
            else
            {
                // Proceso terminado
                // Obtiene si existe error interno de mysqldump
                if (BackupError(sFile))
                {
                    errorbackup = true;
                }
            }
        }
        catch (Exception)
        {
            errorbackup = true;
        }
        if (errorbackup)
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ErrorProcesoBackUp"));
        }
    }
    private void MuestraProgreso(double files)
    {
        //Label lblProgressBar = new label();
        double totalFiles = 0;
        double percentage = 0;
        for(int i =0; i < files; i++)
        {
        //corres el proceso
        
        //actualizas el label con una tabla html con 2 columnas
        //En este caso no lo estoy basando en el 100% sino en el 200%

        //sacas el %
        percentage = (files * 200) / totalFiles;

        //parece que cuando el porcentaje esta entre 0 y 1 el progressBar muestra el progreso como si fuera 50% (la mitad) y para solucionarlo, en el caso que el porcentaje este entre 0 y 1, se pone el valor como 1%
        if (percentage >= 0 && percentage < 1)
        {
        percentage = 1;
        }

        //Se actualiza el progressBar con el nuevo porcentaje
        Response.Write("<script language = javascript > document.getElementById('progreso').innerHTML='<table width=20px height=18px border=0 cellspacing=1><tr><td width="
        + percentage + " background=url('../Cluster/Images/progress.gif')></td><td></td></tr></table>' </script>");

        //Esta line muesta el porcentaje en otro label que esta posicionado sobre el progressBar
        Response.Write("<script language=javascript> document.getElementById('porciento').innerHTML = '" + Math.Round(((files * 200) / totalFiles) / 2) + "%'; </script>");

        //Con el metodo flush los datos viajan directamente desde el servidor hasta el cliente sin necesidad que se almacenen en el buffer (osea refrescara el progressBar)
        Response.Flush();
        }
    }
    private bool BackupError(string sFile)
    {
        bool errorbackup = false;
        // Obtiene resultado del proceso de backup
        string sFileError = sFile + ".error";
        foreach (var line in System.IO.File.ReadAllLines(sFileError))
        {
            if (line.Contains(("error")))
            {
                errorbackup = true;
                break;
            }
        }
        if (!errorbackup)
        {
            File.Delete(sFileError);
        }
        return errorbackup;
    }

    private void ProcesaRestore()
    {
        System.IO.DirectoryInfo DIR = new System.IO.DirectoryInfo(pathBckRst);
        if (!DIR.Exists)
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ErrorProcesoRestore"));
            return;
        }
        string sFile = pathBckRst + "\\" + TBIDBR.Text;

        System.IO.FileInfo fi = new System.IO.FileInfo(@sFile);
        if (!fi.Exists)
        {
            cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ErrorProcesoRestore"));
            return;
        }

        string argumentos;
        argumentos = dbAccesu + " " + dbAccesp + " " + sFile;
            try
            {
                Process proceso_Restore;
                proceso_Restore = new Process();
                proceso_Restore.StartInfo.UseShellExecute = true;
                proceso_Restore.StartInfo.FileName = "restore_ulisesv5000.bat";
                proceso_Restore.StartInfo.WorkingDirectory = "c:\\inetpub\\wwwroot\\UlisesV5000\\U5kCfg\\Servicios";
                proceso_Restore.StartInfo.Arguments = argumentos;
                proceso_Restore.StartInfo.WindowStyle = ProcessWindowStyle.Minimized;
                proceso_Restore.Start();
                do
                {
                    if (!proceso_Restore.HasExited)
                    {
                        proceso_Restore.Refresh();
                    }
                }
                while (!proceso_Restore.WaitForExit(1000));

                int retorno = proceso_Restore.ExitCode;
                if (retorno != 0)
                {
                    cMsg.alert((string)GetGlobalResourceObject("Espaniol", "ErrorProcesoRestore"));
                    return;
                }
                else
                {
                    RegistraProceso(INCIDENCIA_RESTORE, TBIDBR.Text);
                }
            }
            catch (Exception e)
            {
                logDebugView.Error("(GestionBackupRestore-ProcesaRestore):", e);
            }
            FormsAuthentication.Initialize();
            HttpContext context1 = HttpContext.Current;
            HttpCookie cookie1 = new HttpCookie(FormsAuthentication.FormsCookieName, "");
            cookie1.Path = FormsAuthentication.FormsCookiePath;
            cookie1.Expires = new DateTime(0x7cf, 10, 12);
            cookie1.Secure = FormsAuthentication.RequireSSL;
            context1.Response.Cookies.Remove(FormsAuthentication.FormsCookieName);
            context1.Response.Cookies.Add(cookie1);
            Response.Redirect("~/Login.aspx");
    }

    private void TerminaActualizacion()
    {
        LBIDBR.Enabled = true;
        LblErrorConfirmacion.Visible = false;
        LblMensajeConfirmacion.Visible = false;
        selBackup = false;
        Panel1.Enabled = false;
        BtnRestore.Visible = BtnEliminar.Visible = LBIDBR.Items.Count > 0;
        BtnBackup.Visible = true;
        BtnAceptar.Visible = BtnCancelar.Visible = false;
        CargaDatosBR();
    }


    protected void LBIDBR_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (LBIDBR.SelectedIndex >= 0)
        {
            BtnEliminar_ConfirmButtonExtender.ConfirmText = String.Format((string)GetGlobalResourceObject("Espaniol", "EliminarFicheroBackup"), LBIDBR.SelectedValue);
            MostrarElemento();
        }
    }


    protected void BtnCancelar_Click(object sender, EventArgs e)
    {
        CancelarOperacion();
        cMsg.confirm((string)GetGlobalResourceObject("Espaniol", "CancelarCambios"), "cancelaElemento");
    }


    private void CancelarOperacion()
    {
        if (selBackup)
        {
            if (LBIDBR.Items.Count > 0)
                LBIDBR.SelectedIndex = IndexListBox1 != -1 ? IndexListBox1 : 0;
            MostrarElemento();
        }
        TerminaActualizacion();
    }

    private void RegistraProceso(uint proceso, string fichero)
    {
        string[] arrAux = new string[1];
        string sTexto = string.Empty;
        string sIdSistema = (string)Session["idsistema"];
        if (proceso == INCIDENCIA_BACKUP)
        {
            sTexto = (string)GetGlobalResourceObject("Espaniol", "ProcesoBackup");
            if (null == sTexto)
            {
                sTexto = "Servidor activo. Backup Base de Datos . Identificador: {0} Usuario: {1}";
            }
        }
        else
        {
            sTexto = (string)GetGlobalResourceObject("Espaniol", "ProcesoRestore");

            if (null == sTexto)
            {
                sTexto = "Servidor activo. Restore Base de Datos . Identificador: {0} Usuario: {1}";
            }
        }
        sTexto = string.Format(sTexto, fichero, sIdUsuario);
        arrAux[0] = sTexto;
        ServicioCD40.GeneraIncidenciaConfiguracion(sIdSistema, proceso, arrAux);
    }

}