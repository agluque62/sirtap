using System;
using System.Collections.Generic;
using System.Data;
using System.Text;

namespace CD40.BD.Entidades
{
    public class DependenciasATS : Tablas
    {
        #region Propiedades de DependenciasATS

        private string _IdDependenciaATS;
        public string IdDependenciaATS
        {
            get { return _IdDependenciaATS; }
            set { _IdDependenciaATS = value; }
        }

        private string _IdSistema;
        public string IdSistema
        {
            get { return _IdSistema; }
            set { _IdSistema = value; }
        }

        private string _IdDependenciaATSNew;
        public string IdDependenciaATSNew
        {
            get { return _IdDependenciaATSNew; }
            set { _IdDependenciaATSNew = value; }
        }
        #endregion
        public DependenciasATS()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (_IdDependenciaATS != null && _IdSistema != null )
                Consulta.Append("SELECT * FROM dependenciasats WHERE IdSistema='" + _IdSistema + "' AND IdDependenciaATS = '" + _IdDependenciaATS + "'");
            else if (_IdSistema != null)
                Consulta.Append("SELECT * FROM dependenciasats WHERE IdSistema='" + _IdSistema + "'");
            else
                Consulta.Append("SELECT * FROM dependenciasats");

            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();

            if (ds != null && ds.Tables.Count > 0)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    DependenciasATS r = new DependenciasATS();

                    if (dr["IdDependenciaATS"] != System.DBNull.Value)
                        r.IdDependenciaATS = (string)dr["IdDependenciaATS"];
                    if (dr["IdSistema"] != System.DBNull.Value)
                        r.IdSistema = (string)dr["IdSistema"];
                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
            string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO dependenciasats (IdDependenciaATS, Idsistema)" +
                            " VALUES ('" + IdDependenciaATS + "','" + IdSistema + "')");

            consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(_IdSistema, "dependenciasats");
            return consulta;
        }

        public override string[] UpdateSQL()
        {
            string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE dependenciasats SET IdDependenciaATS='" + _IdDependenciaATSNew + "' " +
                                            "WHERE IdDependenciaATS= '" + _IdDependenciaATS + "' AND IdSistema='" + _IdSistema + "'" );
            consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(_IdSistema, "dependenciasats");

            return consulta;
        }

        public override string[] DeleteSQL()
        {
            string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            if (_IdDependenciaATS != null)
                Consulta.Append("DELETE FROM dependenciasats WHERE IdDependenciaATS= '" + _IdDependenciaATS + "'");
            else
                Consulta.Append("DELETE FROM dependenciasats");
            consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(_IdSistema, "dependenciasats");
            return consulta;
        }
    }
}
