﻿//------------------------------------------------------------------------------
// <auto-generated>
//     Este código fue generado por una herramienta.
//     Versión de runtime:4.0.30319.18063
//
//     Los cambios en este archivo podrían causar un comportamiento incorrecto y se perderán si
//     se vuelve a generar el código.
// </auto-generated>
//------------------------------------------------------------------------------

namespace U5ki.Infrastructure.Resources {
    using System;
    
    
    /// <summary>
    ///   Clase de recurso fuertemente tipado, para buscar cadenas traducidas, etc.
    /// </summary>
    // StronglyTypedResourceBuilder generó automáticamente esta clase
    // a través de una herramienta como ResGen o Visual Studio.
    // Para agregar o quitar un miembro, edite el archivo .ResX y, a continuación, vuelva a ejecutar ResGen
    // con la opción /str o recompile su proyecto de VS.
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("System.Resources.Tools.StronglyTypedResourceBuilder", "4.0.0.0")]
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    public class Errors {
        
        private static global::System.Resources.ResourceManager resourceMan;
        
        private static global::System.Globalization.CultureInfo resourceCulture;
        
        [global::System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal Errors() {
        }
        
        /// <summary>
        ///   Devuelve la instancia de ResourceManager almacenada en caché utilizada por esta clase.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        public static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp = new global::System.Resources.ResourceManager("U5ki.Infrastructure.Resources.Errors", typeof(Errors).Assembly);
                    resourceMan = temp;
                }
                return resourceMan;
            }
        }
        
        /// <summary>
        ///   Reemplaza la propiedad CurrentUICulture del subproceso actual para todas las
        ///   búsquedas de recursos mediante esta clase de recurso fuertemente tipado.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        public static global::System.Globalization.CultureInfo Culture {
            get {
                return resourceCulture;
            }
            set {
                resourceCulture = value;
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a El elemento {0} en {1} no puede ser nulo..
        /// </summary>
        public static string ElementCannotBeNull {
            get {
                return ResourceManager.GetString("ElementCannotBeNull", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Excepción no esperada.
        /// </summary>
        public static string NotExpectedException {
            get {
                return ResourceManager.GetString("NotExpectedException", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Salida de flujo que no deberia pasar..
        /// </summary>
        public static string NotExpectedFlowCondition {
            get {
                return ResourceManager.GetString("NotExpectedFlowCondition", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a No se ha conseguido sintonizar un elemento, ni se ha encontrado un esclavo para soportarlo temporalmente..
        /// </summary>
        public static string PrimaryAndSlaveGearNotTuned {
            get {
                return ResourceManager.GetString("PrimaryAndSlaveGearNotTuned", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Error iniciando el servicio..
        /// </summary>
        public static string StarServiceError {
            get {
                return ResourceManager.GetString("StarServiceError", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Error apagando servicio..
        /// </summary>
        public static string StopServiceError {
            get {
                return ResourceManager.GetString("StopServiceError", resourceCulture);
            }
        }
    }
}
