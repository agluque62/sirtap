using System.Windows.Forms;

namespace PictureBox
{
    public partial class Form1 : Form
    {
        private int tiempoDeVisualizacion = 10; // Valor predeterminado
        public Form1(int tiempo)
        {
            InitializeComponent();
            tiempoDeVisualizacion = tiempo;
            // Ruta al archivo PNG que deseas mostrar
            string Path = @"C:\Program Files (x86)\UlisesV5000\Ulises5000I-TA\Resources\";
            string image = @"trabajando.png";
            string imagePath = Path+image;
            // Cargar la imagen en el PictureBox
            if (File.Exists(imagePath))
                pictureBox1.Image = System.Drawing.Image.FromFile(imagePath);
            else
            {
                image = @"wait.ico";
                imagePath = Path + image;
                if (File.Exists(imagePath))
                    pictureBox1.Image = System.Drawing.Image.FromFile(imagePath);

            }
            // Configurar el estado del formulario para que arranque maximizado
            this.WindowState = FormWindowState.Maximized;
            // Configurar el ajuste de imagen para que la imagen se expanda al tamaño del PictureBox
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            // Ajustar tamaño y ubicación del PictureBox para ocupar toda la ventana
            pictureBox1.Dock = DockStyle.Fill;
            System.Windows.Forms.Timer timer = new System.Windows.Forms.Timer();
            timer.Interval = tiempoDeVisualizacion * 1000; // Convertir a milisegundos
            timer.Tick += Timer_Tick;
            timer.Start();
        }
        private void Timer_Tick(object sender, EventArgs e)
        {
            this.Close();
        }
        private void Timer1_Tick(object sender, EventArgs e)
        {

        }
    }
}
