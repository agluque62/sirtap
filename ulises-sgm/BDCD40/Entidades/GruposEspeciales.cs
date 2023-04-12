using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace CD40.BD.Entidades
{
    public class GruposEspeciales : Tablas
    {
        #region Propiedades de GruposEspeciales

        private string _IdAgrupacion;
        public string IdAgrupacion
        {
            get { return _IdAgrupacion; }
            set { _IdAgrupacion = value; }
        }

        private string _idSistema;
        public string IdSistema
        {
            get { return _idSistema; }
            set { _idSistema = value; }
        }

        #endregion

		//static AccesoABaseDeDatos ServiceAccesoABaseDeDatos;

        public GruposEspeciales()
        {
			//if (ServiceAccesoABaseDeDatos == null)
			//    ServiceAccesoABaseDeDatos = new AccesoABaseDeDatos();
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (!string.IsNullOrEmpty(IdSistema) && !string.IsNullOrEmpty(IdAgrupacion))
                Consulta.AppendFormat("SELECT * FROM GruposEspeciales WHERE IdSistema='{0}' AND IdAgrupacion='{1}'", IdSistema, IdAgrupacion);
            else if (IdSistema != null)
                Consulta.Append("SELECT * FROM GruposEspeciales WHERE IdSistema='" + IdSistema + "'");
            else if (IdAgrupacion != null)
                Consulta.Append("SELECT * FROM GruposEspeciales WHERE IdAgrupacion='" + IdAgrupacion + "'");
            else
                Consulta.Append("SELECT * FROM GruposEspeciales");

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
                    GruposEspeciales a = new GruposEspeciales();

                    a.IdAgrupacion = (string)dr["IdAgrupacion"];
                    a.IdSistema = (string)dr["IdSistema"];

                    ListaResultado.Add(a);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO GruposEspeciales (IdSistema,IdAgrupacion)" +
                            " VALUES ('" + IdSistema + "','" +
                                         IdAgrupacion + "')");

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistema, "GruposEspeciales");
			return consulta;
        }

        public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE GruposEspeciales SET IdAgrupacion='" + IdAgrupacion + "'," +
                                            "IdSistema='" + IdSistema + "' " +
                                            "WHERE IdAgrupacion='" + IdAgrupacion + "' AND " + "IdSistema='" + IdSistema + "'"
                                            );

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistema, "GruposEspeciales");
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[2];
			
			Consulta.Remove(0, Consulta.Length);
            if (IdSistema != null && IdAgrupacion != null)
                Consulta.Append("DELETE FROM GruposEspeciales WHERE IdSistema='" + IdSistema + "' AND IdAgrupacion='" + IdAgrupacion + "'");
            else if (IdSistema != null)
                Consulta.Append("DELETE FROM GruposEspeciales WHERE IdSistema='" + IdSistema + "'");
            else if (IdAgrupacion != null)
                Consulta.Append("DELETE FROM GruposEspeciales WHERE IdAbonado='" + IdAgrupacion + "'");
            else
                Consulta.Append("DELETE FROM GruposEspeciales");

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistema, "GruposEspeciales");
			return consulta;
		}
    }
}
