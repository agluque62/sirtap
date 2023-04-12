using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Threading;
using NLog;
using Nucleo.CheckIO;

namespace CheckHid
{
    class Program
    {
        /// <summary>
        /// 
        /// </summary>
        static Logger _nlog = LogManager.GetCurrentClassLogger();
        static Random rnd = new Random();

        /// <summary>
        /// Gestor de Dispositivos...
        /// </summary>
        static CMediaDevManager _devman = new CMediaDevManager();
        static bool bRunning = false;

        /// <summary>
        /// Rutina de Entrada...
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            _devman.Init();

            ConsoleKeyInfo result;
            do
            {
                PrintOpciones();

                result = Console.ReadKey(true);
                switch (result.Key)
                {
                    case ConsoleKey.D1:
                        if (bRunning == false)
                        {
                            bRunning = true;
                            (new Thread(new ThreadStart(CheckHidThread)) { IsBackground = true }).Start();
                        }
                        break;

                    case ConsoleKey.D2:
                        bRunning = false;
                        break;

                    case ConsoleKey.D3:
                        break;

                }
            } while (result.Key != ConsoleKey.Escape);

            bRunning = false;
        }

        /// <summary>
        /// 
        /// </summary>
        static void PrintOpciones()
        {
            Console.Clear();

            Console.WriteLine("CheckHid 1.1. NucleoDF Copyright 2014-2015.");
            Console.WriteLine("");
            Console.WriteLine("Hid Devices Found: {0}", _devman.DevList());
            Console.WriteLine("");
            Console.WriteLine("Status: {0}", bRunning == true ? "Test Started" : "Test Stopped");
            Console.WriteLine("");
            Console.WriteLine("Options:");
            Console.WriteLine("\t1: Test Start.");
            Console.WriteLine("\t2: Test Stop.");
            Console.WriteLine("");
            Console.WriteLine("\tESC: Exit");
            Console.WriteLine("");
        }

        /// <summary>
        /// 
        /// </summary>
        static void CheckHidThread()
        {
            SortedDictionary<byte, ISoundDevice> deviceList;
            
            // Inicializacion de la prueba...
            try
            {
                _nlog.Info("CheckHidThread Started");
                deviceList = _devman.ListDevice();
                _devman.Start();
            }
            catch (Exception x)
            {
                _nlog.Fatal("Exception on Starting CheckHidThread:\n\t{0}", x.Message);
                bRunning = false;
                PrintOpciones();
                return;
            }

            // Lazo de la Prueba....
            while (bRunning == true)
            {
                int ptime = Properties.Settings1.Default.PTime;
                try
                {
                    //
                    foreach (ISoundDevice sndDev in deviceList.Values)
                    {
                        Thread.Sleep(ptime);
                        if (sndDev != null)
                        {
                            sndDev.GetState();
                            switch (rnd.Next(4))
                            {
                                case 0:                            
                                    sndDev.SetGpio(4, 1);
                                    break;
                                case 1:                            
                                    sndDev.SetGpio(4, 0);
                                    break;
                                case 2:
                                    sndDev.SetGpio(5, 1);
                                    break;
                                case 3:
                                    sndDev.SetGpio(5, 0);
                                    break;
                            }
                        }
                    }

                }
                catch (Exception x)
                {
                    _nlog.Fatal("Exception on CheckHidThread Loop: \n\t{0}", x.Message);
                    bRunning = false;
                    PrintOpciones();
                    return;
                }
            }

            // Finalización de la Prueba.
            try
            {
                _devman.Close();
                _nlog.Info("CheckHidThread Stopped");
            }
            catch (Exception )
            {
                bRunning = false;
                PrintOpciones();
                return;
            }
        }

    }
}
