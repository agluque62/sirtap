using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;


namespace NucleoSim
{
    delegate void OutputMethod (string Texto);

    public class UdpState
    {
        public IPEndPoint e;
        public UdpClient u;
    }

    class UDPListener
    {
        bool iniciado = false;
        private int port = 0;
        private UdpClient socket = null;
        OutputMethod output = null;

        public UDPListener()
        {
        }

        public UDPListener(UdpClient socket, OutputMethod output)
        {
            this.socket = socket;
            this.output = output;
        }

        public UDPListener(OutputMethod output)
        {
            this.output = output;
        }

        public void Iniciar(int port=-1)
        {
            if (!iniciado)
            {
                if (port > 0)
                {
                    this.port = port;
                    try
                    {
                        IPEndPoint e = new IPEndPoint(IPAddress.Any, this.port);
                        socket = new UdpClient(e);
                        UdpState s = new UdpState();
                        s.e = e;
                        s.u = socket;
                        iniciado = true;
                        socket.BeginReceive(new AsyncCallback(ReceiveCallback), s);
                    }
                    catch (Exception e)
                    {
                        System.Windows.Forms.MessageBox.Show(e.Message);
                    }
                }
                else if (socket != null)
                {
                    UdpState s = new UdpState();
                    s.e = (IPEndPoint)socket.Client.LocalEndPoint;
                    s.u = socket;
                    iniciado = true;
                    socket.BeginReceive(new AsyncCallback(ReceiveCallback), s);
                }
            }
        }

        public void Ejecutar ()
        {
            while (iniciado)
            {
            }
        }

        public void Parar()
        {
            if (iniciado)
            {
                socket.Close();
                socket = null;
                iniciado = false;
            }
        }

        public void ReceiveCallback(IAsyncResult ar)
        {
            UdpClient u = (UdpClient)((UdpState)(ar.AsyncState)).u;
            IPEndPoint e = (IPEndPoint)((UdpState)(ar.AsyncState)).e;

            Byte[] receiveBytes = u.EndReceive(ar, ref e);
            string receiveString = Encoding.ASCII.GetString(receiveBytes);

            if (output != null)
            {
                output.Invoke(receiveString);
            }
            socket.BeginReceive(new AsyncCallback(ReceiveCallback), (UdpState)(ar.AsyncState));
        }



    }
}
