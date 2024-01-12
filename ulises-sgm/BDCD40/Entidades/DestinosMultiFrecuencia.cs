using System;
using System.Data;
using System.Collections.Generic;
using System.Text;

namespace CD40.BD.Entidades
{
    public class DestinosMultiFrecuencia : Tablas
    {
        #region Propiedades de DestinosMultiFrecuencia
        private string _idSistema;
        public string IdSistema
        {
            get { return _idSistema; }
            set { _idSistema = value; }
        }

        private string _IdDestino;
        public string IdDestino
        {
            get { return _IdDestino; }
            set { _IdDestino = value; }
        }

        private bool _FrecuenciaDefecto;
        public bool FrecuenciaDefecto
        {
            get { return _FrecuenciaDefecto; }
            set { _FrecuenciaDefecto = value; }
        }

        private string _Frecuencia;
        public string Frecuencia
        {
            get { return _Frecuencia; }
            set { _Frecuencia = value; }
        }

        #endregion

        public DestinosMultiFrecuencia()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdSistema != null && IdDestino != null)
                Consulta.Append("SELECT * FROM DestinosMultiFrecuencia WHERE IdSistema='" + IdSistema + "' AND IdDestino='" + IdDestino + "' ORDER BY Frecuencia ASC");
            else
                Consulta.Append("SELECT * FROM DestinosMultiFrecuencia ORDER BY Frecuencia ASC");
            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();
            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    DestinosMultiFrecuencia r = new DestinosMultiFrecuencia();
                    if (dr["IdSistema"] != System.DBNull.Value)
                        r.IdSistema = (string)dr["IdSistema"];
                    if (dr["IdDestino"] != System.DBNull.Value)
                        r.IdDestino = (string)dr["IdDestino"];
                    if (dr["FrecuenciaDefecto"] != System.DBNull.Value)
                        r.FrecuenciaDefecto = (bool)dr["FrecuenciaDefecto"];
                    if (dr["Frecuencia"] != System.DBNull.Value)
                        r.Frecuencia = (string)dr["Frecuencia"];
                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];
            Consulta.Remove(0, Consulta.Length);

            Consulta.Append("INSERT INTO DestinosMultiFrecuencia VALUES ('" + IdSistema + "', '" + IdDestino + "', "+ FrecuenciaDefecto +", '"+ Frecuencia +"')");

			consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "DestinosMultiFrecuencia");
			return consulta;
        }

        public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];
			
			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE DestinosMultiFrecuencia SET FrecuenciaDefecto=" + FrecuenciaDefecto + ", " +
                                            " WHERE IdSistema='" + IdSistema + "' AND IdDestino='" + IdDestino + "' AND Frecuencia='" + Frecuencia + "'"
                                            );
			consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "DestinosMultiFrecuencia");
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[4];

			Consulta.Remove(0, Consulta.Length);
            if (IdSistema != null && IdDestino != null )
                Consulta.Append("DELETE FROM DestinosMultiFrecuencia WHERE IdDestino='" + IdDestino + "' AND IdSistema='" + IdSistema + "' ");
            else if (IdSistema != null)
                Consulta.Append("DELETE FROM DestinosMultiFrecuencia WHERE IdSistema='" + IdSistema + "'");
            else
                Consulta.Append("DELETE FROM DestinosMultiFrecuencia");
            consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "DestinosMultiFrecuencia");
		    return consulta;
		}

    }
}