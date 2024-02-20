using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Net;
using System.Net.Sockets;


namespace NucleoSim
{

    delegate void LogMethod(string Texto);

    class RecordEvents
    {
        static public string CtlSetParams = "I00";
        static public string CtlConfig = "H00";
        static public string CtlEnd = "H01";


        static public string GrsStartSession = "G00";
        static public string GrsStopSession = "G01";
        static public string GrsSqOn = "G02";
        static public string GrsSqOff = "G03";
        static public string GrsSTOn = "G04";
        static public string GrsSTOff = "G05";

        // Eventos Originados en el Subsistema de Terminal... (los 'mandatory')
        static public string TerStartSession = "T00";
        static public string TerStopSession = "T01";
        static public string TerCallStart = "T02";
        static public string TerCallEnd = "T03";
        static public string TerCallConnect = "T04";
        static public string TerCallTransferStart = "T05";      // Cuando se inicia la llamada al que se quiere transferir.
        static public string TerCallTransferDisc = "T06";       // Cuando se desconecta el origen...
        static public string TerCallTransferConn = "T07";       // Cuando se conecta el nuevo destino...
        static public string TerCallHoldOn = "T08";
        static public string TerCallHoldOff= "T09";
        static public string TerPttOn = "T20";
        static public string TerPttOff = "T21";

        // Envio de MEDIA...
        static public string Media = "MMM";
        static public string Record = "I01";                    //ESC. 22.01.2016. Añadido
        static public string Pause = "I02";                     //ESC. 22.01.2016. Añadido


    }


    class RecordClient
    {
        private int ServerPort = -1;
        private IPAddress ServerIP = null;
        private UdpClient Socket = null;
        private IPEndPoint URL = null;
        private LogMethod LOG = null;

        //private readonly Mutex m = new Mutex();


        public RecordClient(int opcion=1)
        {
            if (opcion == 4)
            {
                ServerPort = Properties.NucleoSim.Default.ServerPort3;
                ServerIP = IPAddress.Parse(Properties.NucleoSim.Default.ServerIP3);
            }
            else if (opcion == 3)
            {
                ServerPort = Properties.NucleoSim.Default.ServerPort2;
                ServerIP = IPAddress.Parse(Properties.NucleoSim.Default.ServerIP2);
            }
            else if (opcion == 2)
            {
                ServerPort = Properties.NucleoSim.Default.ServerPort2;
                ServerIP = IPAddress.Parse(Properties.NucleoSim.Default.ServerIP2);
            }
            else
            {
                ServerPort = Properties.NucleoSim.Default.ServerPort1;
                ServerIP = IPAddress.Parse(Properties.NucleoSim.Default.ServerIP1);
            }
            URL = new IPEndPoint(ServerIP, ServerPort);
            Socket = new UdpClient();
        }

        public UdpClient GetUDPSocket()
        {
            return (Socket);
        }

        public int GetUDPPort()
        {
            return (((IPEndPoint)Socket.Client.LocalEndPoint).Port);
        }

        public RecordClient( LogMethod log, int opcion=1)
        {
            if (opcion == 3)
            {
                ServerPort = Properties.NucleoSim.Default.ServerPort3;
                ServerIP = IPAddress.Parse(Properties.NucleoSim.Default.ServerIP3);
            }
            else if (opcion == 2)
            {
                ServerPort = Properties.NucleoSim.Default.ServerPort2;
                ServerIP = IPAddress.Parse(Properties.NucleoSim.Default.ServerIP2);
            }
            else
            {
                ServerPort = Properties.NucleoSim.Default.ServerPort1;
                ServerIP = IPAddress.Parse(Properties.NucleoSim.Default.ServerIP1);
            }
            URL = new IPEndPoint(ServerIP, ServerPort);
            Socket = new UdpClient();
            Log = log;
        }

        public IPAddress IPServicio
        {
            get
            {
                return (ServerIP);
            }
        }

        public int PuertoServicio
        {
            get
            {
                return (ServerPort);
            }
        }

        public LogMethod Log
        {
            get
            {
                return (LOG);
            }
            set
            {
                LOG = value;
            }
        }

        private void Traza(string Texto)
        {
            if (LOG != null)
            {
                LOG.Invoke(Texto);
            }
        }

        private string ByteArrayToString(byte[] Datos)
        {
            return ( BitConverter.ToString(Datos).Replace('-', ' ' ) );
        }

        private void Send(string Evento)
        {
            Send(Evento, null, null);
        }

        private void Send(string Evento, string Recurso, params object[] Valores)
        {
             byte[] Datos = null;
            Datos = GetSimpleCmd(Evento, Recurso, Valores);
            Traza ( "\tComando " + Evento + " Datos=[" + Encoding.Default.GetString(Datos) +"]" );
            Traza("\t" + ByteArrayToString(Datos) );
            //m.WaitOne();
            int res = Socket.Send(Datos, Datos.Count(), URL);
            //m.ReleaseMutex();
            Boolean Ok = (res == Datos.Count());
            Traza("\tEnviado (Ok=" + Ok +")\r\n");
        }

