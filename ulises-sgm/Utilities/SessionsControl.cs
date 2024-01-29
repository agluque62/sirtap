using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Web;
using System.Web.Security;
using System.IO;

namespace UtilitiesCD40
{
    public class SessionsControl
    {
        public class Session
        {
            public string Key { get; set; } //= "";
            public string UserId { get; set; }// = "";
            public DateTime Expires { get; set; }
            public DateTime LastActivityTime { get; set; }
            public TimeSpan Time2Renew { get; set; }
            public DateTime ExpiresSession { get; set; } // JOI 20230216
            public override string ToString()
            {
                String result = String.Format("User: {0}, Expires: {1}, LastActivity: {2}", 
                                                UserId, 
                                                Expires.ToString(@"hh\:mm\:ss"), 
                                                LastActivityTime.ToString(@"hh\:mm\:ss")); 
                
                return result;
            }
        }
        public int MaxSessions { get; set; }
        public TimeSpan IdleTime { get; set; }
        public SessionsControl(int sessions = 2)
        {
            MaxSessions = sessions;
        }

        public void InitializeVar()
        {
            Locker = new object();
            Sessions = new List<Session>();
            Expired = new List<Session>();
            Inactives = new List<Session>();
            RandomGenerator = new Random((int)DateTime.Now.Ticks);
        }

        public void Logon(TimeSpan Duration, string userid, string userprofile,  Action<bool, HttpCookie, string> respond)
        {
            string msg;
            Cleanup();
            IdleTime = TimeSpan.FromSeconds(60);
            // Adaptado a las Cookies de la Aplicacion de CFG.
            msg = String.Format("Getting access for {0}, Duration: {1}. {2} Sessions Active.", userid, Duration, SessionsCount());
            NLogLogger.Debug<SessionsControl>(msg);
            GetAccess(userid, (result, error) =>
            {
                if (result == true)
                {
                    FormsAuthenticationTicket ticket = new FormsAuthenticationTicket(1, userid,
                                                                                    System.DateTime.Now,
                                                                                    System.DateTime.Now + Duration,
                                                                                    false,
                                                                                    userprofile,
                                                                                    FormsAuthentication.FormsCookiePath);
                    // Encrypt the ticket.
                    string encTicket = FormsAuthentication.Encrypt(ticket);
                    lock (Locker)
                    {
                        Session ses = new Session();
                        ses.Key = userid;
                        ses.Expires = DateTime.Now + Duration;
                        ses.ExpiresSession = ses.Expires;// JOI 20230216
                        ses.LastActivityTime = DateTime.Now;
                        ses.Time2Renew = Duration;
                        Sessions.Add(ses);
                        msg = String.Format("Logon => {0};  Expires: {1}; LastActivity {2}; Time2Renew {3}; Sessions Active: {4}", 
                            ses.Key, 
                            ses.Expires, 
                            ses.LastActivityTime, 
                            ses.Time2Renew, 
                            SessionsCount());
                        NLogLogger.Debug<SessionsControl>(msg);
                    }
                    respond(true, new HttpCookie(FormsAuthentication.FormsCookieName, encTicket) /*{ Expires = ticket.Expiration }*/, error);
                }
                else
                {
                    respond(false, null, error);
                }
            });
        }
        public void Logout(HttpRequest request, HttpResponse response, Action<string> user)
        {
            Cleanup();
            if (Sessions != null)
            {
                var keyAccess = KeyCookie(request);
                if (keyAccess != null)// default
                {
                    lock (Locker)
                    {
                        var Ticket = FormsAuthentication.Decrypt(keyAccess);
                        Session ses = Sessions.Where(s => s.Key == Ticket.Name).Select(s => s).FirstOrDefault();
                        if (ses == null)
                        {
                            NLogLogger.Debug<SessionsControl>("Logout de una sesión con valor null");
                            return;// 
                        }
                        var userId = ses.Key;
                        user(userId);
                        // Limpia la tabla...
                        Sessions = Sessions.Where(s => s.Key != Ticket.Name).Select(s => s).ToList();
                        string msg = String.Format("Logout => {0}. {1} Sessions Active.", userId, SessionsCount());
                        NLogLogger.Debug<SessionsControl>(msg);
                    }
                }
                FormsAuthentication.Initialize();
                response.Cookies.Remove(FormsAuthentication.FormsCookieName);
                response.Cookies.Add(new HttpCookie(FormsAuthentication.FormsCookieName, "")
                {
                    Path = FormsAuthentication.FormsCookiePath,
                    Secure = FormsAuthentication.RequireSSL,
                    Expires = new DateTime(0x7cf, 10, 12)
                });
            }
        }

