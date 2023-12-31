﻿#define _WEBLOGIN_
using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Globalization;
using System.Net;

using System.Text.RegularExpressions;

using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using Newtonsoft.Json.Linq;

using U5kBaseDatos;
using Utilities;
using NucleoGeneric;
using System.Runtime.Remoting.Contexts;

namespace U5kManServer.WebAppServer
{
    class U5kManWebApp : BaseCode /*WebServerBase*/
    {
        IHttpServer _httpServer;
        public U5kManWebApp(IHttpServer httpServer=null) 
        {
            //_httpServer = httpServer ?? new HttpServer();
            _httpServer = httpServer ?? new HttpsServer() { httpOnly = Properties.u5kManServer.Default.https == false };
        }
        public void Start(int port=8090)
        {
            Dictionary<string, wasRestCallBack> cfg = new Dictionary<string, wasRestCallBack>()
                {
                    {"/listinci",restListInci},     // GET & POST
                    {"/std",restStd},               // GET
                    {"/cwp",restCwps},              // GET
                    {"/cwp/*/version",restCwpVersion},              // GET
                    {"/exteq",restExtEqu},          // GET
                    {"/pbxab",restPabx},            // GET
                    {"/gws",restGws},               // GET
                    {"/gws/*",restGwData},          // GET /gws/name & POST /gws/name {cmd: (getVersion, chgPR)}
                    {"/db/operadores",restDbCwps},  // GET
                    {"/db/pasarelas",restDbGws},    // GET
                    {"/db/mnitems",restDbMNItems},    // GET
                    {"/db/incidencias",restDbInci}, // GET & POST
                    {"/db/historicos",restDbHist},  // POST para enviar el Filtro.
                    {"/db/estadistica",restDbEstadistica},  // POST para enviar el Filtro.
                    {"/db/systemusers",restDbSystemUsers},  // 
                    {"/options",restOptions},               // GET & POST
                    {"/snmpopts",restSnmpOptions},               // GET & POST
                    {"/rdsessions",restRdSessions},         // GET
                    {"/gestormn",restRdMNMan},         // GET
                    {"/rdhf",restHFTxData},        // GET
                    {"/rddata", restRadioData },    // GET
                    {"/rd11", restRadio11Control }, // POST
                    {"/sacta", restSacta},              // GET & POST
                    {"/sacta/*", restSacta},              // GET & POST
                    {"/extatssest",restExtAtsDest},
                    {"/versiones",restVersiones},
                    {"/allhard", restAllHard},
                    {"/tifxinfo", restTifxInfo},
                    {"/logs", restLogs},
                    {"/logs/*", restLogs},
                    {"/reset",(context, sb, gdt)=>
                        {
                            if (context.Request.HttpMethod == "POST")
                            {
                                U5kGenericos.ResetService = true;
                                RecordManualAction("Reset Modulo");     // todo. Multiidioma...            
                            }
                            else
                            {
                                context.Response.StatusCode = 404;
                                sb.Append(JsonConvert.SerializeObject( new{ res="Operacion no Soportada"} ));
                            }
                        }
                    },
                {"/alive", RestAlive },
                {"/logout", RestLogout }
            };
            List<string> SecureUris = new List<string>()
            {
                "/styles/bootstrap/bootstrap.min.css",
                "/styles/uv5ki-styles.css",
                "/scripts/jquery/jquery-2.1.3.min.js",
                "/scripts/datatables.min.js",
                "/simulate/listinci.json",
                "/listinci",
                "/images/corporativo-a.png",
                "/favicon.ico",
                "/images/corporativo-a.png",
                "/db/systemusers"
            };
            /** Rutina a la que llama el servidor base para autentificar un usuario */
            _httpServer.AuthenticateUser = (data, response) =>
            {
                /** Control de Disable */
                if (Enable)
                {

                    /** 'namecontroluser'=user&'namecontrolpwd'=pwd */
                    var items = data.Split('&')
                            .Where(x => !string.IsNullOrEmpty(x))
                            .Select(x => x.Split('='))
                            .Where(x => x[1] != "")
                            .ToDictionary(x => x[0], x => x[1]);

                    if (items.Keys.Contains("username") && items.Keys.Contains("password"))
                    {
                        var user = items["username"];
                        var pass = items["password"];
                        U5kBdtService.SystemUserInfo loggeduser = null;
                        // Obtengo la Info de usuario
                        // RM#7287. Al testear el LOGIN. El usuario debe ser case insensitive
                        GlobalServices.GetWriteAccess((gdt) => { loggeduser = gdt.SystemUsers.Where(u => u.id.ToLower() == user.ToLower() && u.pwd == pass).FirstOrDefault(); }, false);
                        if (loggeduser == null)
                        {
                            response(false, "Usuario o password incorrecta", null);
                            return;
                        }
                        response(true, loggeduser.id, loggeduser.ProfileId);
                    }
                    else
                    {
                        response(false, "No ha introducido usuario o password", null);
                    }
                }
                else
                {
                    response(false, $"Servicio Web Inhabilitado ({DisableCause}).", null);
                }
            };
            try
            {
                _sync_server.Start(Properties.u5kManServer.Default.MiDireccionIP,
                    Properties.u5kManServer.Default.MainStandByMcastAdd,
                    Properties.u5kManServer.Default.SyncserverPort);
            }
            catch (Exception x)
            {
                LogException<U5kManWebApp>("Arrancando SyncServer", x);
            }
            try
            {
                _httpServer.Start(port, new CfgServer()
                {
                    DefaultDir = "/appweb",
                    DefaultUrl = "/index.html",
                    LoginUrl = "/login.html",
                    LogoutUrl = "/logout",
                    LoginErrorTag = "<div id='result'>",
                    HtmlEncode = false,
                    //SessionDuration = SessionDuration,
                    SecureUris = SecureUris,
                    CfgRest = cfg
                });
            }
            catch (Exception x)
            {
                LogException<U5kManWebApp>("Arrancando HttpServer", x);
            }
        }
        public void Stop()
        {
            try
            {
                _httpServer.Stop();
                _sync_server.Stop();
            }
            catch (Exception x)
            {
                LogException<U5kManWebApp>("", x);
            }
        }
        public void EnableDisable(bool enable, string cause = "")
        {
            _httpServer.IsEnabled = enable;
            Enable = enable;
            DisableCause = cause;
        }

