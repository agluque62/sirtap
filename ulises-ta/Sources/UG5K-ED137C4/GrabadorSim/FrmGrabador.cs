using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections;



namespace GrabadorSim
{
    public partial class FrmGrabador : Form
    {
        private class Sampler
        {
            public string Nombre { get; set; }
            public string Estado { get; set; }
 
            public Sampler(string nombre, string estado)
            {
                Nombre = nombre;
                Estado = estado;
             }
        }

        private int RTPPort = -1;
        private int RTSPPort = -1;
        private IPAddress ServerIP = null;
        private IPAddress ServerIPLocal = null;
        private IPAddress ServerIPREDAN = null;
        private IPAddress IP = null;
        private Boolean Parar = false;
        private Boolean Funcionando = false;
        private int Sesiones = 0;
        private int SesionesTot = 0;
        private Thread servidor = null;
        private TcpListener listener = null;
        private int RetardoSETUP = 0;
        private int RetardoANNOUNCE = 0;
        private int RetardoTEARDOWN = 0;
        private int RetardoPeticion = 0;
        private int RetardoMedia = 0;
        private string DirFicheros = null;
        private ArrayList Samplers = null;
        private List<Sampler> EstadoSamplers = null;
        private bool LogActivado = true;

        public FrmGrabador()
        {
            InitializeComponent();
        }

        private void FrmGrabador_Load(object sender, EventArgs e)
        {
            RTSPPort = Properties.Grabador.Default.RTSPPort;
            RTPPort = Properties.Grabador.Default.RTPPort;
            ServerIP = IPAddress.Parse(Properties.Grabador.Default.IP);
            ServerIPLocal = IPAddress.Parse(Properties.Grabador.Default.IPLocal);
            ServerIPREDAN = IPAddress.Parse(Properties.Grabador.Default.IPRedan);
            this.Text = "Grabador Simulado - <Desconectado>";
            this.txtIP.Text = "";
            this.txtRTSP.Text = RTSPPort.ToString();
            this.txtRTP.Text = RTPPort.ToString();
            this.btnIniciar.Enabled = true;
            this.btnIniciarLocal.Enabled = true;
            this.btnParar.Enabled = false;
            this.picEstado.Image = imlEstado.Images[0];
            this.tmrControl.Start();
            RetardoSETUP = Properties.Grabador.Default.RetardoSetup;
            RetardoANNOUNCE = Properties.Grabador.Default.RetardoAnnounce;
            RetardoTEARDOWN = Properties.Grabador.Default.RetardoTearDown;
            RetardoPeticion = Properties.Grabador.Default.RetardoPeticion;
            RetardoMedia = Properties.Grabador.Default.RetardoMedia;
            DirFicheros= Properties.Grabador.Default.DirectorioFicheros;
            Samplers = new ArrayList();
            EstadoSamplers = new List<Sampler>();
            string Linea;
            StreamReader Fichero = new StreamReader("Samplers.txt");
            while ((Linea = Fichero.ReadLine()) != null)
            {
                Samplers.Add(Linea);
                EstadoSamplers.Add(new Sampler(Linea, ""));
            }
            Fichero.Close();
            BindingSource source = new BindingSource();
            source.DataSource = EstadoSamplers;
            this.dgvSamplers.AutoGenerateColumns = true;
            this.dgvSamplers.DataSource = source;
            this.dgvSamplers.Columns[0].Width = this.dgvSamplers.Size.Width / 4;
            this.dgvSamplers.Columns[1].Width = this.dgvSamplers.Size.Width - this.dgvSamplers.Columns[0].Width;
            Thread.CurrentThread.Priority = ThreadPriority.AboveNormal;
        }

        private string GenerarIdSesion()
        {
            string id = System.Guid.NewGuid().ToString();
            return (id);
        }

        private void btnIniciar_Click(object sender, EventArgs e)
        {
            IniciarServidor(ServerIP);
        }

        private void btnIniciarLocal_Click(object sender, EventArgs e)
        {
            IniciarServidor(ServerIPLocal);
        }

        private void IniciarServidor(IPAddress direccionIP )
        {
            this.picEstado.Image = imlEstado.Images[1];
            Parar = false;
            this.btnIniciar.Enabled = false;
            this.btnIniciarLocal.Enabled = false;
            this.btnIniciarRedan.Enabled = false;
            this.btnParar.Enabled = true;
            Sesiones = 0;
            SesionesTot = 0;
            IP = direccionIP;
            this.txtIP.Text = IP.ToString();
            this.Text = "Grabador Simulado - " + IP + ":" + RTSPPort;

            servidor = new Thread(Servidor) { IsBackground = true };
            servidor.Start();
        }

