using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SirtapRadio
{
    internal static class Program
    {
        public static Form1 MainForm;
        /// <summary>
        /// Punto de entrada principal para la aplicación.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            MainForm = new Form1();
            Application.Run(MainForm);
        }

        public static void SetParamValueAppConfiguration(string param, string value)
        {
            try
            {
                var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                var settings = configFile.AppSettings.Settings;
                if (settings[param] == null)
                {
                    settings.Add(param, value);
                }
                else
                {
                    settings[param].Value = value;
                }
                configFile.Save(ConfigurationSaveMode.Modified);
                ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);
            }
            catch (Exception)
            {
                MessageBox.Show($"No se puede establecer el parametro {param} valor {value} en el fichero de configuracion", "ERROR",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }    
}