        public void CtlSetParams(int opcion)
        {
            string ip = Properties.NucleoSim.Default.IPPasarela;
            if (opcion == 3)
            {
                ip = Properties.NucleoSim.Default.IPPasarela2;
            }
            Send(RecordEvents.CtlSetParams, ip);
        }

        public void CtlConfig()
        {
            Send(RecordEvents.CtlConfig);
        }

        public void CtlEnd()
        {
            Send(RecordEvents.CtlEnd);
        }

        public void GrsStartSession(string nrec)
        {
            Send(RecordEvents.GrsStartSession, nrec);
        }

        public void GrsEndSession(string nrec)
        {
            Send(RecordEvents.GrsStopSession, nrec);
        }

        public void GrsSquelchOn(string nrec)
        {
            Send(RecordEvents.GrsSqOn, nrec, nrec);     //ESC. 22.01.2016. Añadido nrec, como FreqID
        }

        public void GrsSquelchOff(string nrec)
        {
            Send(RecordEvents.GrsSqOff, nrec, nrec);    //ESC. 22.01.2016. Añadido nrec, como FreqID
        }

        public void GrsSTOn(string nrec)
        {
            Send(RecordEvents.GrsSTOn, nrec );
        }

        public void GrsSTOff(string nrec)
        {
            Send(RecordEvents.GrsSTOff, nrec );
        }

        public void TerStartSession(string nrec)
        {
            Send(RecordEvents.TerStartSession, nrec);
        }

        public void TerEndSession(string nrec)
        {
            Send(RecordEvents.TerStopSession, nrec);
        }

        public void TerPttOn(string nrec, string tipo = "1")
        {
            Send(RecordEvents.TerPttOn, nrec, tipo );
        }

        public void TerPttOff(string nrec)
        {
            Send(RecordEvents.TerPttOff, nrec );
        }

        public void TerCallStart(string nrec, string direccion, string prioridad, string telf1, string telf2 )
        {
            Send(RecordEvents.TerCallStart, nrec, direccion, prioridad, telf1, telf2);
        }

        public void TerCallEnd(string nrec, string causa, string origen)
        {
            Send(RecordEvents.TerCallEnd, nrec, causa, origen);
        }

        public void TerCallConnect(string nrec, string telf)
        {
            Send(RecordEvents.TerCallConnect, nrec, telf);
        }

        public void TerCallTransferStart(string nrec, string nrec2, string telf1, string telf2)
        {
            Send(RecordEvents.TerCallTransferStart, nrec, nrec2, telf1, telf2);
        }

        public void TerCallTransferDisc(string nrec, string nrec2, string causa, string origen)
        {
            Send(RecordEvents.TerCallTransferDisc, nrec, nrec2, causa, origen);
        }

        public void TerCallTransferConn(string nrec, string nrec2, string telf)
        {
            Send(RecordEvents.TerCallTransferConn, nrec, nrec2, telf);
        }

        public void TerCallHoldOn(string nrec)
        {
            Send(RecordEvents.TerCallHoldOn, nrec);
        }

        public void TerCallHoldOff(string nrec)
        {
            Send(RecordEvents.TerCallHoldOff, nrec);
        }

 //ESC. 22.01.2016. Añadido
        public void Record(string nrec)
        {
            Send(RecordEvents.Record, nrec);
        }

        public void Pause(string nrec)
        {
            Send(RecordEvents.Pause, nrec);
        }
        //--ESC

        public void SendMedia(string nrec, int paquete, byte[] data, int lon)
        {
            string init = string.Format("V,{0},{1},{2},", RecordEvents.Media, nrec, paquete);            
            byte[] cmd = new byte[init.Length + lon];           

            Array.Copy(Encoding.ASCII.GetBytes(init), 0, cmd, 0, init.Length); 
            Array.Copy(data, 0, cmd, init.Length, lon);

            //m.WaitOne();
            Socket.Send(cmd, init.Length + lon, URL);
            //m.ReleaseMutex();
        }

        private byte[] GetSimpleCmd(string ev, string nrec, object[] Valores)
        {
            StringBuilder strcmd = new StringBuilder("V,");
            strcmd.Append(ev);
            if (nrec != null)
            {
                strcmd.Append(",");
                strcmd.Append( nrec );
            }
            if (Valores != null)
            {
                foreach (object Valor in Valores)
                {
                    strcmd.Append(",");
                    strcmd.Append((string)Valor);
                }
            }
            return Encoding.ASCII.GetBytes(strcmd.ToString());
        }
    }


}
