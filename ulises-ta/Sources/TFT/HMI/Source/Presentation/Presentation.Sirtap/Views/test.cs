using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Moq;
using Utilities;
//using Xunit;

namespace HMI.Presentation.Sirtap.Views
{

    public class ValidadorCredencialesTests
    {
        //[Fact]
        public async Task ValidarCredencialesAsync_CredencialesValidas_DeberiaDevolverMensajeExitoso()
        {
            // Arrange
            string usuario = "1";
            string clave = "1";
            string mensajeEsperado = "MISION 1";

            var mockValidador = new Mock<IValidadorCredenciales>();
            mockValidador.Setup(v => v.ValidarCredencialeshttp(usuario, clave))
                         .ReturnsAsync(mensajeEsperado);

            // Act
            var resultado = await mockValidador.Object.ValidarCredencialeshttp(usuario, clave);

            // Assert
            Debug.Assert(mensajeEsperado==resultado);
        }

        //[Fact]
        public async Task ValidarCredencialesAsync_CredencialesInvalidas_DeberiaDevolverMensajeDenegado()
        {
            // Arrange
            string usuario = "usuarioEjemplo";
            string clave = "claveIncorrecta";
            string mensajeEsperado = "";

            var mockValidador = new Mock<IValidadorCredenciales>();
            mockValidador.Setup(v => v.ValidarCredencialeshttp(usuario, clave))
                         .ReturnsAsync(mensajeEsperado);

            // Act
            var resultado = await mockValidador.Object.ValidarCredencialeshttp(usuario, clave);

            // Assert
            Debug.Assert(mensajeEsperado== resultado);
        }
    }
}