        bool Enable { get; set; } = true;
        string DisableCause { get; set; } = "";

        protected void RestLogout(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            context.Response.ContentType = "application/json";
            if (context.Request.HttpMethod == "POST")
            {
                _httpServer.Logout(context);
                context.Response.Redirect("/login.html");
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(JsonHelper.ToString(new { res = context.Request.HttpMethod + ": Metodo No Permitido" }, false));
            }
        }
        protected void RestAlive(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            context.Response.ContentType = "application/json";
            if (context.Request.HttpMethod == "GET")
            {
                context.Response.StatusCode = 200;
                sb.Append(JsonHelper.ToString(new { res = "OK" }, false));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(JsonHelper.ToString(new { res = context.Request.HttpMethod + ": Metodo No Permitido" }, false));
            }
        }
        protected void RestLogs(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            context.Response.ContentType = "application/json";
            if (context.Request.HttpMethod == "GET")
            {
                context.Response.StatusCode = 200;
                // TODO. Leer los Logs y Añadir 
                // ReadLog((logs) => { sb.Append(JsonConvert.SerializeObject(logs, Formatting.Indented)); });
                sb.Append(JsonHelper.ToString(new { res = "Contenido del Logs" }, false));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(JsonHelper.ToString(new { res = context.Request.HttpMethod + ": Metodo No Permitido" }, false));
            }
        }
        protected void restListInci(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADInci>(new U5kManWADInci(true) { }));
            }
            else if (context.Request.HttpMethod == "POST")
            {
                using (var reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
                {
                    string strData = reader.ReadToEnd();
                    U5kManWADInci.Reconoce(strData);
                }
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = idiomas.strings.WAP_MSG_001 /* "OK" */}));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restStd(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                string user = context.Request.QueryString["user"];
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADStd>(new U5kManWADStd(gdt, user, true) { }));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restCwps(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADCwps>(new U5kManWADCwps(gdt, true) { }));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restAllHard(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(SafeExecute<string>("restAllHard", () => U5kManWebAppData.JSerialize<U5kManAllhard>(new U5kManAllhard(gdt) { }), DefaultStringObject));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restCwpVersion(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                string name = context.Request.Url.LocalPath.Split('/')[2];
#if STD_ACCESS_V0
                stdPos top = U5kManService._std.stdpos.Where(t => t.name == name).FirstOrDefault();
#else
                stdPos top = gdt.STDTOPS.Where(t => t.name == name).FirstOrDefault();
#endif
                if (top == null)
                {
                    context.Response.StatusCode = 404;
                    sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = idiomas.strings.TOP_NoExiste + ", " + name }));
                }
                else
                {
                    sb.Append(top.sw_version);
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restGws(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADGws>(new U5kManWADGws(gdt, true) { }));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restGwData(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            string name = context.Request.Url.LocalPath.Split('/')[2];
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADGwData>(new U5kManWADGwData(gdt, name, true) { }));
            }
            else if (context.Request.HttpMethod == "POST")
            {
                using (var reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
                {
                    string strData = reader.ReadToEnd();
                    var cmdType = new { cmd = "" };
                    var cmd = JsonConvert.DeserializeAnonymousType(strData, cmdType);
                    if (cmd != null && cmd.cmd == "chgPR")
                    {
                        U5kManWADGwData.MainStandByChange(name);
                        /** Generar Historico de la actuacion... */
                        RecordManualAction("Cambio P/R en GW " + name);     // todo. Multiidioma...
                    }
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restExtEqu(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADExtEqu>(new U5kManWADExtEqu(gdt, true) { }));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restExtAtsDest(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(SafeExecute<string>("restExtAtsDest", () => U5kManWebAppData.JSerialize<U5kManWADExtAtsDst>(new U5kManWADExtAtsDst(gdt, true) { }), DefaultStringObject));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restPabx(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADPbx>(new U5kManWADPbx(gdt, true) { }));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restDbCwps(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADDbCwps>(new U5kManWADDbCwps(gdt, true) { }));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restDbGws(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADDbGws>(new U5kManWADDbGws(gdt, true) { }));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restDbMNItems(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADDbMNItems>(new U5kManWADDbMNItems(true) { }));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restDbInci(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADDbInci>(new U5kManWADDbInci(true) { }));
            }
            else if (context.Request.HttpMethod == "POST")
            {
                using (var reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
                {
                    string strData = reader.ReadToEnd();
                    U5kManWADDbInci incis = U5kManWebAppData.JDeserialize<U5kManWADDbInci>(strData);

                    /** Sincronizar el otro servidor */
                    _sync_server.Sync(cmdSync.Incidencias, strData);

                    incis.Save();
                    RecordManualAction("Modificacion de Perfil de Incidencias");   // todo. Multi-Idioma.
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restDbHist(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "POST")
            {
                using (var reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
                {
                    string strData = reader.ReadToEnd();
                    sb.Append(U5kManWebAppData.JSerialize<U5kManWADDbHist>(new U5kManWADDbHist(strData) { }));
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restDbEstadistica(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "POST")
            {
                using (var reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
                {
                    string strData = reader.ReadToEnd();
                    sb.Append(U5kManWebAppData.JSerialize<U5kManWADDbEstadistica>(new U5kManWADDbEstadistica(strData) { }));
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restDbSystemUsers(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(SafeExecute<string>("restDbSystemUsers", () => JsonConvert.SerializeObject(gdt.SystemUsers), DefaultStringObject));
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restOptions(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADOptions>(new U5kManWADOptions(gdt, true) {}));
            }
            else if (context.Request.HttpMethod == "POST")
            {
                using (var reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
                {
                    SafeExecute("restOptions POST", () =>
                    {
                        string strData = reader.ReadToEnd();
                        U5kManWADOptions opts = U5kManWebAppData.JDeserialize<U5kManWADOptions>(strData);
                        /** Sincronizar el otro servidor */
                        _sync_server.Sync(cmdSync.Opciones, strData);

                        opts.Save();
                        /** Generar Historico de la actuacion...*/
                        RecordManualAction("Modificacion de opciones de Aplicacion");   // todo. Multi-Idioma.
                    });
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restSnmpOptions(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADSnmpOptions>(new U5kManWADSnmpOptions(true) { }));
            }
            else if (context.Request.HttpMethod == "POST")
            {
                using (var reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
                {
                    SafeExecute("restSnmpOptions POST", () =>
                    {
                        string strData = reader.ReadToEnd();
                        U5kManWADSnmpOptions opts = U5kManWebAppData.JDeserialize<U5kManWADSnmpOptions>(strData);
                        /** Sincronizar el otro servidor */
                        _sync_server.Sync(cmdSync.OpcionesSnmp, strData);
                        opts.Save();
                        /** TODO. Generar Historico de la Actuacion */
                        RecordManualAction("Modificacion de opciones de SNMP");   // todo. Multi-Idioma.
                    });
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restRdSessions(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
#if _HAY_NODEBOX__
                sb.Append(JsonConvert.SerializeObject(U5kManService._sessions_data));
#else
                sb.Append(SafeExecute<string>("restRdSessions", () => Services.CentralServicesMonitor.Monitor.RadioSessionsString, DefaultStringObject));
#endif
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restRdMNMan(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
#if _HAY_NODEBOX__
                sb.Append(JsonConvert.SerializeObject(U5kManService._MNMan_data));
#else
                sb.Append(SafeExecute<string>("restRdMNMan", () => Services.CentralServicesMonitor.Monitor.RadioMNDataString, DefaultStringObject));
#endif
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restRadioData(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
#if DEBUG1
                var data = File.ReadAllText(".\\appweb\\simulate\\rddata.json");
                sb.Append(data);
#else
                SafeExecute("restRadioData", () =>
                {
                    Services.CentralServicesMonitor.Monitor.GetRadioData((data) =>
                    {
                        var strData = U5kManWebAppData.JSerialize(data);
                        sb.Append(strData);
                    });
                });
#endif
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize(new { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restRadio11Control(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "POST")
            {
                /** Payload { id: "", ... }*/
                using (var reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
                {
                    SafeExecute("restRadio11Control", () =>
                    {
                        var data = JsonConvert.DeserializeObject(reader.ReadToEnd()) as JObject;
                        if (JsonHelper.JObjectPropertyExist(data, "id") && JsonHelper.JObjectPropertyExist(data, "command"))
                        {
                            // var idEquipo = (string)data["id"];
                            Services.CentralServicesMonitor.Monitor.RdUnoMasUnoCommand(data, (success, msg) =>
                            {
                                if (success)
                                {
                                    context.Response.StatusCode = 200;
                                    sb.Append(JsonConvert.SerializeObject(new { res = "Operacion Realizada." }));
                                }
                                else
                                {
                                    context.Response.StatusCode = 500;
                                    sb.Append(JsonConvert.SerializeObject(new { res = "Internal Error: " + msg }));
                                }
                            });
                        }
                        else
                        {
                            context.Response.StatusCode = 400;
                            sb.Append(JsonConvert.SerializeObject(new { res = "Bad Request..." }));
                        }
                    });
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize(new { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restHFTxData(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
#if _HAY_NODEBOX__
                sb.Append(JsonConvert.SerializeObject(U5kManService._txhf_data));
#else
                sb.Append(SafeExecute<string>("restHFTxData", () => Services.CentralServicesMonitor.Monitor.HFRadioDataString, DefaultStringObject));
#endif
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restTifxInfo(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
#if _HAY_NODEBOX__
                sb.Append(U5kManService._ps_data);
#else
                sb.Append(SafeExecute<string>("restTifxInfo", () => Services.CentralServicesMonitor.Monitor.PresenceDataString, DefaultStringObject));
#endif
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restSacta(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
#if !_SACTA_API_V1_
            if (context.Request.HttpMethod == "GET")
            {
#if DEBUG
                if (DebuggingHelper.Simulating)
                {
                    var sacta_config = new
                    {
                        TickPresencia = 5000,
                        TimeoutPresencia = 30000,
                        sacta = new
                        {
                            Domain = 1,
                            Center = 107,
                            GrpUser = 110,
                            SpiUsers = "111,112,113,114,7286,7287,7288,7289,15000",
                            SpvUsers = "86,87,88,89,7266,7267,7268,7269,34000",
                            lan1 = new
                            {
                                ipmask = "192.168.0.71",
                                mcast = "225.12.101.1",
                                udpport = 19204
                            },
                            lan2 = new
                            {
                                ipmask = "192.168.1.71",
                                mcast = "225.212.101.1",
                                udpport = 19204
                            }
                        },
                        scv = new
                        {
                            Domain = 1,
                            Center = 107,
                            User = 10,
                            Interfaz = "192.168.0.212",
                            udpport = 15100,
                            Ignore = "305"
                        }
                    };
                    sb.Append(JsonHelper.ToString(sacta_config));
                }
                else
#endif
                {
                    ServicioInterfazSacta sacta_srv = new ServicioInterfazSacta(U5kManServer.Properties.u5kManServer.Default.MySqlServer);
                    sb.Append(SafeExecute<string>("restSacta GET", () => sacta_srv.SactaConfGet(), DefaultStringObject));
                }
            }
            else if (context.Request.HttpMethod == "POST")
            {
                string[] UrlFields = context.Request.Url.LocalPath.Split('/');
                if (UrlFields.Length > 2)
                {
                    string activar = UrlFields[2];
                    U5KStdGeneral stdg = gdt.STDG;
                    if (activar == "true")
                    {
                        stdg.SactaServiceEnabled = true;
                        Task.Factory.StartNew(() =>
                        {
                            SafeExecute("restSacta Activate", () =>
                            {
                                ServicioInterfazSacta sacta_srv = new ServicioInterfazSacta(U5kManServer.Properties.u5kManServer.Default.MySqlServer);
                                sacta_srv.StartSacta();
                                GlobalServices.GetWriteAccess((data) =>
                                {
                                    U5kManService._main.EstadoSacta(16, stdg);
                                });
                                /** TODO. Generar Historico de Actuacion */
                                RecordManualAction("Activacion Manual de Servicio SACTA");   // todo. Multi-Idioma.
                            });
                        });
                    }
                    else if (activar == "false")
                    {
                        stdg.SactaServiceEnabled = false;
                        Task.Factory.StartNew(() =>
                        {
                            SafeExecute("restSacta Deactivate", () =>
                            {
                                ServicioInterfazSacta sacta_srv = new ServicioInterfazSacta(U5kManServer.Properties.u5kManServer.Default.MySqlServer);
                                sacta_srv.EndSacta();
                                GlobalServices.GetWriteAccess((data) =>
                                {
                                    U5kManService._main.EstadoSacta(0, stdg);
                                });
                                /** TODO. Generar Historico de Actuacion */
                                RecordManualAction("Desactivacion Manual de Servicio SACTA");   // todo. Multi-Idioma.
                            });
                        });
                    }
                    else
                    {
                        context.Response.StatusCode = 400;
                        sb.Append(JsonConvert.SerializeObject(new { res = "Codigo no implementado: " + activar }));
                    }
                }
                else
                {
                    ServicioInterfazSacta sacta_srv = new ServicioInterfazSacta(U5kManServer.Properties.u5kManServer.Default.MySqlServer);
                    using (var reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
                    {
                        SafeExecute("restSacta Config.", () =>
                        {
                            string strData = reader.ReadToEnd();
                            sacta_srv.SactaConfSet(strData);

                            /** Sincronizar el otro servidor */
                            _sync_server.Sync(cmdSync.OpcionesSacta, strData);

                            sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = idiomas.strings.WAP_MSG_001 /* "OK" */}));
                            RecordManualAction("Cambio de opciones de Servicio SACTA");   // todo. Multi-Idioma.
                        });
                    }
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
#else
            ServicioInterfazSacta sacta_srv = new ServicioInterfazSacta(U5kManServer.Properties.u5kManServer.Default.MySqlServer);
            if (context.Request.HttpMethod == "GET")
            {
                sb.Append(sacta_srv.SactaConfGet());
            }
            else if (context.Request.HttpMethod == "POST")
            {
                string[] UrlFields = context.Request.Url.LocalPath.Split('/');
                if (UrlFields.Length > 2)
                {
                    string activar = UrlFields[2];
                    U5KStdGeneral stdg = gdt.STDG;
                    if (activar == "true")
                    {
                        stdg.SactaServiceEnabled = true;
                        sacta_srv.StartSacta();
                        RecordManualAction("Activacion Manual de Servicio SACTA");   // todo. Multi-Idioma.
                        sb.Append(JsonConvert.SerializeObject(new { res = $"Estado SACTA {sacta_srv.GetEstadoSacta()} "}));

                        //Task.Factory.StartNew(() =>
                        //{
                        //    sacta_srv.StartSacta();
                        //    GlobalServices.GetWriteAccess((data) =>
                        //    {
                        //        U5kManService._main.EstadoSacta(16, stdg);
                        //    });
                        //    /** TODO. Generar Historico de Actuacion */
                        //    RecordManualAction("Activacion Manual de Servicio SACTA");   // todo. Multi-Idioma.
                        //});
                    }
                    else if (activar == "false")
                    {
                        stdg.SactaServiceEnabled = false;
                        sacta_srv.EndSacta();
                        RecordManualAction("Desactivacion Manual de Servicio SACTA");   // todo. Multi-Idioma.
                        sb.Append(JsonConvert.SerializeObject(new { res = $"Estado SACTA {sacta_srv.GetEstadoSacta()} " }));

                        //Task.Factory.StartNew(() =>
                        //{

                        //    //ServicioInterfazSacta sacta_srv = new ServicioInterfazSacta(U5kManServer.Properties.u5kManServer.Default.MySqlServer);
                        //    sacta_srv.StartSacta();
                        //    GlobalServices.GetWriteAccess((data) =>
                        //    {
                        //        U5kManService._main.EstadoSacta(0, stdg);
                        //    });
                        //    /** TODO. Generar Historico de Actuacion */
                        //    RecordManualAction("Desactivacion Manual de Servicio SACTA");   // todo. Multi-Idioma.
                        //});
                    }
                    else
                    {
                        context.Response.StatusCode = 400;
                        sb.Append(JsonConvert.SerializeObject(new { res = "Codigo no implementado: " + activar }));
                    }
                }
                else
                {
                    using (var reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
                    {
                        string strData = reader.ReadToEnd();
                        sacta_srv.SactaConfSet(strData);

                        /** Sincronizar el otro servidor */
                        _sync_server.Sync(cmdSync.OpcionesSacta, strData);

                        sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = idiomas.strings.WAP_MSG_001 /* "OK" */}));
                        RecordManualAction("Cambio de opciones de Servicio SACTA");   // todo. Multi-Idioma.
                    }
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
#endif
            }
        protected void restVersiones(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                SafeExecute("", () =>
                {
                    /** 20180327. Se obtinen las versiones al pedirlas no periodicamente */
                    U5KStdGeneral stdg = gdt.STDG;
                    GetVersionDetails(stdg);

                    /** */

                    List<Utilities.VersionDetails.VersionData> versiones = new List<Utilities.VersionDetails.VersionData>()
                    { JsonConvert.DeserializeObject<Utilities.VersionDetails.VersionData>(stdg.stdServ1.jversion),
                      JsonConvert.DeserializeObject<Utilities.VersionDetails.VersionData>(stdg.stdServ2.jversion) };
                    sb.Append(JsonConvert.SerializeObject(versiones));
                });
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        protected void restLogs(HttpListenerContext context, StringBuilder sb, U5kManStdData gdt)
        {
            if (context.Request.HttpMethod == "GET")
            {
                NLog.Targets.FileTarget fileTarget = (NLog.Targets.FileTarget)NLog.LogManager.Configuration.FindTargetByName("csvfile");
                if (fileTarget != null)
                {
                    var parts = context.Request.Url.LocalPath.Split('/');
                    int filenumber = parts.Length < 3 ? -1 : Int32.Parse(parts[2]);
                    string filename = filenumber < 0 ? fileTarget.FileName.Render(new NLog.LogEventInfo()) :
                        fileTarget.ArchiveFileName.Render(new NLog.LogEventInfo()).Replace("{#####}", filenumber.ToString("D5")
                        );

                    if (File.Exists(filename))
                    {
                        sb.Append(JsonConvert.SerializeObject(new
                        {
                            file = filename,
                            data = File.ReadAllLines(filename)
                        }));
                    }
                    else
                    {
                        sb.Append(JsonConvert.SerializeObject(new
                        {
                            file = filename,
                            error = "No se encuentra el fichero"
                        }));
                    }
                }
                else
                {
                    sb.Append(JsonConvert.SerializeObject(new
                    {
                        target = "csvfile",
                        error = "No se encuentra el target con este nombre"
                    }));
                }
            }
            else
            {
                context.Response.StatusCode = 404;
                sb.Append(U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = context.Request.HttpMethod + idiomas.strings.WAP_MSG_002 /*": Metodo No Permitido"*/ }));
            }
        }
        void GetVersionDetails(U5KStdGeneral stdg)
        {
#if DEBUG
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
#endif
            try
            {
                if (U5kManService.cfgSettings/*Properties.u5kManServer.Default*/.ServidorDual == true)
                {
                    string MyName = System.Environment.MachineName;
                    //if (MyName == stdg.stdServ1.name)
                    if (stdg.stdServ1.name.StartsWith(MyName))
                    {
                        stdg.stdServ1.jversion = VersionDetails.SwVersions.ToString();
                        _sync_server.QueryVersionData();
                    }
                    //else if (MyName == stdg.stdServ2.name)
                    else if (stdg.stdServ2.name.StartsWith(MyName))
                    {
                        stdg.stdServ2.jversion = VersionDetails.SwVersions.ToString();
                        _sync_server.QueryVersionData();
                    }
                    else
                    {
                        stdg.stdServ1.jversion = VersionDetails.SwVersions.ToString();
                        stdg.stdServ2.jversion = "";
                    }
                }
                else
                {
                    stdg.stdServ1.jversion = VersionDetails.SwVersions.ToString();
                    stdg.stdServ2.jversion = "";
                }
            }
            catch (Exception x)
            {
                LogException<U5kServiceMain>( "", x);
            }
            finally
            {
            }
#if DEBUG
            sw.Stop();
#endif
        }
        protected void RecordManualAction(string action)
        {
            RecordEvent<U5kManWebApp>(DateTime.Now, 
                U5kBaseDatos.eIncidencias.IGRL_U5KI_SERVICE_INFO, 
                U5kBaseDatos.eTiposInci.TEH_SISTEMA, "WEB",        
                Params("WebApp", action));
        }
        string DefaultStringObject => U5kManWebAppData.JSerialize<U5kManWADResultado>(new U5kManWADResultado() { res = "Error Interno. Ver Log" });
        public static MainStandbySyncServer _sync_server = new MainStandbySyncServer();

    }

}
