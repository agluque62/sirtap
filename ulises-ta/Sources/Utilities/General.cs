using System;
using System.Text;
using System.Collections.Generic;
using System.Runtime.Remoting.Messaging;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using NLog;
using System.Windows.Forms;
using System.Xml;
using System.Threading.Tasks;
using System.IO;

namespace Utilities
{
	public delegate void GenericEventHandler(object sender);
	public delegate void GenericEventHandler<T>(object sender, T par);
	public delegate void GenericEventHandler<T1, T2>(object sender, T1 par1, T2 par2);
	public delegate void GenericEventHandler<T1, T2, T3>(object sender, T1 par1, T2 par2, T3 par3);
	public delegate void GenericEventHandler<T1, T2, T3, T4>(object sender, T1 par1, T2 par2, T3 par3, T4 par4);
	public delegate void GenericEventHandler<T1, T2, T3, T4, T5>(object sender, T1 par1, T2 par2, T3 par3, T4 par4, T5 par5);

	public struct Pair
	{
		public readonly object First;
		public readonly object Second;

		public Pair(object first, object second)
		{
			First = first;
			Second = second;
		}

		public override string ToString()
		{
			return string.Format("{0} {1}", First, Second);
		}
	}

	public struct Pair<T>
   {
      public readonly T First;
      public readonly T Second;

      public Pair(T first, T second)
      {
         First = first;
         Second = second;
      }

		public override string ToString()
		{
			return string.Format("{0} {1}", First, Second);
		}
   }

   public struct Pair<T1, T2>
   {
      public readonly T1 First;
      public readonly T2 Second;

      public Pair(T1 first, T2 second)
      {
         First = first;
         Second = second;
      }

		public override string ToString()
		{
			return string.Format("{0} {1}", First, Second);
		}
   }

	public struct Tuple<T1, T2, T3>
	{
      public readonly T1 First;
      public readonly T2 Second;
		public readonly T3 Third;

      public Tuple(T1 first, T2 second, T3 third)
      {
         First = first;
         Second = second;
			Third = third;
      }

		public override string ToString()
		{
			return string.Format("{0} {1} {2}", First, Second, Third);
		}
	}

	//public class EventArg<T> : EventArgs
	//{
	//   public T Info;

	//   public EventArg(T info)
	//   {
	//      Info = info;
	//   }
	//}

   public static class General
   {
		// static Logger _Logger = LogManager.GetCurrentClassLogger();

		public static void SafeLaunchEvent(GenericEventHandler ev, object sender)
		{
			if (ev != null)
			{
				ev(sender);
			}
		}

      public static void SafeLaunchEvent<T>(GenericEventHandler<T> ev, object sender, T par)
      {
         // _Logger.Info("SafeLaunchEvent: {0}", par);
         if (ev != null)
         {
            ev(sender, par);
         }
      }

		public static void SafeLaunchEvent<T1, T2>(GenericEventHandler<T1, T2> ev, object sender, T1 par1, T2 par2)
		{
			if (ev != null)
			{
				ev(sender, par1, par2);
			}
		}

		public static void SafeLaunchEvent<T1, T2, T3>(GenericEventHandler<T1, T2, T3> ev, object sender, T1 par1, T2 par2, T3 par3)
		{
			if (ev != null)
			{
				ev(sender, par1, par2, par3);
			}
		}

		public static void SafeLaunchEvent<T1, T2, T3, T4>(GenericEventHandler<T1, T2, T3, T4> ev, object sender, T1 par1, T2 par2, T3 par3, T4 par4)
		{
			if (ev != null)
			{
				ev(sender, par1, par2, par3, par4);
			}
		}

		public static void SafeLaunchEvent<T1, T2, T3, T4, T5>(GenericEventHandler<T1, T2, T3, T4, T5> ev, object sender, T1 par1, T2 par2, T3 par3, T4 par4, T5 par5)
		{
			if (ev != null)
			{
				ev(sender, par1, par2, par3, par4, par5);
			}
		}

		public static void SafeLaunchEvent(EventHandler ev, object sender)
		{
			if (ev != null)
			{
				ev(sender, EventArgs.Empty);
			}
		}

		public static void SafeLaunchEvent<T>(EventHandler<T> ev, object sender, T e)
			where T : EventArgs
		{
			if (ev != null)
			{
				ev(sender, e);
			}
		}

      public static void AsyncSafeLaunchEvent<T>(GenericEventHandler<T> ev, object sender, T msg)
      {
         if (ev != null)
         {
            GenericEventHandler<T> auxEv = delegate(object s, T m)
            {
               ev(s, m);
            };
            auxEv.BeginInvoke(sender, msg, AsyncSafeLaunchEventCallback, null);
         }
      }

