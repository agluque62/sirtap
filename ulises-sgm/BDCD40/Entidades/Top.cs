using System;
using System.Data;
using System.Collections.Generic;
using System.Text;

namespace CD40.BD.Entidades
{
    public class Top : Tablas
    {
        #region Propiedades de Top

        private string _idSistema;
        public string IdSistema
        {
            get { return _idSistema; }
            set { _idSistema = value; }
        }

        private string _idTop;
        public string IdTop
        {
            get { return _idTop; }
            set { _idTop = value; }
        }

        private string _ModoArranque;
        public string ModoArranque
        {
            get { return _ModoArranque; }
            set { _ModoArranque = value; }
        }

        private uint _PosicionSacta;
        public uint PosicionSacta
        {
            get { return _PosicionSacta; }
            set { _PosicionSacta = value; }
        }

        private string _IpRed1;
        public string IpRed1
        {
            get { return _IpRed1; }
            set { _IpRed1 = value; }
        }

        private string _IpRed2;
        public string IpRed2
        {
            get { return _IpRed2; }
            set { _IpRed2 = value; }
        }

		private bool _ConexionJacksEjecutivo;
		public bool ConexionJacksEjecutivo
		{
			get { return _ConexionJacksEjecutivo; }
			set { _ConexionJacksEjecutivo = value; }
		}

		private bool _ConexionJacksAyudante;
		public bool ConexionJacksAyudante
		{
			get { return _ConexionJacksAyudante; }
			set { _ConexionJacksAyudante = value; }
		}

		private int _NumAltavoces;
		public int NumAltavoces
		{
			get { return _NumAltavoces; }
			set { _NumAltavoces = value; }
		}
        private string _Grabador1;
        public string Grabador1
        {
            get { return _Grabador1; }
            set { _Grabador1 = value; }
        }
        private string _Grabador2;
        public string Grabador2
        {
            get { return _Grabador2; }
            set { _Grabador2 = value; }
        }

        // 20210316 #4749
        private string _IdDependenciaATS;
        public string IdDependenciaATS
        {
            get { return _IdDependenciaATS; }
            set { _IdDependenciaATS = value; }
        }


        private int _GrabacionAnalogica;
        public int GrabacionAnalogica
        {
            get { return _GrabacionAnalogica; }
            set { _GrabacionAnalogica = value; }
        }

        private int _GrabacionED137; 
        public int GrabacionED137
        {
            get { return _GrabacionED137; }
            set { _GrabacionED137 = value; }
        }

        private int _TipoGrabacionAnalogica;
        public int TipoGrabacionAnalogica
        {
            get { return _TipoGrabacionAnalogica; }
            set { _TipoGrabacionAnalogica = value; }
        }

        private int _RtspGrabador1;
        public int RtspGrabador1
        {
            get { return _RtspGrabador1; }
            set { _RtspGrabador1 = value; }
        }
        private int _RtspGrabador2;
        public int RtspGrabador2
        {
            get { return _RtspGrabador2; }
            set { _RtspGrabador2 = value; }
        }



        #endregion

		//static AccesoABaseDeDatos ServiceAccesoABaseDeDatos;

