using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace CD40.BD.Entidades
{
    public class LogProcesos : Tablas
    {
        #region Propiedades de LogProcesos

        private string _Id;
        public string Id
        {
            get { return _Id; }
            set { _Id = value; }
        }

        private int _PID;
        public int PID
        {
            get { return _PID; }
            set { _PID = value; }
        }

        private string _IdSistema;
        public string IdSistema
        {
            get { return _IdSistema; }
            set { _IdSistema = value; }
        }

        private DateTime _FechaHora;
        public DateTime FechaHora
        {
            get { return _FechaHora; }
            set { _FechaHora = value; }
        }

        private DateTime _FechaHoraLeido;
        public DateTime FechaHoraLeido
        {
            get { return _FechaHoraLeido; }
            set { _FechaHoraLeido = value; }
        }

        private string _Usuario;
        public string Usuario
        {
            get { return _Usuario; }
            set { _Usuario = value; }
        }

        private string _Descripcion;
        public string Descripcion
        {
            get { return _Descripcion; }
            set { _Descripcion = value; }
        }

        private bool _Activo;
        public bool Activo
        {
            get { return _Activo; }
            set { _Activo = value; }
        }

        #endregion

        public LogProcesos()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);

            if (PID != 0)
                Consulta.Append("SELECT Id, PID, IdSistema, FechaHora, FechaHoraLeido, Usuario, Descripcion, Activo FROM LogProcesos " +
                                " WHERE (FechaHoraLeido is null Or Activo = 1) AND PID=" + PID + " ORDER BY Activo desc, FechaHora desc");
            else
                Consulta.Append("SELECT Id, PID, IdSistema, FechaHora, FechaHoraLeido, Usuario, Descripcion, Activo FROM LogProcesos " +
                                " WHERE (FechaHoraLeido is null Or Activo = 1) ORDER BY Activo desc, FechaHora desc");

            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();

            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    LogProcesos s = new LogProcesos();

                    if (dr["Id"] != System.DBNull.Value)
                        s.Id = (string)dr["Id"];
                    if (dr["PID"] != System.DBNull.Value)
                        s.PID = (int)dr["PID"];
                    if (dr["IdSistema"] != System.DBNull.Value)
                        s.IdSistema = (string)dr["IdSistema"];
                    if (dr["FechaHora"] != System.DBNull.Value)
                        s.FechaHora = (DateTime)dr["FechaHora"];
                    if (dr["FechaHoraLeido"] != System.DBNull.Value)
                        s.FechaHoraLeido = (DateTime)dr["FechaHoraLeido"];
                    if (dr["Usuario"] != System.DBNull.Value)
                        s.Usuario = (string)dr["Usuario"];
                    if (dr["Descripcion"] != System.DBNull.Value)
                        s.Descripcion = (string)dr["Descripcion"];
                    if (dr["Activo"] != System.DBNull.Value)
                        s.Activo = (bool)dr["Activo"];

                    ListaResultado.Add(s);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
            string[] consulta = new string[1];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO LogProcesos " +
                                "SET Id ='" + Id + "'" +
                                "  , PID=" + PID +
                                "  , IdSistema='" + IdSistema + "'" +
                                "  , Usuario='" + Usuario + "'" +
                                "  , Descripcion='" + Descripcion + "'" +
                                "  , Activo=" + (Activo ? 1 : 0));

            consulta[0] = Consulta.ToString();
            return consulta;
        }

        public override string[] UpdateSQL()
        {
            string[] consulta = new string[1];
            string cmdUpdate ="";

            Consulta.Remove(0, Consulta.Length);

            cmdUpdate = "UPDATE LogProcesos " +
                        "SET Usuario='" + Usuario + "'" +
                        "  , Activo=" + (Activo ? 1 : 0);

            if (PID != 0)
                cmdUpdate = cmdUpdate + "  , PID=" + PID;

            if (!string.IsNullOrEmpty(IdSistema))
                cmdUpdate = cmdUpdate + "  , IdSistema='" + IdSistema + "'";

            if (FechaHoraLeido != DateTime.MinValue)
                cmdUpdate = cmdUpdate + "  , FechaHoraLeido='" + FechaHoraLeido.ToString("yyyy/MM/dd HH:mm:ss") + "'";

            if (!string.IsNullOrEmpty(Descripcion))
                cmdUpdate = cmdUpdate + "  , Descripcion='" + Descripcion + "'" ;

            cmdUpdate = cmdUpdate + " WHERE Id = '" + Id + "'";

            consulta[0] = cmdUpdate;
            return consulta;
        }

        public override string[] DeleteSQL()
        {
            string[] consulta = new string[1];

            Consulta.Remove(0, Consulta.Length);
            if (FechaHora != null)
                Consulta.Append("DELETE FROM LogProcesos WHERE FechaHoraLeido is not null AND FechaHora='" + FechaHora.ToString() + "'");
            else
                Consulta.Append("DELETE FROM LogProcesos WHERE FechaHoraLeido is not null ");

            consulta[0] = Consulta.ToString();
            return consulta;
        }

    }
}
