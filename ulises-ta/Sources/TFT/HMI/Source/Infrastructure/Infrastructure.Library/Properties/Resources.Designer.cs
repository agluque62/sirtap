﻿//------------------------------------------------------------------------------
// <auto-generated>
//     Este código fue generado por una herramienta.
//     Versión de runtime:4.0.30319.42000
//
//     Los cambios en este archivo podrían causar un comportamiento incorrecto y se perderán si
//     se vuelve a generar el código.
// </auto-generated>
//------------------------------------------------------------------------------

namespace HMI.Infrastructure.Library.Properties {
    using System;
    
    
    /// <summary>
    ///   Clase de recurso fuertemente tipado, para buscar cadenas traducidas, etc.
    /// </summary>
    // StronglyTypedResourceBuilder generó automáticamente esta clase
    // a través de una herramienta como ResGen o Visual Studio.
    // Para agregar o quitar un miembro, edite el archivo .ResX y, a continuación, vuelva a ejecutar ResGen
    // con la opción /str o recompile su proyecto de VS.
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("System.Resources.Tools.StronglyTypedResourceBuilder", "17.0.0.0")]
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    internal class Resources {
        
        private static global::System.Resources.ResourceManager resourceMan;
        
        private static global::System.Globalization.CultureInfo resourceCulture;
        
        [global::System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal Resources() {
        }
        
        /// <summary>
        ///   Devuelve la instancia de ResourceManager almacenada en caché utilizada por esta clase.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp = new global::System.Resources.ResourceManager("HMI.Infrastructure.Library.Properties.Resources", typeof(Resources).Assembly);
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
        internal static global::System.Globalization.CultureInfo Culture {
            get {
                return resourceCulture;
            }
            set {
                resourceCulture = value;
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a AddServices() method called for Module {0}..
        /// </summary>
        internal static string AddServicesCalled {
            get {
                return ResourceManager.GetString("AddServicesCalled", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Section &apos;{0}&apos; depends on section &apos;{1}&apos; which was not found..
        /// </summary>
        internal static string DependencyNotFound {
            get {
                return ResourceManager.GetString("DependencyNotFound", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a A duplicated module with name {0} has been found by the loader..
        /// </summary>
        internal static string DuplicatedModule {
            get {
                return ResourceManager.GetString("DuplicatedModule", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Failed to load module from assembly {0}. Error was:
        ///{1}.
        /// </summary>
        internal static string FailedToLoadModule {
            get {
                return ResourceManager.GetString("FailedToLoadModule", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a The solution profile references an unknown schema..
        /// </summary>
        internal static string InvalidSolutionProfileSchema {
            get {
                return ResourceManager.GetString("InvalidSolutionProfileSchema", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Module {0} added to the container..
        /// </summary>
        internal static string LogModuleAdded {
            get {
                return ResourceManager.GetString("LogModuleAdded", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Loaded assembly file {0} for Module..
        /// </summary>
        internal static string LogModuleAssemblyLoaded {
            get {
                return ResourceManager.GetString("LogModuleAssemblyLoaded", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Assembly file {0} was not found..
        /// </summary>
        internal static string ModuleNotFound {
            get {
                return ResourceManager.GetString("ModuleNotFound", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Load() method called for Module {0}..
        /// </summary>
        internal static string ModuleStartCalled {
            get {
                return ResourceManager.GetString("ModuleStartCalled", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a No user was selected for the authentication service..
        /// </summary>
        internal static string NoUserProvidedForAuthentication {
            get {
                return ResourceManager.GetString("NoUserProvidedForAuthentication", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a The ModuleLoaderService cannot initialize the module because some of their dependencies are not present. Make sure to deploy all the assemblies needed to execute the module..
        /// </summary>
        internal static string ReferencedAssemblyNotFound {
            get {
                return ResourceManager.GetString("ReferencedAssemblyNotFound", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Service {1} added for Module {0}..
        /// </summary>
        internal static string ServiceAdded {
            get {
                return ResourceManager.GetString("ServiceAdded", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a Service {1} will be added on demand for Module {0}..
        /// </summary>
        internal static string ServiceAddedOnDemand {
            get {
                return ResourceManager.GetString("ServiceAddedOnDemand", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Busca una cadena traducida similar a There is no a user matching the information you have provided..
        /// </summary>
        internal static string UserNotFoundMessage {
            get {
                return ResourceManager.GetString("UserNotFoundMessage", resourceCulture);
            }
        }
    }
}