      public static ushort Crc16(byte[] data)
      {
         const ushort POLINOMIO = 0x9021;
         int crc = 0;

         for (int i = 0, num = data.Length; i < num; i++)
         {
            byte c = data[i];

            for (int j = 0; j < 8; j++)
            {
               int cn = c ^ ((crc >> 8) & 0xff);

               crc <<= 1;
               if ((cn & 0x80) > 0)
               {
                  crc ^= POLINOMIO;
               }

               c <<= 1;
            }
         }

         crc = (crc << 8) | (crc >> 8);
         crc &= 0x7f7f;

         return (ushort)crc;
      }

		public static bool TimeElapsed(int? last, int interval)
		{
			if (last.HasValue)
			{
				if (Environment.TickCount - last.Value <= interval)
				{
					return false;
				}
			}

			return true;
		}

		public static bool TimeElapsed(ref int? last, int interval)
		{
			if (last.HasValue)
			{
				if (Environment.TickCount - last.Value <= interval)
				{
					return false;
				}

				last = null;
			}

			return true;
		}

		public static List<string> GetOperationalV4Ips()
		{
			List<string> ips = new List<string>();
			NetworkInterface[] nets = NetworkInterface.GetAllNetworkInterfaces();

			foreach (NetworkInterface iface in nets)
			{
				if ((iface.NetworkInterfaceType != NetworkInterfaceType.Loopback) &&
					(iface.OperationalStatus == OperationalStatus.Up))
				{
					foreach (UnicastIPAddressInformation ip in iface.GetIPProperties().UnicastAddresses)
					{
						if (ip.Address.AddressFamily == AddressFamily.InterNetwork)
						{
							ips.Add(ip.Address.ToString());
						}
					}
				}
			}

			ips.Sort();
			return ips;
		}

      static void AsyncSafeLaunchEventCallback(IAsyncResult ar)
      {
         object delg = ((AsyncResult)ar).AsyncDelegate;
         delg.GetType().GetMethod("EndInvoke").Invoke(delg, new object[] { ar });
      }
   }

	public class BinToLogString
	{
		private byte[] _Data;
		private int _Length;

		public BinToLogString(byte[] data)
		{
			_Data = data;
			_Length = data.Length;
		}

		public BinToLogString(byte[] data, int length)
		{
			_Data = data;
			_Length = length;
		}

