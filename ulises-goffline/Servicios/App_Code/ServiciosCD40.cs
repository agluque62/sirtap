using System;
using System.Data;
using System.Web;
using System.Web.Services;
using System.Web.Services.Protocols;
using System.Xml.Serialization;
using System.Collections.Generic;
using System.Text;
using CD40.BD.Entidades;
using CD40.BD;


/// <summary>
/// 
/// </summary>
public struct DatosControlBackup
{
	public string NomRecursoHistoricos;
	public string NomRecursoIndicadores;

	public uint[] Profundidad;
}

[WebService(Namespace = "http://CD40.es/")]
[WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]

#region XML-Includes
[XmlInclude(typeof(Tablas))]
[XmlInclude(typeof(Operadores))]
[XmlInclude(typeof(HistoricoIncidencias))]
[XmlInclude(typeof(LogProcesos))]
[XmlInclude(typeof(Tipos.ExportaTipoEnumerados))]
[XmlInclude(typeof(Configuraciones))]
#endregion

public partial class ServiciosCD40 : System.Web.Services.WebService
{
    object _Sync = new object();
    private GestorBaseDatos GestorBDCD40;
    private GestorBaseDatos GestorBDCD40ToMantto;
    private MySql.Data.MySqlClient.MySqlConnection MySqlConnectionToCd40;
    private MySql.Data.MySqlClient.MySqlConnection MySqlConnectionToCd40ToMantto;

    static System.Threading.TimerCallback TimerProcedure;
    static System.Threading.Timer TimerPresenciaCD30;

    private static bool EstadoCD30Activo = false;
    private static int Estado1 = 0;
    private static int Estado2 = 2;


    /// <summary>
    /// 
    /// </summary>
    public ServiciosCD40()
    {
        string cadenaConexion;
        System.Configuration.Configuration webConfiguracion = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        if (webConfiguracion.ConnectionStrings.ConnectionStrings.Count > 0)
        {
            cadenaConexion = webConfiguracion.ConnectionStrings.ConnectionStrings["ConexionBaseDatosCD40_Offline"].ToString();
            MySqlConnectionToCd40 = new MySql.Data.MySqlClient.MySqlConnection(cadenaConexion);
            GestorBDCD40 = new GestorBaseDatos(MySqlConnectionToCd40);
        }
    }


