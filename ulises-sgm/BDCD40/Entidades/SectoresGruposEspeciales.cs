using System;
using System.Data;
using System.Collections.Generic;
using System.Text;

namespace CD40.BD.Entidades
{
    public class SectoresGruposEspeciales : Tablas
    {
        #region Propiedades de SectoresGruposEspeciales

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

        private string _IdSector;
        public string IdSector
        {
            get { return _IdSector; }
            set { _IdSector = value; }
        }

        private string _IdNucleo;
        public string IdNucleo
        {
            get { return _IdNucleo; }
            set { _IdNucleo = value; }
        }
        #endregion
		
        public SectoresGruposEspeciales()
        {
			//if (ServiceAccesoABaseDeDatos == null)
			//    ServiceAccesoABaseDeDatos = new AccesoABaseDeDatos();
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdSistema != null && IdAgrupacion != null)
                Consulta.Append("SELECT * FROM SectoresGruposEspeciales WHERE IdSistema='" + IdSistema + "' AND IdAgrupacion='" + IdAgrupacion + "'");
            else if (IdSistema != null && IdSector != null && IdNucleo != null)
                Consulta.Append("SELECT * FROM SectoresGruposEspeciales WHERE IdSistema='" + IdSistema + "' AND IdSector='" + IdSector + "' AND IdNucleo='" + IdNucleo + "'");
			else if (IdSistema != null)
				Consulta.Append("SELECT * FROM SectoresGruposEspeciales WHERE IdSistema='" + IdSistema + "'");
			else
                Consulta.Append("SELECT * FROM SectoresGruposEspeciales");

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
                    SectoresGruposEspeciales a = new SectoresGruposEspeciales();

                    a.IdAgrupacion = (string)dr["IdAgrupacion"];
                    a.IdSistema = (string)dr["IdSistema"];
                    a.IdSector = (string)dr["IdSector"];
                    a.IdNucleo = (string)dr["IdNucleo"];

                    ListaResultado.Add(a);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO SectoresGruposEspeciales (IdSistema,IdAgrupacion,IdSector,IdNucleo)" +
                            " VALUES ('" + IdSistema + "','" +
                                         IdAgrupacion + "','" +
                                         IdSector + "','" +
                                         IdNucleo + "')");

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistema, "SectoresGruposEspeciales");
			return consulta;
        }

		public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];
			
			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE SectoresGruposEspeciales SET IdAgrupacion='" + IdAgrupacion + "'," +
                                            "IdSistema='" + IdSistema + "'," +
                                            "IdSector='" + IdSector + "'," +
                                            "IdNucleo='" + IdNucleo + "' " +
                                            "WHERE IdAgrupacion='" + IdAgrupacion + "' AND " + "IdSistema='" + IdSistema + "'"
                                            );

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistema, "SectoresGruposEspeciales");
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[2];
			
			Consulta.Remove(0, Consulta.Length);
            if (IdSistema != null && IdAgrupacion != null)
                Consulta.Append("DELETE FROM SectoresGruposEspeciales WHERE IdSistema='" + IdSistema + "' AND IdAgrupacion='" + IdAgrupacion + "'");
            else if (IdSistema != null)
                Consulta.Append("DELETE FROM SectoresGruposEspeciales WHERE IdSistema='" + IdSistema + "'");
            else
                Consulta.Append("DELETE FROM SectoresGruposEspeciales");

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(IdSistema, "SectoresGruposEspeciales");
			return consulta;
		}
    }
}
