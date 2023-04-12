using System;
using System.Collections.Generic;
using System.Data;
using System.Text;

namespace CD40.BD.Entidades
{
    public class PermisosOpeDepATS : Tablas
    {
        #region Propiedades de PermisosOpeDepATS

        private string _idOperador;
        public string IdOperador
        {
            get { return _idOperador; }
            set { _idOperador = value; }
        }

        private string _IdSistema;
        public string IdSistema
        {
            get { return _IdSistema; }
            set { _IdSistema = value; }
        }

        private string _IdDependenciaATS;
        public string IdDependenciaATS
        {
            get { return _IdDependenciaATS; }
            set { _IdDependenciaATS = value; }
        }

        #endregion

        public PermisosOpeDepATS()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (_IdSistema != null && _idOperador != null)
                Consulta.Append("SELECT * FROM permisosopedepats WHERE IdSistema='" + _IdSistema + "' AND IdOperador = '" + _idOperador + "'");
            else if (_IdSistema != null)
                Consulta.Append("SELECT * FROM permisosopedepats WHERE IdSistema='" + _IdSistema + "'");
            else
                Consulta.Append("SELECT * FROM permisosopedepats");

            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();

            if (ds != null && ds.Tables.Count > 0)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    PermisosOpeDepATS r = new PermisosOpeDepATS();
                    if (dr["IdOperador"] != System.DBNull.Value)
                        r.IdSistema = (string)dr["IdOperador"];
                    if (dr["IdSistema"] != System.DBNull.Value)
                        r.IdSistema = (string)dr["IdSistema"];
                    if (dr["IdDependenciaATS"] != System.DBNull.Value)
                        r.IdDependenciaATS = (string)dr["IdDependenciaATS"];


                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
            string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO permisosopedepats (IdOperador, Idsistema, IdDependenciaATS)" +
                            " VALUES ('" + IdOperador + "','" + IdSistema + "','" + IdDependenciaATS + "')");

            consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(_IdSistema, "permisosopedepats");
            return consulta;
        }

        public override string[] UpdateSQL()
        {
            string[] consulta = new string[2];
            // No se realiza Update en permisosopedepats
            //Consulta.Remove(0, Consulta.Length);
            //Consulta.Append("UPDATE permisosopedepats SET IdDependenciaATS='" + _IdDependenciaATS + "' " +
            //                                "WHERE IdDependenciaATS= '" + _IdDependenciaATS + "'");
            //consulta[0] = Consulta.ToString();
            //consulta[1] = ReplaceSQL(_IdSistema, "dependenciasats");
            return consulta;
        }

        public override string[] DeleteSQL()
        {
            string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);

            if (_IdSistema != null && _idOperador != null && _IdDependenciaATS != null)
            {
                Consulta.Append("DELETE FROM permisosopedepats WHERE IdOperador = '" + _idOperador + "' AND IdSistema = '" +
                                                                                       _IdSistema + "' AND IdDependenciaATS = '" +
                                                                                       _IdDependenciaATS + "'");
            }
            else if (_IdSistema != null && _idOperador != null)
            {
                Consulta.Append("DELETE FROM permisosopedepats WHERE IdOperador = '" + _idOperador + "' AND IdSistema = '" +
                                                                                       _IdSistema + "'");
            }
            else if (_IdSistema != null)
            {
                Consulta.Append("DELETE FROM permisosopedepats WHERE IdSistema = '" + _IdSistema + "'");
            }
            else
            {
                Consulta.Append("DELETE FROM permisosopedepats");
            }

            consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(_IdSistema, "permisosopedepats");
            return consulta;
        }
    }
}
