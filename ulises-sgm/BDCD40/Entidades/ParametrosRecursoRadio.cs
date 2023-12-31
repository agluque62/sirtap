using System;
using System.Data;
using System.Collections.Generic;
using System.Text;

namespace CD40.BD.Entidades
{
    public class ParametrosRecursoRadio : ParametrosRDSCV
    {
        #region Propiedades
        // Identificador del recurso
        protected string _IdRecursoSCV;
        protected string IdRecursoSCV
        {
            get { return _IdRecursoSCV; }
            set { _IdRecursoSCV = value; }
        }
        // Identificador del sistema al que pertenece el recurso
        protected string _IdSistemaSCV;
        protected string IdSistemaSCV
        {
            get { return _IdSistemaSCV; }
            set { _IdSistemaSCV = value; }
        }
        // Tipo del Recurso
        protected uint _TipoRecurso;
        protected uint TipoRecurso
        {
            get { return _TipoRecurso; }
            set { _TipoRecurso = value; }
        }
        // Selecci�n ganancia/AGC TX. 0: Ganancia; 1: AGC
        private uint _GananciaAGCTX;
        public uint GananciaAGCTX
        {
            get { return _GananciaAGCTX; }
            set { _GananciaAGCTX = value; }
        }
        // Ganancia o nivel salida AGC TX. En dBm
        private float _GananciaAGCTXdBm;
        public float GananciaAGCTXdBm
        {
            get { return _GananciaAGCTXdBm; }
            set { _GananciaAGCTXdBm = value; }
        }
        // Selecci�n ganancia/AGC RX. 0: Ganancia; 1: AGC
        private uint _GananciaAGCRX;
        public uint GananciaAGCRX
        {
            get { return _GananciaAGCRX; }
            set { _GananciaAGCRX = value; }
        }
        // Ganancia o nivel salida AGC RX. En dBm
        private float _GananciaAGCRXdBm;
        public float GananciaAGCRXdBm
        {
            get { return _GananciaAGCRXdBm; }
            set { _GananciaAGCRXdBm = value; }
        }
        // Usar supersi�n silencio
        private bool _SupresionSilencio;
        public bool SupresionSilencio
        {
            get { return _SupresionSilencio; }
            set { _SupresionSilencio = value; }
        }
        // Tama�o por defecto trama RTP. En ms. Por defecto 20msg
        private uint _TamRTP;
        public uint TamRTP
        {
            get { return _TamRTP; }
            set { _TamRTP = value; }
        }
        //Codec preferido. 0:G711 ley A; 1:G711 ley �
        private uint _Codec;
        public uint Codec
        {
            get { return _Codec; }
            set { _Codec = value; }
        }
        private bool _GrabacionEd137;
        public bool GrabacionEd137
        {
            get { return _GrabacionEd137; }
            set { _GrabacionEd137 = value; }
        }
        // Umbral AGC TX. En dBm
        private float _UmbralAGCTXdBm;
        public float UmbralAGCTXdBm
        {
            get { return _UmbralAGCTXdBm; }
            set { _UmbralAGCTXdBm = value; }
        }

        //Umbral AGC RX. En dBm
        private float _UmbralAGCRXdBm;
        public float UmbralAGCRXdBm
        {
            get { return _UmbralAGCRXdBm; }
            set { _UmbralAGCRXdBm = value; }
        }
        #endregion

        public ParametrosRecursoRadio()
        {
            TipoRecurso = (uint)Tipos.Tipo_Recurso.TR_RADIO;
        }

        public override /* sealed */ string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdSistemaSCV != null && IdRecursoSCV != null)
                Consulta.Append("SELECT * FROM ParametrosRecurso WHERE IdSistema='" + IdSistemaSCV + "' AND IdRecurso='" + IdRecursoSCV + "' AND TipoRecurso='" + TipoRecurso + "'");
            else if (IdSistemaSCV != null)
                Consulta.Append("SELECT * FROM ParametrosRecurso WHERE IdSistema='" + IdSistemaSCV + "' AND TipoRecurso=" + TipoRecurso);
            else
                Consulta.Append("SELECT * FROM ParametrosRecurso");

            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            System.Collections.Generic.List<Tablas> listaParametros = new System.Collections.Generic.List<Tablas>();

            if (ds != null && ds.Tables.Count > 0)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    ParametrosRecursoRadio pr = new ParametrosRecursoRadio();