		public override string ToString()
		{
			StringBuilder str = new StringBuilder();

			if ((_Data != null) && (_Length > 0))
			{
				StringBuilder hexStr = new StringBuilder(48);
				StringBuilder asciiStr = new StringBuilder(16);

				for (int i = 0, j = 0, iTotal = (_Length + 15) / 16; i < iTotal; i++)
				{
					for (int jTotal = Math.Min(_Length, (i + 1) * 16); j < jTotal; j++)
					{
						hexStr.AppendFormat("{0:X02} ", _Data[j]);
						asciiStr.Append(_Data[j] > 0x20 && _Data[j] < 0x7F ? (char)_Data[j] : '.');
					}

					str.AppendFormat("{0:X08}  {1,-48} {2}{3}", i * 16, hexStr, asciiStr, Environment.NewLine);
					hexStr.Length = 0;
					asciiStr.Length = 0;
				}

				int newLineLength = Environment.NewLine.Length;
				str.Remove(str.Length - newLineLength, newLineLength);
			}

			return str.ToString();
		}
		public static Dictionary<string, string> ReadXml(
			string file = "archivotonos.xml",
			string usuario = "L2",
			string nodo = "//usuario",
			string cnombre = "nombre",
			string ctonos = "tonos",
			string cllamada = "llamada"
			)
		{
			Dictionary<string, string> dict = new Dictionary<string, string>();
			try
			{
				XmlDocument xmlDoc = new XmlDocument();
				xmlDoc.Load(file);

				XmlNodeList userNodes = xmlDoc.SelectNodes("//usuario");
				foreach (XmlNode userNode in userNodes)
				{
					string nombreUsuario = userNode.Attributes[cnombre].Value;
					if (nombreUsuario == usuario)
					{
						XmlNodeList tonoNodes = userNode.SelectNodes(ctonos);
						foreach (XmlNode tonoNode in tonoNodes)
						{
							string llamada = tonoNode.Attributes[cllamada].Value;
							string tono = tonoNode.InnerText;
							dict[llamada] = tono;
						}
						break;
					}
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show("Error al cargar los tonos por llamada: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			return dict;
		}

	}

#if PERFORMOPERATION
	// Define la interfaz IOperationService
	public interface IOperationService
	{
		Task<string> PerformOperation(string operation, object data);
	}

	// Implementa la interfaz IOperationService
	public class OperationService : IOperationService
	{
		public async Task<string> PerformOperation(string operation, object data)
		{
			switch (operation)
			{
				case "HttpService":
					return await PerformHttpService((HttpServiceData)data);
				case "HttpServiceSimulated":
					return await PerformHttpServiceSimulated((HttpServiceData)data);
				default:
					throw new ArgumentException("Operación no válida");
			}
		}

		private async Task<string> PerformHttpService(HttpServiceData data)
		{
			// Lógica para realizar la solicitud HTTP

			return await Utilities.HttpHelper.SendPostRequest(data.ApiUrl, data.PostData);
		}
		private async Task<string> PerformHttpServiceSimulated(HttpServiceData data)
		{
			// Lógica para realizar la solicitud HTTP
			data.ApiUrl = "http://localhost:3000/login";
			if (data.PostData == $"{{\"username\": \"\", \"password\": \"\"}}")
				return "OK";
			return await Utilities.HttpHelper.SendPostRequest(data.ApiUrl, data.PostData);
		}

		private /*async Task*/string PerformJsonReader(JsonReaderData data)
		{
			// Lógica para leer el archivo JSON
			try
			{
				string jsonContent = File.ReadAllText(data.JsonFilePath);
				return jsonContent;
			}
			catch (Exception ex)
			{
				Console.WriteLine($"Error al leer el archivo JSON: {ex.Message}");
				throw;
			}
		}
	}
#endif

#if NEW_SERVICE
	public interface IOperationService1
	{
		Task <string> Login(string usuario, string clave);
		Task Logout();
	}

	public class SimultatedLoginService : IOperationService1
	{
		Task<string> IOperationService1.Login(string usuario, string clave)
		{
			if (usuario == "1" && clave == "1") 
				return Task.FromResult("MISION 1");
			// Define los datos para la operación HttpService
			var httpServiceData = new HttpServiceData
			{
				ApiUrl = "http://localhost:3000/login",
				PostData = $"{{\"username\": \"{ usuario}\", \"password\": \"{clave}\"}}"
			};
			// Ejecuta la operación HttpService
			string result = Utilities.HttpHelper.SendPostRequest(httpServiceData.ApiUrl, httpServiceData.PostData).Result;
			return Task.FromResult(result);

		}

		Task IOperationService1.Logout()
		{
			throw new NotImplementedException();
		}
	}

	public class RealLoginService : IOperationService1
	{
		public Task<string> Login(string usuario, string clave)
		{
			throw new NotImplementedException();
		}

		public Task Logout()
		{
			throw new NotImplementedException();
		}
	}

#endif

	public interface IValidadorCredenciales
	{
		string SimuladorValidarCredenciales(string usuario, string clave);
		Task<string> ValidarCredencialeshttp(string usuario, string clave);
		Task <string> Login(string usuario, string clave);
		Task Logout();
	}

	public class ValidadorCredenciales : IValidadorCredenciales
	{
		public string SimuladorValidarCredenciales(string usuario, string clave)
		{
			// Lógica de validación de credenciales (puedes personalizarla según tus necesidades)
			if (usuario == "1" && clave == "1")
			{
				return "MISION 1";
			}
			if (usuario == "2" && clave == "2")
			{
				return "MISION 2";
			}
			return "";
		}
		public async Task<string> ValidarCredencialeshttp(string usuario, string clave)
		{
			// Lógica para realizar la solicitud HTTP
			var data= new HttpServiceData
			{
				ApiUrl = "http://localhost:3000/login",
				PostData = $"{{\"username\": \"{ usuario}\", \"password\": \"{ clave}\"}}"
			};
			return await Utilities.HttpHelper.SendPostRequest(data.ApiUrl, data.PostData);
		}

        Task<string> IValidadorCredenciales.Login(string usuario, string clave)
        {
			// Lógica de validación de credenciales (puedes personalizarla según tus necesidades)
			if (usuario == "1" && clave == "1")
			{
				return Task.FromResult("MISION 1");
			}
			if (usuario == "3" && clave == "3")
			{
				return Task.FromResult("MISION 3");
			}
			if (clave == "2") return Task.FromResult("MISION 2");
			if (clave == "4") return Task.FromResult("MISION 4");
			if (clave == "5") return Task.FromResult("MISION 5");
			if (clave == "6") return Task.FromResult("MISION 6");
			if (clave == "7") return Task.FromResult("MISION 7");
			if (clave == "8") return Task.FromResult("MISION 8");
			if (clave == "9") return Task.FromResult("MISION 9");
			if (clave == "0") return Task.FromResult("MISION 0");
			else
				return Task.FromResult("");
			//throw new NotImplementedException();
		}

        Task IValidadorCredenciales.Logout()
        {
			return Task.FromResult("");
            //throw new NotImplementedException();
        }
    }

	// Define clases de datos específicas para cada operación
	public class HttpServiceData
	{
		public string ApiUrl { get; set; }
		public string PostData { get; set; }
	}
}