        delegate void SetTextCallback(string Texto);
        private void SetText(string Texto)
        {
            if (this.txtLog.InvokeRequired)
            {
                try
                {
                    SetTextCallback d = new SetTextCallback(SetText);
                    this.Invoke(d, new object[] { Texto });
                }
                catch (Exception e)
                {
                    Log(e.Message);
                }
            }
            else
            {
                this.txtLog.Text = Texto;
            }
        }

        private void AppendText(string Texto)
        {
            if (this.txtLog.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(AppendText);
                this.Invoke(d, new object[] { Texto });
            }
            else
            {
                this.txtLog.AppendText ( Texto );
            }
        }

        private void Log(string linea)
        {
            if ( !LogActivado ) 
            {
                return;
            }
            StringBuilder Texto = new StringBuilder(this.txtLog.Text);
            Texto = Texto.AppendLine(linea);
            string Contenido = Texto.ToString();
            while (Contenido.Length > this.txtLog.MaxLength)
            {
                Contenido = Contenido.Substring(Contenido.IndexOf('\n') + 1);
            }
            SetText(Contenido);
        }

        private void Servidor()
        {
            Funcionando = true;
            listener = new TcpListener(IP, RTSPPort);
            try
            {
                listener.Start();
                Log("Arrancado Servidor RTSP");
            }
            catch (Exception ex)
            {
                Log(ex.Message);
                Parar = true;
            }
            while (!Parar)
            {
                try
                {
                    TcpClient client = listener.AcceptTcpClient();
                     client.NoDelay = true;
                    client.ReceiveBufferSize = 1024;

                    Log("Acceptada conexion en " + client.Client.RemoteEndPoint + "\r\n");

                    SesionesTot = SesionesTot + 1;
                    Thread sesion = new Thread(() => Sesion(client)) { IsBackground = true };
                    sesion.Start();
                }
                catch (ThreadAbortException)
                {
                    listener.Stop();
                    Parar = true;
                    break;
                }
                catch (Exception ex)
                {
                    if (!Parar)
                    {
                        Log(ex.Message);
                    }
                }
            }
            Funcionando = false;
            Log("Parado Servidor RTSP");
        }

        private string ByteArrayToString(byte[] Datos)
        {
            return ( BitConverter.ToString(Datos).Replace('-', ' ' ) );
        }

