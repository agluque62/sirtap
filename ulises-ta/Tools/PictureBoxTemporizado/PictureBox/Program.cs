namespace PictureBox
{
    internal static class Program
    {
        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            // To customize application configuration such as set high DPI settings or default font,
            // see https://aka.ms/applicationconfiguration.
            ApplicationConfiguration.Initialize();
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            int tiempoDeVisualizacion = 10; // Valor predeterminado

            if (args.Length > 0 && int.TryParse(args[0], out int tiempo))
            {
                tiempoDeVisualizacion = tiempo;
            }

            Application.Run(new Form1(tiempoDeVisualizacion));
        }
    }
}