        public Top()
        {
			//if (ServiceAccesoABaseDeDatos == null)
			//    ServiceAccesoABaseDeDatos = new AccesoABaseDeDatos();
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
			if (IdSistema != null && IdTop != null)
                Consulta.Append("SELECT T.*,eu1.IpRed1 as IpRecorder1,eu2.IpRed1 as IpRecorder2 FROM Top T " +
                            "LEFT JOIN equiposeu eu1 ON (eu1.IdEquipos=T.Grabador1) " +
                            "LEFT JOIN equiposeu eu2 ON (eu2.IdEquipos=T.Grabador2) " +
                            "WHERE T.IdSistema='" + IdSistema + "' AND IdTop='" + IdTop + "'");
			else if (IdSistema != null && IpRed1 != null)
                Consulta.Append("SELECT T.*,eu1.IpRed1 as IpRecorder1,eu2.IpRed1 as IpRecorder2 FROM Top T " +
                            "LEFT JOIN equiposeu eu1 ON (eu1.IdEquipos=T.Grabador1) " +
                            "LEFT JOIN equiposeu eu2 ON (eu2.IdEquipos=T.Grabador2) " +
                            "WHERE T.IdSistema='" + IdSistema + "' AND IpRed1='" + IpRed1 + "'");
			else if (IdSistema != null && PosicionSacta != 0)
                Consulta.Append("SELECT T.*,eu1.IpRed1 as IpRecorder1,eu2.IpRed1 as IpRecorder2 FROM Top T " +
                            "LEFT JOIN equiposeu eu1 ON (eu1.IdEquipos=T.Grabador1) " +
                            "LEFT JOIN equiposeu eu2 ON (eu2.IdEquipos=T.Grabador2) " +
                            "WHERE T.IdSistema='" + IdSistema + "' AND PosicionSacta=" + PosicionSacta);
            else if (IdSistema != null && Grabador1 != null)
                Consulta.Append("SELECT T.*,eu1.IpRed1 as IpRecorder1,eu2.IpRed1 as IpRecorder2 FROM Top T " +
                            "LEFT JOIN equiposeu eu1 ON (eu1.IdEquipos=T.Grabador1) " +
                            "LEFT JOIN equiposeu eu2 ON (eu2.IdEquipos=T.Grabador2) " +
                            "WHERE T.IdSistema='" + IdSistema + "' AND (Grabador1='" + Grabador1 + "' OR Grabador2='" + Grabador1 + "')");
            else if (IdSistema != null)
                Consulta.Append("SELECT T.*,eu1.IpRed1 as IpRecorder1,eu2.IpRed1 as IpRecorder2 FROM Top T " +
                            "LEFT JOIN equiposeu eu1 ON (eu1.IdEquipos=T.Grabador1) " +
                            "LEFT JOIN equiposeu eu2 ON (eu2.IdEquipos=T.Grabador2) " +
                            "WHERE T.IdSistema='" + IdSistema + "' ORDER BY IdTop");
			else if (IdTop != null)
                Consulta.Append("SELECT T.*,eu1.IpRed1 as IpRecorder1,eu2.IpRed1 as IpRecorder2 FROM Top T " +
                            "LEFT JOIN equiposeu eu1 ON (eu1.IdEquipos=T.Grabador1) " +
                            "LEFT JOIN equiposeu eu2 ON (eu2.IdEquipos=T.Grabador2) " +
                            "WHERE IdTop='" + IdTop + "'");
			else
                Consulta.Append("SELECT T.*,eu1.IpRed1 as IpRecorder1,eu2.IpRed1 as IpRecorder2 FROM Top T " +
                            "LEFT JOIN equiposeu eu1 ON (eu1.IdEquipos=T.Grabador1) " +
                            "LEFT JOIN equiposeu eu2 ON (eu2.IdEquipos=T.Grabador2) " +
                            "ORDER BY IdTop");

            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();

			//DataSetResultado = this.DataSetSelectSQL();
            if (ds != null && ds.Tables.Count > 0)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    Top top = new Top();

                    top.IdTop = (string)dr["IdTOP"];
                    top.IdSistema = (string)dr["IdSistema"];
                    if (dr["PosicionSacta"] != System.DBNull.Value)
                        top.PosicionSacta = (uint)dr["PosicionSacta"];
                    if (dr["ModoArranque"] != System.DBNull.Value)
                        top.ModoArranque = (string)dr["ModoArranque"];
                    if (dr["IpRed1"] != System.DBNull.Value)
                        top.IpRed1 = (string)dr["IpRed1"];
                    if (dr["IpRed2"] != System.DBNull.Value)
                        top.IpRed2 = (string)dr["IpRed2"];
					if (dr["ConexionJacksEjecutivo"] != System.DBNull.Value)
                        top.ConexionJacksEjecutivo = (bool)dr["ConexionJacksEjecutivo"];    //Incompatibilidad MySql Server 5.6.11 y 5.0  != 0;
					if (dr["ConexionJacksAyudante"] != System.DBNull.Value)
                        top.ConexionJacksAyudante = (bool)dr["ConexionJacksAyudante"];  //Incompatibilidad MySql Server 5.6.11 y 5.0  != 0;
					if (dr["NumAltavoces"] != System.DBNull.Value)
						top.NumAltavoces = (short)dr["NumAltavoces"];
                    if (dr["Grabador1"] != System.DBNull.Value)
                        top.Grabador1 = (string)dr["Grabador1"];
                    else
                        top.Grabador1 = null;
                    if (dr["Grabador2"] != System.DBNull.Value)
                        top.Grabador2 = (string)dr["Grabador2"];
                    else
                        top.Grabador2 = null;
                    // 20210316 #4749
                    if (dr["IdDependenciaATS"] != System.DBNull.Value)
                    {
                        top.IdDependenciaATS = (string)dr["IdDependenciaATS"];
                    }

                    if (dr["GrabacionAnalogica"] != System.DBNull.Value)
                        top.GrabacionAnalogica = (int)dr["GrabacionAnalogica"];

                    if (dr["GrabacionED137"] != System.DBNull.Value)
                        top.GrabacionED137 = (int)dr["GrabacionED137"];

                    if (dr["TipoGrabacionAnalogica"] != System.DBNull.Value)
                        top.TipoGrabacionAnalogica = (int)dr["TipoGrabacionAnalogica"];
                    
                    // #RFQ-2
                    if (dr["RtspGrabador1"] != System.DBNull.Value)
                        top.RtspGrabador1 = (int)dr["RtspGrabador1"];

                    if (dr["RtspGrabador2"] != System.DBNull.Value)
                        top.RtspGrabador2 = (int)dr["RtspGrabador2"];

                    ListaResultado.Add(top);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO Top (IdSistema,IdTOP,PosicionSacta,ModoArranque,IpRed1,IpRed2,ConexionJacksEjecutivo,ConexionJacksAyudante,NumAltavoces,Grabador1,Grabador2,IdDependenciaATS,GrabacionAnalogica,GrabacionED137,TipoGrabacionAnalogica,RtspGrabador1,RtspGrabador2 )" +
                                        " VALUES ('" + IdSistema + "','" +
                                                     IdTop + "'," +
                                                     PosicionSacta + ",'" +
                                                     ModoArranque + "','" +
                                                     IpRed1 + "','" +
                                                     IpRed2 + "'," +
                                                     ConexionJacksEjecutivo + "," +
                                                     ConexionJacksAyudante + "," +
                                                     NumAltavoces + ",'" +
                                                     Grabador1 + "','" +
                                                     Grabador2 + "','" +
                                                     IdDependenciaATS + "', " +
                                                     GrabacionAnalogica + ", " +
                                                     GrabacionED137 + ", " +
                                                     TipoGrabacionAnalogica + ", " +
                                                     RtspGrabador1 + ", " +
                                                     RtspGrabador2 +  ")");
            

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistema, "Top");
			return consulta;
        }

		public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];
			
			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE Top SET idTOP='" + IdTop + "'," +
                                                        "idSistema='" + IdSistema + "'," +
                                                        "PosicionSacta=" + PosicionSacta + "," +
                                                        "ModoArranque='" + ModoArranque + "'," +
                                                        "IpRed1='" + IpRed1 + "'," +
                                                        "IpRed2='" + IpRed2 + "'," +
                                                        "ConexionJacksEjecutivo=" + ConexionJacksEjecutivo + "," +
                                                        "ConexionJacksAyudante=" + ConexionJacksAyudante + "," +
                                                        "NumAltavoces=" + NumAltavoces + "," +
                                                        "Grabador1='" + Grabador1 + "'," +
                                                        "Grabador2='" + Grabador2 + "', " +
                                                        "IdDependenciaATS='" + IdDependenciaATS + "', " +
                                                        "GrabacionAnalogica=" + GrabacionAnalogica + ", " +
                                                        "GrabacionED137=" + GrabacionED137 + ", " +
                                                        "TipoGrabacionAnalogica=" + TipoGrabacionAnalogica + ", " +
                                                        "RtspGrabador1=" + RtspGrabador1 + ", " +
                                                        "RtspGrabador2=" + RtspGrabador2 + " " +
                                                        "WHERE IdTOP='" + IdTop + "' AND " + "idSistema='" + IdSistema + "'"
                                                        );

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistema, "Top");
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            if (IdSistema != null && IdTop != null)
                Consulta.Append("DELETE FROM Top WHERE IdSistema='" + IdSistema + "' AND IdTop='" + IdTop + "'");
            else if (IdSistema != null)
                Consulta.Append("DELETE FROM Top WHERE IdSistema='" + IdSistema + "'");
            else if (IdTop != null)
                Consulta.Append("DELETE FROM Top WHERE IdTop='" + IdTop + "'");
            else
                Consulta.Append("DELETE FROM Top");

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistema, "Top");
			return consulta;
		}

		//public override int SelectCountSQL(string where)
		//{
		//    Consulta.Remove(0, Consulta.Length);
		//    Consulta.Append("SELECT COUNT(*) FROM Top WHERE " + where);

		//    return Convert.ToInt32(ServiceAccesoABaseDeDatos.ExecuteScalar(Consulta.ToString()));
		//}
    }
}