        private void Sesion(TcpClient client)
        {
            Sesiones = Sesiones + 1;
            byte[] buffer = new byte[1024];
            byte[] message = new byte[1024];        
            NetworkStream stream = client.GetStream();
            int bytesRead  = 0;
            int paquetes = 0;
            int tampaquete;
            //int bytesrecibidos = 0;
            string remote = client.Client.RemoteEndPoint.ToString();
            string sampler = "";
            int Caracter = 0;
            char[] Primeros = {'A','S','T','R','P'};

            Log("Iniciada session para " + remote + "(ThreadId=" + Thread.CurrentThread.ManagedThreadId + ")");
            while (!Parar)
            {
                try
                {
                    bytesRead = stream.Read(buffer, 0, 1024);
                    if (bytesRead > 0)
                    {
                        string peticion = ASCIIEncoding.ASCII.GetString(buffer, 0, bytesRead);
                        while (peticion.Length > 0 )
                        {
                            if ( peticion.StartsWith("$\0" ) )
                            {
                                //byte byte1 = (byte)peticion[2];
                                //byte byte2 = (byte)peticion[3];
                                //tampaquete = byte1 * 256 + byte2 + 4;
                                // if (tampaquete <= 176)
                                //{
                                if (sampler.Length == 0)
                                {
                                    Log("\t\tRecibido paquete de datos en estado incorrecto");
                                }
                                else
                                {
                                    paquetes = paquetes + 1;
                                    //bytesrecibidos = bytesrecibidos + tampaquete;
                                    //Log("\t\tRecibido paquete de datos " + paquetes + " de " + remote + " (ThreadId=" + Thread.CurrentThread.ManagedThreadId + ")");
                                    //Log("Binario=" + ByteArrayToString(message) + "\r\n");

                                    if (paquetes % 4 == 0)
                                    {
                                        Sampler Estado = BuscarSampler(sampler);
                                        string Txt = Estado.Estado + (Caracter == 0 ? "*" : "|");
                                        Caracter = (Caracter + 1) % 2;
                                        if (Txt.Length < 35)
                                        {
                                            Estado.Estado = Txt;
                                        }
                                        else
                                        {
                                            Estado.Estado = Txt.Substring(Txt.Length - 35);
                                        }
                                    }
                                }
                                //if (peticion.Length <= tampaquete)
                                //{
                                //peticion = "";
                                //}
                                //else
                                //{
                                //    peticion = peticion.Substring(tampaquete);
                                // }
                                //}
                                // else
                                //{
                                int sig = peticion.IndexOfAny(Primeros, 2);
                                if (sig<0)
                                {
                                     peticion = "";
                                }
                                else
                                {
                                    peticion = peticion.Substring(sig);
                                }
                                //}
                            }
                            else if (peticion.StartsWith("ANN") || peticion.StartsWith("SET") || peticion.StartsWith("REC") || peticion.StartsWith("PAU") ||
                                     peticion.StartsWith("TEA") || peticion.StartsWith("OPT"))
                            {                                
                                Log("Recibida Peticion de " + remote + " (BYTES=" + bytesRead + ") :");
                                Log("[\r\n" + peticion + "\r\n]");
                                bool reiniciar = false;
                                tampaquete = 0;
                                byte[] respuesta = ProcesarPeticion(peticion, ref sampler, ref reiniciar, ref tampaquete);
                                if (respuesta == null)
                                {
                                    respuesta = GenerarRespuesta(451, 0);
                                }
                                stream.Write(respuesta, 0, respuesta.Length);
                                Log("Respuesta a " + remote + ":");
                                Log("[\r\n" + ASCIIEncoding.ASCII.GetString(respuesta, 0, respuesta.Length) + "\r\n]");
                                //Log("Binario=" + ByteArrayToString(respuesta) + "\r\n");
                                if (reiniciar)
                                {
                                    paquetes = 0;
                                    //bytesrecibidos = 0;
                                }
                                //if (peticion.Length <= tampaquete)
                                //{
                                peticion = "";
                                //}
                                //else
                                //{
                                //    peticion = peticion.Substring(tampaquete);
                                //}
                            }
                            else
                            {
                                int sig = peticion.IndexOfAny(Primeros, 1);
                                if (sig < 0)
                                {
                                    peticion = "";
                                }
                                else
                                {
                                    peticion = peticion.Substring(sig);
                                }
                            }
                            Application.DoEvents();
                            Thread.Sleep(RetardoMedia);
                        }
                    }
                }
                catch (ThreadAbortException)
                {
                    break;
                }
                catch (Exception ex)
                {
                    Log(ex.StackTrace);
                    Parar = true;
                    listener.Stop();
                }
            }

            client.Close();
            Sesiones = Sesiones - 1;
            Log("Finalizada Session para " + remote);
        }

        private Sampler BuscarSampler(string nombre)
        {
            Sampler res = null;
            foreach (Sampler obj in EstadoSamplers)
            {
                if (obj.Nombre.Equals(nombre))
                {
                    res = obj;
                    break;
                }
            }
            return (res);
        }