                    pr.IdRecursoSCV = (string)dr["IdRecurso"];
                    pr.IdSistemaSCV = (string)dr["IdSistema"];
                    pr.TipoRecurso = (uint)dr["TipoRecurso"];
                    if (dr["GananciaAGCTX"] != System.DBNull.Value)
                        pr.GananciaAGCTX = (uint)dr["GananciaAGCTX"];
                    if (dr["GananciaAGCTXdBm"] != System.DBNull.Value)
                        pr.GananciaAGCTXdBm = (float)dr["GananciaAGCTXdBm"];
                    if (dr["GananciaAGCRX"] != System.DBNull.Value)
                        pr.GananciaAGCRX = (uint)dr["GananciaAGCRX"];
                    if (dr["GananciaAGCRXdBm"] != System.DBNull.Value)
                        pr.GananciaAGCRXdBm = (float)dr["GananciaAGCRXdBm"];
                    if (dr["SupresionSilencio"] != System.DBNull.Value)
                        pr.SupresionSilencio = (bool)dr["SupresionSilencio"];   //Incompatibilidad MySql Server 5.6.11 y 5.0  != 0;
                    if (dr["TamRTP"] != System.DBNull.Value)
                        pr.TamRTP = (uint)dr["TamRTP"];
                    if (dr["Codec"] != System.DBNull.Value)
                        pr.Codec = (uint)dr["Codec"];
                    if (dr["GrabacionEd137"] != System.DBNull.Value)
                        pr.GrabacionEd137 = (bool)dr["GrabacionEd137"];   //Incompatibilidad MySql Server 5.6.11 y 5.0  != 0;
                    if (dr["UmbralAGCTXdBm"] != System.DBNull.Value)
                        pr.UmbralAGCTXdBm = (float)dr["UmbralAGCTXdBm"];
                    if (dr["UmbralAGCRXdBm"] != System.DBNull.Value)
                        pr.UmbralAGCRXdBm = (float)dr["UmbralAGCRXdBm"];
                    listaParametros.Add(pr);
                }
            }
            return listaParametros;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.AppendFormat(System.Globalization.CultureInfo.GetCultureInfo("en-US"), "INSERT INTO ParametrosRecurso " +
                "(IdSistema,IdRecurso,TipoRecurso,GananciaAGCTX,GananciaAGCTXdBm,GananciaAGCRX,GananciaAGCRXdBm,SupresionSilencio,TamRTP,"+
                "Codec,GrabacionEd137,UmbralAGCTXdBm,UmbralAGCRXdBm) " +                                                                        // ERROR #4285
                "VALUES ('{0}','{1}',{2},{3},{4:0.00},{5},{6:0.00},{7},{8},{9},{10},{11:0.00},{12:0.00})",
                IdSistemaSCV, IdRecursoSCV, TipoRecurso, GananciaAGCTX, GananciaAGCTXdBm, GananciaAGCRX, GananciaAGCRXdBm, SupresionSilencio, TamRTP, Codec,
                GrabacionEd137, UmbralAGCTXdBm, UmbralAGCRXdBm);

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistemaSCV, "ParametrosRecurso");
			return consulta;
		}

        public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
            Consulta.AppendFormat(System.Globalization.CultureInfo.GetCultureInfo("en-US"), "UPDATE ParametrosRecurso SET IdRecurso='{0}',IdSistema='{1}',TipoRecurso={2}," + 
                                  "GananciaAGCTX={3},GananciaAGCTXdBm={4:0.00},GananciaAGCRX={5},GananciaAGCRXdBm={6:0.00}," +
                                  "SupresionSilencio={7},TamRTP={8},Codec={9},GrabacionEd137={10}," + 
                                  "UmbralAGCTXdBm={11:0.00},UmbralAGCRXdBm={12:0.00}" + 
                                  " WHERE IdRecurso='{13}' AND IdSistema='{14}'", 
                                 IdRecursoSCV, IdSistemaSCV, TipoRecurso, 
                                 GananciaAGCTX, GananciaAGCTXdBm, GananciaAGCRX, GananciaAGCRXdBm, 
                                 SupresionSilencio, TamRTP, Codec, GrabacionEd137,
                                 UmbralAGCTXdBm, UmbralAGCRXdBm,
                                 IdRecursoSCV, IdSistemaSCV);
			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistemaSCV, "ParametrosRecurso");
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
            if (IdSistemaSCV != null && IdRecursoSCV != null)
                Consulta.Append("DELETE FROM ParametrosRecurso WHERE IdSistema='" + IdSistemaSCV + "' AND IdRecurso='" + IdRecursoSCV + "'");
            else if (IdSistemaSCV != null)
                Consulta.Append("DELETE FROM ParametrosRecurso WHERE IdSistema='" + IdSistemaSCV + "'");
            else
                Consulta.Append("DELETE FROM ParametrosRecurso");

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistemaSCV, "ParametrosRecurso");

			return consulta;
		}

		//public override int SelectCountSQL(string where)
		//{
		//    Consulta.Remove(0, Consulta.Length);
		//    Consulta.Append("SELECT COUNT(*) FROM ParametrosRecurso WHERE " + where);

		//    return Convert.ToInt32(ServiceAccesoABaseDeDatos.ExecuteScalar(Consulta.ToString()));
		//}

        public override string[] UpdateDestinoSQL() { return null; }

        public override string[] LiberaDestinoSQL() { return null; }
    }
}
