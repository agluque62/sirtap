using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using BlueWave.Interop.Asio;

namespace CheckAsioChannels
{
    class Program
    {
        /// <summary>
        /// 
        /// </summary>
       
        [STAThread]
        static void Main(string[] args)
        {
            ConsoleKeyInfo result;
            do
            {
                PrintOpciones();

                result = Console.ReadKey(true);
                switch (result.Key)
                {
                    case ConsoleKey.D1:
                        PrintAsioChannels();
                        break;
                }

            } while (result.Key != ConsoleKey.Escape);
        }

        /// <summary>
        /// 
        /// </summary>
        static void PrintOpciones()
        {
            Console.Clear();

            Console.WriteLine("Chequeo de Nombre de Canales ASIO");
            Console.WriteLine("");
            Console.WriteLine("Opciones:");
            Console.WriteLine("\t1: Ver Caneles");
            Console.WriteLine("");
            Console.WriteLine("\tESC: Salir");
            Console.WriteLine("");
        }

        /// <summary>
        /// 
        /// </summary>
        static void PrintAsioChannels()
        {
            StreamWriter writer = new StreamWriter("misdevices.txt", false);
            if (AsioDriver.InstalledDrivers.Length > 0)
            {
                Print(writer, "Driver ASIO Instalado: " + AsioDriver.InstalledDrivers[0].Name);

                AsioDriver driver = AsioDriver.SelectDriver(AsioDriver.InstalledDrivers[0]);
                driver.CreateBuffers(false);

                Print(writer, "\tCanales de Entrada:");
                foreach (Channel channel in driver.InputChannels)
                {
                    Print(writer, "\t\t" + channel.Name);
                }

                Print(writer, "\n\tCanales de Salida:");
                foreach (Channel channel in driver.OutputChannels)
                {
                    Print(writer, "\t\t" + channel.Name);
                }
                driver.DisposeBuffers();
                driver.Release();
            }
            else
            {
                Print(writer, "No hay driver's ASIO Instalados...");
            }
            writer.Close();

            Console.WriteLine("\nPulse una tecla para continuar...");
            Console.ReadKey();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="msg"></param>
        static void Print(StreamWriter writer, string msg)
        {
            Console.WriteLine(msg);
            writer.WriteLine(msg);
        }
    }
}
