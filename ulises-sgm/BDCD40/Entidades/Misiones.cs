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
        // Modo de Operación
        private bool _Seguro;
        public bool Seguro
        {
            get { return _Seguro; }
            set { _Seguro = value; }
        }
        #endregion


        public Misiones()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (!String.IsNullOrEmpty(Descripcion))
                Consulta.Append("SELECT * FROM Misiones WHERE Descripcion='" + Descripcion +"'");
            else if (IdMision > 0)
                Consulta.Append("SELECT * FROM Misiones WHERE IdMision=" + IdMision);
            else 
                Consulta.Append("SELECT * FROM Misiones ORDER BY Descripcion");
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
                    r.Seguro = Convert.ToBoolean(dr["Seguro"]);
                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO Misiones (Descripcion,Tipo,Seguro)" +
                            " VALUES ('" + Descripcion + "'," +
                                         Tipo +
                                         "," + Seguro +")");
			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones");
			return consulta;
        }

        public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE Misiones SET Descripcion='" + Descripcion + "'," +
                                            "Tipo=" + Tipo + ", " +
                                            "Seguro=" + Seguro + " " +
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