        private byte[] ProcesarPeticion(string peticion, ref string Sampler, ref bool reiniciarcont, ref int tampaquete)
        {
            string Comando = null;
            int posFCom = peticion.IndexOf(" ", 0);
            if (posFCom < 0)
            {
                posFCom = peticion.IndexOf("\r\n", 0);
            }
            if (posFCom < 0)
            {
                posFCom = peticion.Length;
            }
            if (posFCom >= 0)
            {
                Comando = peticion.Substring(0, posFCom);
                Log("Recibido Comando=" + Comando);
            }
            if (Comando.Equals("OPTIONS"))
            {
                tampaquete = posFCom;
                //Thread.Sleep(RetardoPeticion);
                return GenerarRespuestaOptions();
            }
            if (Comando.Equals("RECORD"))
            {
                reiniciarcont = true;
            }
            string URL = null;
            int posURL = peticion.IndexOf("rtsp:");
            if (posURL >= 0)
            {
                int posFURL = peticion.IndexOf(" ", posURL);
                if (posFURL < 0)
                {
                    posFURL = peticion.Length;
                }
                URL = peticion.Substring(posURL, posFURL - posURL);
                Log("Recibido URL=" + URL);
                int posSam = URL.IndexOf("iprecorder/", 8);             //ESC. 18.01.2016. Antes del cambio URL "/"
                if (posSam >= 0)
                {
                    //Sampler = URL.Substring(posSam + 11, URL.Length - posSam - 2);    //Ginés
                    Sampler = URL.Substring(posSam + 11, URL.Length - posSam -11);      //ESC. 18.01.2016
                }
                Log("Recibido Sampler=" + Sampler);
            }
            string SesionId = "";
            int posSID = peticion.LastIndexOf("Session: ");
            int posFSID = 0;
            if (posSID >= 0)
            {
                posFSID = peticion.IndexOf("\r\n", posSID);
                if (posFSID < 0)
                {
                    posFSID = peticion.Length;
                }
                SesionId = peticion.Substring(posSID + 9, posFSID - (posSID + 9));
                Log("Recibido SID=" + SesionId);
            }
            string Canal = "0";
            int posCanal = peticion.IndexOf("interleaved=");
            if (posCanal >= 0)
            {
                Canal = peticion.Substring(posCanal + 12, 1);
                Log("Recibido CANAL=" + Canal);
            }
            int posSeq = peticion.LastIndexOf("CSeq: ");
            if (posSeq >= 0)
            {
                int posFSeq = peticion.IndexOf("\r\n", posSeq);
                int numberSeq = Int32.Parse(peticion.Substring(posSeq + 6, posFSeq - (posSeq + 6)));

                if (Samplers.IndexOf(Sampler)<0 )
                {
                    //Thread.Sleep(RetardoPeticion);
                    return GenerarRespuesta(452, numberSeq);
                }
                Sampler Estado = BuscarSampler(Sampler);
                if (Estado != null)
                {
                    string Txt = Estado.Estado + "<" + Comando.Substring(0, 3) + ">";
                    if (Txt.Length < 35)
                    {
                        Estado.Estado = Txt;
                    }
                    else
                    {
                        Estado.Estado = Txt.Substring(Txt.Length - 35);
                    }
                }
                if (Comando.Equals("ANNOUNCE"))
                {
                    //Thread.Sleep(RetardoANNOUNCE);
                    tampaquete = peticion.IndexOf("PCMA/8000\r\n") + 11;
                    return GenerarRespuesta(200, numberSeq);
                }
                else if (Comando.Equals("SETUP"))
                {
                    tampaquete = peticion.IndexOf("interleaved=") + 13;
                    //Thread.Sleep(RetardoSETUP);
                    SesionId = GenerarIdSesion();
                    Log("Abierta sesion SID=" + SesionId);
                    return GenerarRespuesta(200, numberSeq, SesionId, "RTP/AVP/TCP;interleaved=" + Canal);
                    // CreateRTPSocket();                    
                }
                else
                {
                    tampaquete = peticion.IndexOf("</call-record-data>") + 21;
                    if (SesionId.Length != 36)
                    {
                        //Thread.Sleep(RetardoPeticion);
                        return GenerarRespuesta(454, numberSeq);
                    }

                    if (Comando.Equals("SET_PARAMETER"))
                    {
                        //Thread.Sleep(RetardoPeticion);
                        return GenerarRespuesta(200, numberSeq, SesionId);
                    }
                    else if (Comando.Equals("RECORD"))
                    {
                        //Thread.Sleep(RetardoPeticion);
                        return GenerarRespuesta(200, numberSeq, SesionId);
                    }
                    else if (Comando.Equals("PAUSE"))
                    {
                        //Thread.Sleep(RetardoPeticion);
                        return GenerarRespuesta(200, numberSeq, SesionId);
                    }
                    else if (Comando.Equals("TEARDOWN"))
                    {
                        tampaquete = posFSID+2;
                        //Thread.Sleep(RetardoTEARDOWN);
                        return GenerarRespuesta(200, numberSeq, SesionId);
                    }
                    else
                    {
                        //Thread.Sleep(RetardoPeticion);
                        return GenerarRespuesta(405, numberSeq);
                    }
                }
            }
            else
            {
                //Thread.Sleep(RetardoPeticion);
                return GenerarRespuesta(451, 0);           
            }
        }

