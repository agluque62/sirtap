using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using System.Threading;
using NAudio.FileFormats.Wav;
using NAudio.Wave;


namespace NucleoSim
{
    public partial class FrmNucleo : Form
    {
        private Boolean inicio = true;
        private string ficheroGRS = null;
        private string ficheroTER = null;
        private ArrayList recursosGRS = null;
        private ArrayList recursosTER = null;
        private RecordClient Cliente = null;
        private int TiempoDat = 0;
        private int TamPaquetes = 160;
        private UDPListener oidor = null;
        private Object Sync = new Object();
        private int Envios = 0;
        private int DireccionLlamada = 0;

        public FrmNucleo()
        {
            InitializeComponent();
        }

        private void FrmNucleo_Load(object sender, EventArgs e)
        {
            ficheroGRS = Properties.NucleoSim.Default.FicheroDatos;
            ficheroTER = Properties.NucleoSim.Default.FicheroDatos;
            recursosGRS = new ArrayList();
            recursosTER = new ArrayList();
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS1);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS2);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS3);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS4);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS5);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS6);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS7);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS8);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS9);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS10);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS11);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS12);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS13);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS14);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS15);
            recursosGRS.Add(Properties.NucleoSim.Default.RecursoGRS16);
            recursosTER.Add(Properties.NucleoSim.Default.RecursoTER1);
            TiempoDat = Properties.NucleoSim.Default.InterbaloDat;
            this.dudDatos.Text = TiempoDat.ToString();
            DireccionLlamada = Properties.NucleoSim.Default.DireccionLlamada;
            this.cboDireccion.SelectedIndex = DireccionLlamada - 1;
            this.cboPrioridad.SelectedIndex = 2;
            this.txtTelefono.Text = Properties.NucleoSim.Default.Telefono;
            this.txtTelefonoExt.Text = Properties.NucleoSim.Default.TelefonoExterno;
            this.txtTelefonoEntrante.Text = Properties.NucleoSim.Default.TelefonoEntrante;
            this.cboCausaDesc.SelectedIndex = 0;
            this.cboCausaDesc2.SelectedIndex = 0;
            this.txtRecursoEntrante.Text = Properties.NucleoSim.Default.RecursoEntrante;
            this.cboTipo.SelectedIndex = 0;
            this.txtLog.Text = "";
            this.cboServidor.Items.Clear();
            this.cboServidor.Items.Add( Properties.NucleoSim.Default.TextoIP1);
            this.cboServidor.Items.Add(Properties.NucleoSim.Default.TextoIP2);
            this.cboServidor.Items.Add(Properties.NucleoSim.Default.TextoIP3);
            this.cboServidor.SelectedIndex = 0;
            this.lstRecursos.SelectedIndices.Clear();
            this.chkEnvioInd.Checked = false;
            TamPaquetes = Properties.NucleoSim.Default.TamPaquetes;
            this.picEstado.Image = this.imlEstado.Images[2];
            Control.CheckForIllegalCrossThreadCalls = false;
            this.oidor = new UDPListener(Respuesta);
            this.oidor.Iniciar(Properties.NucleoSim.Default.ResponsePort);
        }

        protected void Respuesta(string texto)
        {
            int i = texto.LastIndexOf(",");
            if (i > 0)
            {
                texto = texto.Substring(i + 1, texto.Length - i -1);
            }
            this.txtRespuesta.Text = texto;
        }

        protected void Log(string linea)
        {
            StringBuilder Texto = new StringBuilder(this.txtLog.Text);
            Texto = Texto.AppendLine(linea);
            string Contenido = Texto.ToString();
            while (Contenido.Length > this.txtLog.MaxLength)
            {
                Contenido = Contenido.Substring(Contenido.IndexOf('\n') + 1);
            }
            this.txtLog.Text = Contenido;
        }

        private void cboTipo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.cboTipo.SelectedIndex == 0)
            {
                this.btnSQHON.Text = "SQU ON";
                this.btnSQHOFF.Text = "SQU OFF";
                this.btnSTON.Enabled = true;
                this.btnSTOFF.Enabled = true;
                this.pnlTelefono.Enabled = false; 
                if (!inicio)
                {
                    ficheroTER = this.txtFichero.Text;
                    recursosTER.Clear();
                    recursosTER.AddRange(this.lstRecursos.Items);
                }
                this.txtFichero.Text = ficheroGRS;
                this.lstRecursos.Items.Clear();
                this.lstRecursos.Items.AddRange(recursosGRS.ToArray());
            }
            else
            {
                this.btnSQHON.Text = "PTT ON";
                this.btnSQHOFF.Text = "PTT OFF";
                this.btnSTON.Enabled = false;
                this.btnSTOFF.Enabled = false;
                this.pnlTelefono.Enabled = true;
                if (!inicio)
                {
                    ficheroGRS = this.txtFichero.Text;
                    recursosGRS.Clear();
                    recursosGRS.AddRange(this.lstRecursos.Items);
                }
                this.txtFichero.Text = ficheroTER;
                this.lstRecursos.Items.Clear();
                this.lstRecursos.Items.AddRange(recursosTER.ToArray());
            }
            inicio = false;
        }

        private void btnNuevo_Click(object sender, EventArgs e)
        {
            string valor = "";
            Dialogos.InputBox("Recurso", "Introduzca nombre:", ref valor);
            lstRecursos.Items.Add(valor);
        }

        private void btnMenos_Click(object sender, EventArgs e)
        {
            if (this.lstRecursos.SelectedIndex >= 0)
            {
                this.lstRecursos.Items.RemoveAt(this.lstRecursos.SelectedIndex);
            }
        }

        private void btnSelFichero_Click(object sender, EventArgs e)
        {
            this.ofdlgFichero.Title = "Seleccione fichero de datos";
            this.ofdlgFichero.Filter = "WAV files|*.wav";
            this.ofdlgFichero.InitialDirectory = Path.GetDirectoryName(this.txtFichero.Text);
            this.ofdlgFichero.FileName = this.txtFichero.Text;
            if (this.ofdlgFichero.ShowDialog() == DialogResult.OK)
            {
                this.txtFichero.Text = this.ofdlgFichero.FileName;
            }        
        }

        private void btnSesionON_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                if (this.cboTipo.SelectedIndex == 0)
                {
                    Cliente.GrsStartSession(nrec);
                }
                else
                {
                    Cliente.TerStartSession(nrec);
                }
            }
        }

        private void btnSesionOFF_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                if (this.cboTipo.SelectedIndex == 0)
                {
                    Cliente.GrsEndSession(nrec);
                }
                else
                {
                    Cliente.TerEndSession(nrec);
                }
            }
        }

        private void btnSQUON_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                if (this.cboTipo.SelectedIndex == 0)
                {
                    Cliente.GrsSquelchOn(nrec);
                }
                else
                {
                    Cliente.TerPttOn(nrec);
                }
            }
            /*
            //ESC. 22.01.2016. El comando de PTT/SQU a ON no debe enviar el fichero de audio.
            if (!this.chkEnvioInd.Checked)
            {
                Thread.Sleep(TiempoDat);
                IniciarEnvioFichero();
            }
            */
        }

        private void btnSQUOFF_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                if (this.cboTipo.SelectedIndex == 0)
                {
                    Cliente.GrsSquelchOff(nrec);
                }
                else
                {
                    Cliente.TerPttOff(nrec);
                }
            }
        }

        private void btnSTON_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                Cliente.GrsSTOn(nrec);
            }
        }

        private void btnSTOFF_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                Cliente.GrsSTOff(nrec);
            }
        }

        private void btnSesion_Click(object sender, EventArgs e)
        {
            IniciarEnvioFichero();
        }

        private void IniciarEnvioFichero()
        {
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                Thread workerThread = new Thread(new ParameterizedThreadStart(EnviarFichero));
                workerThread.Start(nrec);
                this.Refresh();
            }
            tmrEnvio.Enabled = true;
        }

        private void EnviarFichero(object param)
        {
            string nrec = (string)param;
            lock (Sync)
            {
                Envios = Envios + 1;
                ActualizarEstado();
            }

            int veces = Convert.ToInt32(this.dudVeces.Text);
            while (veces > 0)
            {
                try
                {
                    var myReader = new WaveFileReader(this.txtFichero.Text);
                    if (myReader.WaveFormat.SampleRate != 8000 || myReader.WaveFormat.BitsPerSample != 8 || myReader.WaveFormat.Encoding != WaveFormatEncoding.ALaw)
                        throw new Exception("El formato del fichero debe ser ALAW,8,8000: ["+ myReader.WaveFormat.Encoding+ "," +myReader.WaveFormat.BitsPerSample+ "," + myReader.WaveFormat.SampleRate+"]");

                    Log("Enviando Fichero " + this.txtFichero.Text);
                    int paquete = 0;
                    int bytesRead = 0;
                    byte[] readChunck = new byte[TamPaquetes];
                    bool fin = false;
                    while (!fin)
                    {
                        //Read 160 bytes at a time, will return 0 when there are no more bytes to Read
                        bytesRead = myReader.Read(readChunck, 0, readChunck.Length);
                        if (bytesRead != 0)
                        {
                            //Process the bytes here
                            paquete = paquete + 1;
                            Cliente.SendMedia(nrec, paquete, readChunck, bytesRead);

                            Log("Enviado paquete de datos " + paquete + " (BYTES=" + bytesRead + ")");
                            Thread.Sleep(TiempoDat);
                        }
                        else
                        {
                            fin = true;
                        }
                    }
                }
                catch (Exception x)
                {
                    Log("Excepcion: " + x.Message);
                }
                Thread.Sleep(10 * TiempoDat);
                veces = veces - 1;
            }
            lock (Sync)
            {
                Envios = Envios - 1;
                ActualizarEstado();
            }
        }

        private void ActualizarEstado()
        {
            if (Envios == 0 )
            {
                this.picEstado.Image = this.imlEstado.Images[2];
            }
            else if (Envios == 1)
            {
                this.picEstado.Image = this.imlEstado.Images[1];
            }
            else
            {
                this.picEstado.Image = this.imlEstado.Images[0];
            }
            Thread.Sleep(50);
        }

        private void EnviarFichero2()
        {
            try
            {
                var myReader = new WaveFileReader(this.txtFichero.Text);
                if (myReader.WaveFormat.SampleRate != 8000 || myReader.WaveFormat.BitsPerSample != 8 || myReader.WaveFormat.Encoding != WaveFormatEncoding.ALaw)
                    throw new Exception("El formato del fichero debe ser ALAW,8,8000: [" + myReader.WaveFormat.Encoding + "," + myReader.WaveFormat.BitsPerSample + "," + myReader.WaveFormat.SampleRate + "]");

                Log("Enviando Fichero " + this.txtFichero.Text);
                int paquete = 0;
                int bytesRead = 0;
                byte[] readChunck = new byte[TamPaquetes];
                bool fin = false;
                while (!fin)
                {
                    //Read 160 bytes at a time, will return 0 when there are no more bytes to Read
                    bytesRead = myReader.Read(readChunck, 0, readChunck.Length);
                    if (bytesRead != 0)
                    {
                        //Process the bytes here
                        paquete = paquete + 1;
                        foreach (string nrec in this.lstRecursos.SelectedItems)
                        {
                            Cliente.SendMedia(nrec, paquete, readChunck, bytesRead);
                            Log("Enviado paquete de datos " + paquete + " (BYTES=" + bytesRead + ")");
                        }
                    }
                    else
                    {
                        fin = true;
                    }
                }
            }
            catch (Exception x)
            {
                Log("Excepcion: " + x.Message);
            }
         }

        private void dudDatos_SelectedItemChanged(object sender, EventArgs e)
        {
            if (Convert.ToInt32(this.dudDatos.Text) < 0)
            {
                this.dudDatos.Text = "0";
            }
            TiempoDat = Convert.ToInt32(this.dudDatos.Text);
        }

        private void txtLog_TextChanged(object sender, EventArgs e)
        {
            this.txtLog.SelectionStart = this.txtLog.Text.Length;
            this.txtLog.ScrollToCaret();
            this.txtLog.DeselectAll();
        }

        private void cboServidor_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            Cliente = new RecordClient(this.Log, this.cboServidor.SelectedIndex + 1);
            this.txtIP.Text = Cliente.IPServicio.ToString();
            this.txtPuerto.Text = Cliente.PuertoServicio.ToString();
            Thread.Sleep(200);
            Cliente.CtlSetParams(this.cboServidor.SelectedIndex + 1);
        }

        private void btnReconfigurar_Click(object sender, EventArgs e)
        {
            Cliente.CtlConfig();
        }

        private void btnFin_Click(object sender, EventArgs e)
        {
            Cliente.CtlEnd();
        }

        private void btnTest_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 5; i = i + 1)
            {
                foreach (string nrec in this.lstRecursos.SelectedItems)
                {
                    if (this.cboTipo.SelectedIndex == 0)
                    {
                        Cliente.GrsStartSession(nrec);
                    }
                    else
                    {
                        Cliente.TerStartSession(nrec);
                    }
                }
                for (int j = 0; j < 5; j = j + 1)
                {
                    foreach (string nrec in this.lstRecursos.SelectedItems)
                    {
                        if (this.cboTipo.SelectedIndex == 0)
                        {
                            Cliente.GrsSquelchOn(nrec);
                        }
                        else
                        {
                            Cliente.TerPttOn(nrec);
                        }
                    }
                    EnviarFichero2 ();
                    Thread.Sleep(100);
                    foreach (string nrec in this.lstRecursos.SelectedItems)
                    {
                        if (this.cboTipo.SelectedIndex == 0)
                        {
                            Cliente.GrsSquelchOff(nrec);
                        }
                        else
                        {
                            Cliente.TerPttOff(nrec);
                        }
                    }
                }
                foreach (string nrec in this.lstRecursos.SelectedItems)
                {
                    if (this.cboTipo.SelectedIndex == 0)
                    {
                        Cliente.GrsEndSession(nrec);
                    }
                    else
                    {
                        Cliente.TerEndSession(nrec);
                    }
                }
                Thread.Sleep(100);
            }
        }

        private void chkEnvioInd_CheckedChanged(object sender, EventArgs e)
        {
            if (this.chkEnvioInd.Checked)
            {
                this.btnSesion.Enabled = true;
                this.dudVeces.Enabled = true;
                this.dudDatos.Enabled = true;
                this.label4.Enabled = true;
            }
            else
            {
                this.btnSesion.Enabled = false;
                this.dudVeces.Enabled = false;
                this.dudDatos.Enabled = false;
                this.label4.Enabled = false;
                TiempoDat = Properties.NucleoSim.Default.InterbaloDat;
                this.dudDatos.Text = TiempoDat.ToString();
                this.dudVeces.Text = "1";
            }
        }

        private void btnCallStart_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                int prioridad = this.cboPrioridad.SelectedIndex+1;
                Cliente.TerCallStart(nrec, DireccionLlamada.ToString(), prioridad.ToString(), this.txtTelefono.Text, this.txtTelefonoExt.Text);
            }
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                Cliente.TerCallConnect(nrec, this.txtTelefonoExt.Text);
            }
        }

        private void btnCallEnd_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                string causa = this.cboCausaDesc.SelectedItem.ToString().Substring(0, 2);
                Cliente.TerCallEnd(nrec, causa, "1");
            }
        }

        private void btnTransferStart_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                Cliente.TerCallTransferStart(nrec, this.txtRecursoEntrante.Text, this.txtTelefonoEntrante.Text, this.txtTelefono.Text);
            }
        }

        private void btnTransferConnect_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                Cliente.TerCallTransferConn(nrec, this.txtRecursoEntrante.Text, this.txtTelefonoEntrante.Text);
            }
        }

        private void btnTransferEnd_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                string causa = this.cboCausaDesc2.SelectedItem.ToString().Substring(0, 2);
                Cliente.TerCallTransferDisc(nrec, this.txtRecursoEntrante.Text, causa, "1");
            }
        }

        private void cboDireccion_SelectedIndexChanged(object sender, EventArgs e)
        {
            DireccionLlamada = this.cboDireccion.SelectedIndex + 1;
        }

        private void btnTodos_Click(object sender, EventArgs e)
        {
            if (this.lstRecursos.SelectedIndices.Count == 0)
            {
                for (int i = 0; i < this.lstRecursos.Items.Count; i = i + 1)
                {
                    this.lstRecursos.SelectedIndices.Add(i);
                }
            }
            else
            {
                this.lstRecursos.SelectedIndices.Clear();
            }
        }

        private void btnHOLDON_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                Cliente.TerCallHoldOn(nrec);
            }
        }

        private void btnHOLDOFF_Click(object sender, EventArgs e)
        {
            this.txtRespuesta.Text = "";
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                Cliente.TerCallHoldOff(nrec);
            }
        }

        private void tmrEnvio_Tick(object sender, EventArgs e)
        {
            lock (Sync)
            {
                try
                {
                    Application.DoEvents();
                }
                catch
                {
                }
                if (Envios == 0)
                {
                    tmrEnvio.Enabled = false;
                }
            }
        }
//ESC. 22.01.2016. Añadido
        private void btnRECord_Click(object sender, EventArgs e)
        {
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                Cliente.Record(nrec);
            }
            
            if (!this.chkEnvioInd.Checked)
            {
                Thread.Sleep(TiempoDat);
                IniciarEnvioFichero();
            }
        }

        private void btnPAUse_Click(object sender, EventArgs e)
        {
            foreach (string nrec in this.lstRecursos.SelectedItems)
            {
                Cliente.Pause(nrec);
            }
        }
//--ESC.
    }
}
