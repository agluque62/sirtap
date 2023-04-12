using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using CD40.BD;
using CD40.BD.Entidades;

/// <summary>
/// Summary description for Utilidades
/// </summary>
public class Utilidades
{
    public Utilidades()
    {
        //
        // TODO: Add constructor logic here
        //
    }

	public static void ErrorLog(string sPathName, string sErrMsg)
	{
		try
		{
			System.IO.StreamWriter sw = new System.IO.StreamWriter(HttpContext.Current.Server.MapPath("~/") + sPathName, true);

            System.Text.StringBuilder sLogFormat = new System.Text.StringBuilder();
                
            sLogFormat.AppendFormat("[{0} {1}] ",DateTime.Now.ToString("dd/MM/yyyy HH:mm:ss"),sErrMsg) ;

			sw.WriteLine(sLogFormat.ToString());
			sw.Flush();
			sw.Close();

            sLogFormat.Clear();
		}
		catch (System.IO.IOException)
		{
		}
	}
}