        public void Renew(HttpRequest request, HttpResponse response)
        {
             Cleanup();
             if (Sessions != null)
             {
                 var keyAccess = KeyCookie(request);
                 if (keyAccess != null)
                 {
                     lock (Locker)
                     {
                         var oldTicket = FormsAuthentication.Decrypt(keyAccess);
                         Session ses = Sessions.Where(s => s.Key == oldTicket.Name).Select(s => s).FirstOrDefault();
                         if (ses != null)// default
                         {
                             // Aplicamos Logout forzado a pantalla de Controlador pasado el tiempo de sesión con o sin actividad
                             if ((ses.ExpiresSession <= DateTime.Now) && (oldTicket.UserData == "0"))
                             {
                                 Logout(request, response, (user) => { });
                                 response.Redirect("~/Login.aspx", false);
                                 return;
                             }
                             var newTicket = FormsAuthentication.RenewTicketIfOld(oldTicket);
                             response.Cookies.Remove(FormsAuthentication.FormsCookieName);
                             response.Cookies.Add(new HttpCookie(FormsAuthentication.FormsCookieName, FormsAuthentication.Encrypt(newTicket)));
                             ses.Expires = newTicket.Expiration;
                             ses.LastActivityTime = DateTime.Now;
                             string msg = String.Format("Renew Ticket for {0}, Expires on {1}", oldTicket.Name, (newTicket.Expiration - DateTime.Now));
                             NLogLogger.Debug<SessionsControl>(msg);
                         }
                         else
                         {
                             // El ticket no está en sesiones. Puede ser que sea de una sesion anterior.
                             Logout(request, response, (user) => { });
                         }
                     }
                 }
             }
        }

        public void SetActivity(HttpRequest request)
        {
            Cleanup();
            // TODO. Adaptar a las Cookies de la Aplicacion de CFG.
            var keyAccess = KeyCookie(request);
            if (keyAccess != null)// default
            {
                lock (Locker)
                {
                    var Ticket = FormsAuthentication.Decrypt(keyAccess);
                    Session ses = Sessions.Where(s => s.Key == Ticket.Name).Select(s => s).FirstOrDefault();
                    if (ses != null)
                    {
                        ses.LastActivityTime = DateTime.Now;
                    }
                }
            } 
        }
        public void Tick4Idle() {Cleanup();}

        public override string ToString()
        {
            String result = String.Format("Sesiones: {0}", String.Join("; ", Sessions.Select(s => s.ToString()))); 
            return result;
        }

        void Cleanup()
        {
            string msg = "";
            Expired.Clear();
            Inactives.Clear();

            if (Sessions != null)
            {
                lock (Locker)
                {
                    Expired = Sessions.Where(s => s.Expires <= DateTime.Now).Select(s => s).ToList();
                    /** Limpia las Sesiones que han expirado */
                    Sessions = Sessions.Where(s => s.Expires > DateTime.Now).Select(s => s).ToList();

                    Inactives = Sessions.Where(s => s.LastActivityTime + IdleTime <= DateTime.Now).Select(s => s).ToList();
                    /** Limpia las Sesiones Inactivas */
                    Sessions = Sessions.Where(s => s.LastActivityTime + IdleTime > DateTime.Now).Select(s => s).ToList();
                }
                Task.Run(() =>
                {
                    // TODO. Generar los registros históricos.
                    Expired.ForEach((s) =>
                    {
                        msg = String.Format("La Sesion de {0} ha expirado", s.UserId);                       
                        //RecordEvent<WebServerBase>(DateTime.Now, eIncidencias.IGRL_NBXMNG_EVENT, eTiposInci.TEH_SISTEMA, "MTTO",
                        //    new object[] { msg, "", "", "", "", "", "", "" });
                        NLogLogger.Debug<SessionsControl>(msg);
                    });
                    Inactives.ForEach((s) =>
                    {
                        msg = String.Format("La Sesion de {0} se ha cancelado por inactividad.", s.UserId);          
                        //RecordEvent<WebServerBase>(DateTime.Now, eIncidencias.IGRL_NBXMNG_EVENT, eTiposInci.TEH_SISTEMA, "MTTO",
                        //    new object[] { msg, "", "", "", "", "", "", "" });
                        NLogLogger.Debug<SessionsControl>(msg);
                    });
                });
            }
        }

        int SessionsCount()
        {
            lock (Locker)
            {
                if (Sessions == null)
                    return 0;
                else
                    return Sessions.Count;
            }
        }

        void GetAccess (string userid, Action<bool, string> response)
        {
            lock (Locker)
            {
                var sesuser = Sessions.Where(s => s.Key == userid).ToList();
                var msg = "";
                if (sesuser.Count != 0)
                    msg = string.Format("El usuario {0} ya tiene una sesion activa.", userid);
                else if (SessionsCount() >= MaxSessions)
                        msg = string.Format("Se ha alcanzado el número maximo de sesiones concurrentes {0}", MaxSessions);
                else 
                     msg = "";
                response(sesuser.Count == 0 && SessionsCount() < MaxSessions, msg);
            }
        }

        string GenetareKey(int length = 16)
        {
            const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

            var randomString = new string(Enumerable.Repeat(chars, length)
                .Select(s => s[RandomGenerator.Next(s.Length)]).ToArray());
            return randomString;
        }

        public string KeyCookie(HttpRequest request)
        {
            var lstCookies = request.Cookies.Keys.Cast<string>().Select(x => request.Cookies[x]).ToList();
            return  lstCookies.Where(c => c.Name == FormsAuthentication.FormsCookieName).Select(c => c.Value).FirstOrDefault();
        }

        private object Locker { get; set; }
        private Random RandomGenerator  { get; set; }
        private List<Session> Sessions  { get; set; }
        private List<Session> Expired   { get; set; }
        private List<Session> Inactives { get; set; }

    }
}
