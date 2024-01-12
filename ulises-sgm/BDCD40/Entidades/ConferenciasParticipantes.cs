using System;
using System.Data;
using System.Collections.Generic;
using System.Text;

namespace CD40.BD.Entidades
{
    public class ConferenciasParticipantes : Tablas
    {
        #region Propiedades de ConferenciasParticipantes
        private string _IdSistema;
        public string IdSistema
        {
            get { return _IdSistema; }
            set { _IdSistema = value; }
        }

        private string _IdConferencia;
        public string IdConferencia
        {
            get { return _IdConferencia; }
            set { _IdConferencia = value; }
        }

        private string _SipUri;
        public string SipUri
        {
            get { return _SipUri; }
            set { _SipUri = value; }
        }
        
        private string _Descripcion;
        public string Descripcion
        {
            get { return _Descripcion; }
            set { _Descripcion = value; }
        }
        
        #endregion

        public ConferenciasParticipantes()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdSistema != null && IdConferencia != null)
                Consulta.Append("SELECT * FROM ConferenciasParticipantes WHERE IdSistema='" + IdSistema + "' AND IdConferencia ='" + IdConferencia + "' ORDER BY SipUri ASC");
            else
                Consulta.Append("SELECT * FROM ConferenciasParticipantes ORDER BY SipUriI ASC");
            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();
            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    ConferenciasParticipantes r = new ConferenciasParticipantes();
                    if (dr["IdSistema"] != System.DBNull.Value)
                        r.IdSistema = (string)dr["IdSistema"];
                    if (dr["IdConferencia"] != System.DBNull.Value)
                        r.IdConferencia = (string)dr["IdConferencia"];
                    if (dr["SipUri"] != System.DBNull.Value)
                        r.SipUri = (string)dr["SipUri"];
                    if (dr["Descripcion"] != System.DBNull.Value)
                        r.Descripcion = (string)dr["Descripcion"];
                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];
            Consulta.Remove(0, Consulta.Length);

            Consulta.Append("INSERT INTO ConferenciasParticipantes VALUES ('" + IdSistema + "', '" + IdConferencia + "', '" + SipUri + "', '" + Descripcion + "')");

			consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "ConferenciasParticipantes");
			return consulta;
        }

        public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];
			
			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE ConferenciasParticipantes SET Descripcion= '" + Descripcion + 
                " WHERE IdSistema='" + IdSistema + "' AND IdConferencia='" + IdConferencia + "'" +
                " AND SipUri = '" + SipUri + "'"
                );
			consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "ConferenciasParticipantes");
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[4];

			Consulta.Remove(0, Consulta.Length);
            if (IdSistema != null && IdConferencia != null && SipUri != null)
                Consulta.Append("DELETE FROM ConferenciasParticipantes WHERE SipUri ='"+ SipUri +"' AND IdConferencia ='" + IdConferencia + "' AND IdSistema='" + IdSistema + "' ");
            else if (IdSistema != null && IdConferencia != null )
                Consulta.Append("DELETE FROM ConferenciasParticipantes WHERE IdConferencia ='" + IdConferencia + "' AND IdSistema='" + IdSistema + "' ");
            else if (IdSistema != null)
                Consulta.Append("DELETE FROM ConferenciasParticipantes WHERE IdSistema='" + IdSistema + "'");
            consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "ConferenciasParticipantes");
		    return consulta;
		}

    }
}