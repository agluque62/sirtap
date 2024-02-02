using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace CD40.BD.Entidades
{
	public class Misiones : Tablas
	{
        #region Propiedades de Misiones

        private uint _IdMision;
        public uint IdMision
        {
            get { return _IdMision; }
            set { _IdMision = value; }
        }

        private string _Descripcion;
        public string Descripcion
        {
            get { return _Descripcion; }
            set { _Descripcion = value; }
        }

		private uint _Tipo;
        public uint Tipo
        {
            get { return _Tipo; }
            set { _Tipo = value; }
        }

        #endregion


        public Misiones()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdMision != 0)
                Consulta.Append("SELECT * FROM Misiones WHERE IdMision=" + IdMision);
            else 
                Consulta.Append("SELECT * FROM Misiones");
            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();
            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    Misiones r = new Misiones();

                    r.IdMision = (uint)dr["IdMision"];
                    r.Descripcion = (string)dr["Descripcion"];
                    r.Tipo = (uint)dr["Tipo"];

                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO Misiones (Descripcion,Tipo)" +
                            " VALUES ('" + Descripcion + "'," +
                                         Tipo + ")");
			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones");
			return consulta;
        }

        public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE Misiones SET Descripcion='" + Descripcion + "'," +
                                            "Tipo=" + Tipo + " " +
                                            "WHERE IdMision=" + IdMision
                                            );

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones");
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
			if (IdMision != 0)
				Consulta.Append("DELETE FROM Misiones WHERE IdMision=" + IdMision);
			else
				Consulta.Append("DELETE FROM Misiones");

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones");
			return consulta;
		}

	}
}
