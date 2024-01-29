using System;
using System.Data;
using System.Collections.Generic;
using System.Text;

namespace CD40.BD.Entidades
{
    public class Conferencias : Tablas
    {
        #region Propiedades de Conferencias
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

        private string _IdSalaBkk;
        public string IdSalaBkk
        {
            get { return _IdSalaBkk; }
            set { _IdSalaBkk = value; }
        }

        private int _TipoConferencia;
        public int TipoConferencia
        {
            get { return _TipoConferencia; }
            set { _TipoConferencia = value; }
        }

        private int _PosHMI;
        public int PosHMI
        {
            get { return _PosHMI; }
            set { _PosHMI = value; }
        }
        
        private string _Descripcion;
        public string Descripcion
        {
            get { return _Descripcion; }
            set { _Descripcion = value; }
        }

        private string _Alias;
        public string Alias
        {
            get { return _Alias; }
            set { _Alias = value; }
        }        
        #endregion

        public Conferencias()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdSistema != null)
                Consulta.Append("SELECT * FROM Conferencias WHERE IdSistema='" + IdSistema + "' ORDER BY IdConferencia ASC");
            else
                Consulta.Append("SELECT * FROM Conferencias ORDER BY PosHMI ASC");
            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();
            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    Conferencias r = new Conferencias();
                    if (dr["IdSistema"] != System.DBNull.Value)
                        r.IdSistema = (string)dr["IdSistema"];
                    if (dr["IdConferencia"] != System.DBNull.Value)
                        r.IdConferencia = (string)dr["IdConferencia"];
                    if (dr["IdSalaBkk"] != System.DBNull.Value)
                        r.IdSalaBkk = (string)dr["IdSalaBkk"];
                    if (dr["TipoConferencia"] != System.DBNull.Value)
                        r.TipoConferencia = (int)dr["TipoConferencia"];
                    if (dr["PosHMI"] != System.DBNull.Value)
                        r.PosHMI = (int)dr["PosHMI"];
                    if (dr["Descripcion"] != System.DBNull.Value)
                        r.Descripcion = (string)dr["Descripcion"];
                    if (dr["Alias"] != System.DBNull.Value)
                        r.Alias = (string)dr["Alias"];
                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];
            Consulta.Remove(0, Consulta.Length);

            Consulta.Append("INSERT INTO Conferencias VALUES ('" + IdSistema + "', '" + IdConferencia + "', '" + IdSalaBkk + "', " + TipoConferencia + ", " + PosHMI + ",'" + Descripcion + "', '"  + Alias + "')");

			consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "Conferencias");
			return consulta;
        }

        public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];
			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE Conferencias SET IdSalaBkk= '" + IdSalaBkk + 
                "', TipoConferencia =" + TipoConferencia + 
                ", PosHMI =" + PosHMI + 
                ", Descripcion='" + Descripcion + "' " +
                ", Alias='" + Alias + "' " +
                " WHERE IdSistema='" + IdSistema + "' AND IdConferencia='" + IdConferencia + "'"
                );
			consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "Conferencias");
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[4];

			Consulta.Remove(0, Consulta.Length);
            if (IdSistema != null && IdConferencia != null )
                Consulta.Append("DELETE FROM Conferencias WHERE IdConferencia ='" + IdConferencia + "' AND IdSistema='" + IdSistema + "' ");
            else if (IdSistema != null)
                Consulta.Append("DELETE FROM Conferencias WHERE IdSistema='" + IdSistema + "'");
            consulta[0] = Consulta.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "Conferencias");
		    return consulta;
		}

    }
}