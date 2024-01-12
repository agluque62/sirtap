using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

using UtilitiesCD40;

/// <summary>
/// Descripción breve de HttpHandlers
/// </summary>
public class HttpTickHandler : IHttpHandler
{
    SessionsControl sessions { get; set; }
    public HttpTickHandler()
    {
        //
        // TODO: Agregar aquí la lógica del constructor
        //
        NLogLogger.Debug<HttpTickHandler>(string.Format("HttpTickHandler created"));
    }

    public bool IsReusable
    {
        get
        {
            return false;
        }
    }

    public void ProcessRequest(HttpContext context)
    {
        sessions = (SessionsControl)context.Application["SessionsControl"];
        if (sessions != null)
        {
            sessions.SetActivity(context.Request);
            context.Response.Write(string.Format("Sesiones activas: {0}", sessions));
        }
        else
        {
            context.Response.Write(string.Format("No existe el control de Sesiones"));

        }

        NLogLogger.Debug<HttpTickHandler>(string.Format("HttpTickHandler => {0}, {1}", context.Request.Url, sessions));
    }


}

public class HttpTickModule : IHttpModule
{
    public void Dispose()
    {
        NLogLogger.Debug<HttpTickModule>("Dispose...");
    }

    public void Init(HttpApplication context)
    {
        NLogLogger.Debug<HttpTickModule>("Init...");
    }
}
