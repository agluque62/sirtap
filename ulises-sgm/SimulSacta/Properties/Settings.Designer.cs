﻿//------------------------------------------------------------------------------
// <auto-generated>
//     Este código fue generado por una herramienta.
//     Versión de runtime:4.0.30319.42000
//
//     Los cambios en este archivo podrían causar un comportamiento incorrecto y se perderán si
//     se vuelve a generar el código.
// </auto-generated>
//------------------------------------------------------------------------------

namespace SimulSACTA.Properties {
    
    
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.Editors.SettingsDesigner.SettingsSingleFileGenerator", "15.8.0.0")]
    internal sealed partial class Settings : global::System.Configuration.ApplicationSettingsBase {
        
        private static Settings defaultInstance = ((Settings)(global::System.Configuration.ApplicationSettingsBase.Synchronized(new Settings())));
        
        public static Settings Default {
            get {
                return defaultInstance;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("5000")]
        public ushort PresenceInterval {
            get {
                return ((ushort)(this["PresenceInterval"]));
            }
            set {
                this["PresenceInterval"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("30000")]
        public ushort ActivityTimeOut {
            get {
                return ((ushort)(this["ActivityTimeOut"]));
            }
            set {
                this["ActivityTimeOut"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("192.168.0.71")]
        public string SactaIpA {
            get {
                return ((string)(this["SactaIpA"]));
            }
            set {
                this["SactaIpA"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("15100")]
        public int ListenPortA {
            get {
                return ((int)(this["ListenPortA"]));
            }
            set {
                this["ListenPortA"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("192.168.1.71")]
        public string SactaIpB {
            get {
                return ((string)(this["SactaIpB"]));
            }
            set {
                this["SactaIpB"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("15100")]
        public int ListenPortB {
            get {
                return ((int)(this["ListenPortB"]));
            }
            set {
                this["ListenPortB"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("192.168.0.212")]
        public string ScvIpA {
            get {
                return ((string)(this["ScvIpA"]));
            }
            set {
                this["ScvIpA"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("19204")]
        public int ScvPortA {
            get {
                return ((int)(this["ScvPortA"]));
            }
            set {
                this["ScvPortA"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("192.168.0.212")]
        public string ScvIpB {
            get {
                return ((string)(this["ScvIpB"]));
            }
            set {
                this["ScvIpB"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("19204")]
        public int ScvPortB {
            get {
                return ((int)(this["ScvPortB"]));
            }
            set {
                this["ScvPortB"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("1")]
        public byte SactaDomain {
            get {
                return ((byte)(this["SactaDomain"]));
            }
            set {
                this["SactaDomain"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("107")]
        public byte SactaCenter {
            get {
                return ((byte)(this["SactaCenter"]));
            }
            set {
                this["SactaCenter"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("110")]
        public ushort SactaGroupUser {
            get {
                return ((ushort)(this["SactaGroupUser"]));
            }
            set {
                this["SactaGroupUser"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("1")]
        public byte ScvDomain {
            get {
                return ((byte)(this["ScvDomain"]));
            }
            set {
                this["ScvDomain"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("107")]
        public byte ScvCenter {
            get {
                return ((byte)(this["ScvCenter"]));
            }
            set {
                this["ScvCenter"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("2")]
        public ushort ProcessorNumber {
            get {
                return ((ushort)(this["ProcessorNumber"]));
            }
            set {
                this["ProcessorNumber"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("111")]
        public ushort SactaSPSIUser {
            get {
                return ((ushort)(this["SactaSPSIUser"]));
            }
            set {
                this["SactaSPSIUser"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("86")]
        public ushort SactaSPVUser {
            get {
                return ((ushort)(this["SactaSPVUser"]));
            }
            set {
                this["SactaSPVUser"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("<?xml version=\"1.0\" encoding=\"utf-16\"?>\r\n<ArrayOfString xmlns:xsi=\"http://www.w3." +
            "org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\r\n  <s" +
            "tring>10</string>\r\n</ArrayOfString>")]
        public global::System.Collections.Specialized.StringCollection ScvUsers {
            get {
                return ((global::System.Collections.Specialized.StringCollection)(this["ScvUsers"]));
            }
            set {
                this["ScvUsers"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("1,1;2,2;3,3;4,4;5,5;6,6;7,7;8,8;9,9;10,10;11,11;12,12;13,13;14,14;15,15;18,15;19;" +
            "15")]
        public string LastSectorization {
            get {
                return ((string)(this["LastSectorization"]));
            }
            set {
                this["LastSectorization"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("True")]
        public bool EnableMulticast {
            get {
                return ((bool)(this["EnableMulticast"]));
            }
            set {
                this["EnableMulticast"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute(@"<?xml version=""1.0"" encoding=""utf-16""?>
<ArrayOfString xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"" xmlns:xsd=""http://www.w3.org/2001/XMLSchema"">
  <string>Sectores Desplegados##1,1;2,2;3,3;4,4;5,5;6,6;7,7;8,8;9,9;10,10;11,11;12,12;13,13;14,14;15,15;18,15;19;15</string>
  <string>Agrupacion 01 (Todos en 1)##1,1;2,1;3,1;4,1;5,1;6,1;7,1;8,1;9,1;10,1;11,1;12,1;13,1;14,1;15,1;18,15;19;15</string>
  <string>Agrupacion 02 (De 2 en 2)##1,1;2,1;3,2;4,2;5,3;6,3;7,4;8,4;9,5;10,5;11,6;12,6;13,7;14,7;15,8;18,8;19;9</string>
  <string>Agrupacion 03 (De 4 en 4)##1,1;2,1;3,1;4,1;5,2;6,2;7,2;8,2;9,3;10,3;11,3;12,3;13,4;14,4;15,4;18,4;19;5</string>
  <string>Sectorizacion Erronea 01 (Faltan Sectores Reales)##1,1;2,2;3,3;4,4;5,5;6,6</string>
  <string>Sectorizacion Erronea 02 (Sectores Repetidos)##1,1;2,2;3,3;4,4;5,5;6,6;7,7;8,8;3,12</string>
  <string>Sectorizacion Erronea 03 (Con Sectores Virtuales)##1,1;2,2;3,3;4,4;5,5;6,6;7,7;8,7</string>
  <string>Sectorizacion Erronea 04 (Con Sectores Inexistentes)##1,1;2,2;3,3;4,4;5,5;6,6;7,7;8,8;10,7</string>
  <string>Sectorizacion Erronea 05 (Con UCS Inexistentes)##1,1;2,2;3,3;4,4;5,5;6,6;7,17;8;8</string>
</ArrayOfString>")]
        public global::System.Collections.Specialized.StringCollection PreSectorizaciones {
            get {
                return ((global::System.Collections.Specialized.StringCollection)(this["PreSectorizaciones"]));
            }
            set {
                this["PreSectorizaciones"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("225.12.101.1")]
        public string SactaMcastA {
            get {
                return ((string)(this["SactaMcastA"]));
            }
            set {
                this["SactaMcastA"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("225.212.101.1")]
        public string SactaMcastB {
            get {
                return ((string)(this["SactaMcastB"]));
            }
            set {
                this["SactaMcastB"] = value;
            }
        }
    }
}