    /// <summary>
    /// 
    /// </summary>
    /// <param name="commit"></param>
    /// <returns></returns>
    private string CreaFicheroDump(bool commit)
    {
        string strUsuario = "root";
        string strPwd = "cd40";
        string strDatabaseOrigen = "cd40_trans";
        string strDatabaseDestino = "cd40";
        string strServer = "localhost";
        string s = string.Empty;
        string strTablas = string.Empty;
        int iCodSalida = -1;
        try
        {
            Microsoft.Win32.RegistryKey rk = Microsoft.Win32.Registry.LocalMachine.OpenSubKey("SOFTWARE\\Nucleo\\CD40", false);
            strUsuario = Convert.ToString(rk.GetValue("Usuario"));
            strPwd = Convert.ToString(rk.GetValue("Clave"));
            strDatabaseOrigen = Convert.ToString(rk.GetValue("Base Datos Origen"));
            strDatabaseDestino = Convert.ToString(rk.GetValue("Base Datos Destino"));
            strServer = Convert.ToString(rk.GetValue("Servidor"));
            rk.Close();
        }
        catch (Exception )
        {
            Microsoft.Win32.RegistryKey rk = Microsoft.Win32.Registry.LocalMachine.OpenSubKey("SOFTWARE", true);
            rk = rk.CreateSubKey("Nucleo");
            rk = rk.CreateSubKey("CD40");

            rk.SetValue("Usuario", strUsuario);
            rk.SetValue("Clave", strPwd);
            rk.SetValue("Base Datos Origen", strDatabaseOrigen);
            rk.SetValue("Base Datos Destino", strDatabaseDestino);
            rk.SetValue("Servidor", strServer);
            rk.Close();
        }
        finally
        {
            string proceso = HttpContext.Current.Server.MapPath("~/") + "trans.bat";
            string argumentos = strUsuario + " " +
                                strPwd + " " +
                                strServer + " " +
                                (commit ? strDatabaseOrigen : strDatabaseDestino) + " " +
                                (commit ? strDatabaseDestino : strDatabaseOrigen) + " " +
                                (commit ? strTablas : "");
            //"root jucar localhost cd40_trans cd40 [tabla1 tabla 2 ... ]";


            System.Diagnostics.ProcessStartInfo p = new System.Diagnostics.ProcessStartInfo();
            p.FileName = proceso; p.Arguments = argumentos;


            //MVO. Se ejecuta el bat en la shell, para que se puedan ejecutar varios comandos
            p.UseShellExecute = true;
            p.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;


            //s = pFinal.StandardError.ReadToEnd();

            //MVO: Se registra en el fichero de log, el tipo de operación realizada: se implanta la sectorización 
            //    o se restaura la BD original a partir de la base de datos en explotación
            if (commit)
                Utilidades.ErrorLog("CommitCD40_Log.txt", "Se implanta la sectorización en la BD de explotación.");
            else
                Utilidades.ErrorLog("CommitCD40_Log.txt", "Se restaura la configuración de la BD de explotación");


            Utilidades.ErrorLog("CommitCD40_Log.txt", "  Se ejecuta: " + proceso + " " + p.Arguments);
            Utilidades.ErrorLog("CommitCD40_Log.txt", "  Tablas modificadas:" + strTablas);

            //Se ejecuta el proceso
            System.Diagnostics.Process pFinal = System.Diagnostics.Process.Start(p);

            try
            {
                //Se espera a que finalice el proceso un máximo de 5 minutos 
                if (pFinal.WaitForExit(300000))
                {
                    //El proceso ha acabo correctamente
                    Utilidades.ErrorLog("CommitCD40_Log.txt", "Proceso realizado correctamente. ExitCode: " + pFinal.ExitCode);
                    iCodSalida = pFinal.ExitCode;
                }
                else
                {
                    Utilidades.ErrorLog("CommitCD40_Log.txt", "Time Out excedido");
                }
            }
            catch (Exception ex)
            {
                Utilidades.ErrorLog("CommitCD40_Log.txt", "Excepcion al ejecutar trans.bat. Error:" + ex.Message.ToString());
            }
            finally
            {
                if (pFinal != null)
                {
                    pFinal.Close();
                }
            }

        }

        return (iCodSalida == 0).ToString();
    }

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    [WebMethod(Description = "Restaura la copia de la base de datos sobre la base de datos original")]
    public string Commit()
    {
        string retorno = CreaFicheroDump(true /* commit */);
        GestorBDCD40.ExecuteNonQuery("TRUNCATE TABLE TablasModificadas", null);
        return retorno;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    [WebMethod(Description = "Recupera la última activa como base de datos de gestíón")]
    public string Rollback()
    {
        string retorno = CreaFicheroDump(false /* rollback */);

        GestorBDCD40.ExecuteNonQuery("TRUNCATE TABLE TablasModificadas", null);

        return retorno;
    }


    /// <summary>
    /// 
    /// </summary>
    /// <param name="t"></param>
    /// <returns></returns>
    [WebMethod(Description = "Retorna un DataSet con los registros que cumplan con la clave pasada en <t>")]
    public DataSet DataSetSelectSQL(Tablas t)
    {
        return GestorBDCD40.DataSetSelectSQL(t, null);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="t"></param>
    /// <returns></returns>
    [WebMethod(Description = "Retorna una Tablas[] con los registros que cumplan con la clave pasada en <t>. Si la " +
                            "SELECT no devuelve ningún registro, el array estará vacío")]
    public List<Tablas> ListSelectSQL(Tablas t)
    {
        return GestorBDCD40.ListSelectSQL(t, null);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="t"></param>
    /// <returns></returns>
    [WebMethod(Description = "Retorna el número de registros insertados o un número negativo cuyo valor" +
                            " absoluto indica el código de excepción en caso de error")]
    public int InsertSQL(Tablas t)
    {
        return GestorBDCD40.InsertSQL(t, null);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="t"></param>
    /// <returns></returns>
    [WebMethod(Description = "Retorna el número de registros actualizados o un número negativo cuyo valor" +
                            " absoluto indica el código de excepción en caso de error. No actualiza las claves.")]
    public int UpdateSQL(Tablas t)
    {
        return GestorBDCD40.UpdateSQL(t, null);
    }


    /// <summary>
    /// 
    /// </summary>
    /// <param name="t"></param>
    /// <returns></returns>
    [WebMethod(Description = "Retorna el número de registros eliminados o un número negativo cuyo valor" +
                            " absoluto indica el código de excepción en caso de error.")]
    public int DeleteSQL(Tablas t)
    {
        //AccesoABaseDeDatos a = new AccesoABaseDeDatos();
        return GestorBDCD40.DeleteSQL(t, null);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="id_sistema"></param>
    /// <param name="datosBackup"></param>
    [WebMethod(Description = "Genera los datos de backup para históricos e indicadores")]
    public void GeneraDatosControlBackup(string id_sistema, DatosControlBackup datosBackup)
    {
        MySql.Data.MySqlClient.MySqlTransaction tran = GestorBDCD40.StartTransaction(true);

        try
        {
            string consulta = "DELETE FROM ControlBackup";
            GestorBDCD40.ExecuteNonQuery(consulta, tran);
            //AccesoABaseDeDatos.ExecuteNonQuery(consulta);

            for (int i = 0; i < 8; i++)
            {
                consulta = "INSERT INTO ControlBackup VALUES ('" + id_sistema + "'," +
                                                                i + "," +
                                                                datosBackup.Profundidad[i] + ",'" +
                                                                (i < 4 ? datosBackup.NomRecursoHistoricos : datosBackup.NomRecursoIndicadores) +
                                                                "')";
                GestorBDCD40.ExecuteNonQuery(consulta, tran);
                //AccesoABaseDeDatos.ExecuteNonQuery(consulta);
            }
            GestorBDCD40.Commit(tran);
        }
        catch (MySql.Data.MySqlClient.MySqlException)
        {
            GestorBDCD40.RollBack(tran);
        }
    }



    /// <summary>
    /// 
    /// </summary>
    /// <param name="id_sistema"></param>
    /// <returns></returns>
    [WebMethod(Description = "Genera los datos de backup para históricos e indicadores")]
    public DatosControlBackup RecuperarDatosControlBackup(string id_sistema)
    {
        DatosControlBackup dControl = new DatosControlBackup();
        dControl.Profundidad = new uint[8];
        string consulta = "SELECT * FROM ControlBackup WHERE IdSistema='" + id_sistema + "'";

        //AccesoABaseDeDatos accesoABaseDeDatos = new AccesoABaseDeDatos();
        //DataSet datosControl = accesoABaseDeDatos.GetDataSet(consulta);
        DataSet datosControl = GestorBDCD40.GetDataSet(consulta, null);
        if (datosControl != null)
        {
            foreach (DataRow dr in datosControl.Tables[0].Rows)
            {
                if (dr["TipoBackup"] != System.DBNull.Value)
                {
                    if ((int)dr["TipoBackup"] < 4)
                        dControl.NomRecursoHistoricos = (string)dr["Recurso"];
                    else
                        dControl.NomRecursoIndicadores = (string)dr["Recurso"];

                    dControl.Profundidad[(int)dr["TipoBackup"]] = (uint)dr["Profundidad"];
                }
            }
        }

        return dControl;
    }



    private void CreateEvent(int idIncidencia, string node, string idioma, string sistema, string ip)
    {
        string consulta;
        string desc = "";
        System.Data.DataSet ds = new System.Data.DataSet();


        lock (_Sync)
        {
            try
            {
                switch (idioma)
                {
                    case "en":
                        consulta = string.Format("SELECT incidencia FROM incidencias_ingles WHERE IdIncidencia={0}", idIncidencia);
                        break;
                    case "fr":
                        consulta = string.Format("SELECT incidencia FROM incidencias_frances WHERE IdIncidencia={0}", idIncidencia);
                        break;
                    default:
                        consulta = string.Format("SELECT incidencia FROM incidencias WHERE IdIncidencia={0}", idIncidencia);
                        break;
                }
                ds = GestorBDCD40ToMantto.GetDataSet(consulta, null);
                if (ds.Tables.Count > 0 && ds.Tables[0].Rows.Count > 0)
                {
                    string[] incidencia = ds.Tables[0].Rows[0]["incidencia"].ToString().Split(new char[] { ' ' });

                    // Formatearlo de acuerdo a como lo espera el servidor de mantenimiento
                    desc = string.Format("{0},{1} {2} {3}", idIncidencia, incidencia[0], node, incidencia[1]);

                    System.Configuration.Configuration webConfiguracion = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
                    string toServerMantto = webConfiguracion.AppSettings.Settings["ServerManttoIp"].Value;

                    UtilitiesCD40.GeneraIncidencias.SendTrap(toServerMantto, idIncidencia.ToString(), desc);
                }
            }
            catch (MySql.Data.MySqlClient.MySqlException )
            {
            }
        }
    }

 
    /// <summary>
    /// 
    /// </summary>
    /// <param name="id_sistema"></param>
    /// <param name="id_nucleo"></param>
    /// <param name="id_sectorizacion"></param>
    /// <returns></returns>
    [WebMethod(Description = "Retorna el identificador del esquema de base de datos utilizado en la conexion")]
    public string GetIdEsquema()
    {
        string cadenaConexion = string.Empty;
        string esquema = string.Empty;
        string EntreInicio = "Database=";
        string EntreFin = ";Uid=";

        System.Configuration.Configuration webConfiguracion = System.Web.Configuration.WebConfigurationManager.OpenWebConfiguration("~");
        if (webConfiguracion.ConnectionStrings.ConnectionStrings.Count > 0)
        {
            cadenaConexion = webConfiguracion.ConnectionStrings.ConnectionStrings["ConexionBaseDatosCD40_Offline"].ToString();
            if (cadenaConexion.Contains(EntreInicio) && cadenaConexion.Contains(EntreFin))
            {
                int StartIndex = cadenaConexion.IndexOf(EntreInicio, 0) + EntreInicio.Length;
                int EndIndex = cadenaConexion.IndexOf(EntreFin, StartIndex);
                esquema = cadenaConexion.Substring(StartIndex, EndIndex - StartIndex);
            }
        }
        return esquema;
    }
}