        private string TextoRespuesta (int codigo )
        {
            if ( codigo==200)
            {
                return ("OK");
            }
            else if ( codigo==250)
            {
                return ("Low on Storage Space->Server return this after receiving a Record request");
            }
            else if ( codigo==405)
            {
                return ("Method Not Allowed");
            }
            else if ( codigo==451)
            {
                return ("Parameter Not Understood");
            }
            else if ( codigo==452)
            {
                return ("Conference Not Found");
            }
            else if ( codigo==453)
            {
                return ("Not Enough Bandwidth");
            }
            else if ( codigo==454)
            {
                return ("Session Not Found");
            }
            else if ( codigo==455)
            {
                return ("Method Not Valid in This State");
            }
            else if ( codigo==456)
            {
                return ("Header Field Not Valid for Resource");
            }
            else if ( codigo==457)
            {
                return ("Invalid Range");
            }
            else if ( codigo==458)
            {
                return ("Parameter Is Read-Only");
            }
            else if ( codigo==459)
            {
                return ("Aggregate Operation Not Allowed");
            }
            else if ( codigo==460)
            {
                return ("Only Aggregate Operation Allowed");
            }
            else if ( codigo==461)
            {
                return ("Unsupported Transport->The Transport field did not contain a supported transport specification");
            }
            else if ( codigo==462)
            {
                return ("Destination Unreachable->The data transmission channel could not be established");
            }
            else if ( codigo==551)
            {
                return ("Option not supported->Option given in the Require or the Proxy-Require was not supported");
            }
            return ("");
        }

        private byte[] GenerarRespuesta(int codigo, int seq, string sesion = "", string transport="")
        {
            StringBuilder str = new StringBuilder("RTSP/1.0 " );
            str.Append ( codigo );
            str.Append ( " " );
            str.Append(TextoRespuesta(codigo));
            str.Append ( "\r\n");
            str.Append ("CSeq: " );
            str.Append (seq.ToString() );
            str.Append("\r\n");
            str.Append("Content-Length: 0\r\n");
            if (sesion.Length > 0)
            {
                str.Append("Session: ");
                str.Append(sesion);
                str.Append("\r\n");
            }
            if (transport.Length > 0)
            {
                str.Append("Transport: ");
                str.Append(transport);
                str.Append("\r\n");
            }
            str.Append("\r\n");

            ASCIIEncoding asen = new ASCIIEncoding();
            return asen.GetBytes(str.ToString());
        }

        private byte[] GenerarRespuestaOptions()
        {
            StringBuilder str = new StringBuilder("RTSP/1.0 200 OK\r\n");
            str.Append("Public: OPTIONS,ANNOUNCE,SETUP,TEARDOWN,PAUSE,RECORD,GET_PARAMETER,SET_PARAMETER\r\n");
            str.Append ("\r\n");

            ASCIIEncoding asen = new ASCIIEncoding();
            return asen.GetBytes(str.ToString());
        }
 
        private void btnParar_Click(object sender, EventArgs e)
        {
            Parar = true;
            listener.Stop();
            //servidor.Abort();
        }

        private void tmrControl_Tick(object sender, EventArgs e)
        {
            if (Funcionando)
            {
                this.btnIniciar.Enabled = false;
                this.btnIniciarLocal.Enabled = false;
                this.btnIniciarRedan.Enabled = false;
                this.btnParar.Enabled = true;
                if (Parar)
                {
                    this.picEstado.Image = imlEstado.Images[1];
                }
                else
                {
                    this.picEstado.Image = imlEstado.Images[2];
                }
            }
            else
            {
                this.btnIniciar.Enabled = true;
                this.btnIniciarLocal.Enabled = true;
                this.btnIniciarRedan.Enabled = true;
                this.btnParar.Enabled = false;
                this.picEstado.Image = imlEstado.Images[0];
                this.txtIP.Text = "";
                this.Text = "Grabador Simulado - <Desconectado>";
                Parar = false;
            }
            this.txtSesiones.Text = Sesiones.ToString();
            this.txtSesionesTot.Text = SesionesTot.ToString();
            this.dgvSamplers.Refresh();
        }

        private void txtLog_TextChanged(object sender, EventArgs e)
        {
            this.txtLog.SelectionStart = this.txtLog.Text.Length;
            this.txtLog.ScrollToCaret();
            this.txtLog.DeselectAll();
        }

        private void btnIniciarRedan_Click(object sender, EventArgs e)
        {
            IniciarServidor(ServerIPREDAN);
        }

        private void chkLog_CheckedChanged(object sender, EventArgs e)
        {
            LogActivado = this.chkLog.Checked;
        }
       
     }
}
